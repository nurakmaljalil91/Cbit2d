#include "cbit/ui/ui_context.hpp"

#include <algorithm>
#include <filesystem>
#include <unordered_map>

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <yoga/Yoga.h>

#include "cbit/core/input.hpp"

namespace cbit::ui {
namespace {

[[nodiscard]] SDL_Color toSdlColor(const UiColor color)
{
    return {color.r, color.g, color.b, color.a};
}

[[nodiscard]] UiRect intersect(const UiRect first, const UiRect second)
{
    const float left = std::max(first.x, second.x);
    const float top = std::max(first.y, second.y);
    const float right = std::min(first.x + first.width, second.x + second.width);
    const float bottom = std::min(first.y + first.height, second.y + second.height);
    return {left, top, std::max(0.0F, right - left), std::max(0.0F, bottom - top)};
}

[[nodiscard]] bool contains(const UiRect rect, const SDL_FPoint point)
{
    return point.x >= rect.x && point.x <= rect.x + rect.width
        && point.y >= rect.y && point.y <= rect.y + rect.height;
}

YGAlign toYogaAlign(const UiAlignment alignment)
{
    switch (alignment) {
    case UiAlignment::Center: return YGAlignCenter;
    case UiAlignment::End: return YGAlignFlexEnd;
    case UiAlignment::Stretch: return YGAlignStretch;
    case UiAlignment::Start: return YGAlignFlexStart;
    }
    return YGAlignFlexStart;
}

YGJustify toYogaJustify(const UiAlignment alignment)
{
    switch (alignment) {
    case UiAlignment::Center: return YGJustifyCenter;
    case UiAlignment::End: return YGJustifyFlexEnd;
    case UiAlignment::Stretch:
    case UiAlignment::Start: return YGJustifyFlexStart;
    }
    return YGJustifyFlexStart;
}

void applyDimension(YGNodeRef node, const UiLength length, const bool width, const bool minimum = false, const bool maximum = false)
{
    const auto pixels = width
        ? (minimum ? YGNodeStyleSetMinWidth : maximum ? YGNodeStyleSetMaxWidth : YGNodeStyleSetWidth)
        : (minimum ? YGNodeStyleSetMinHeight : maximum ? YGNodeStyleSetMaxHeight : YGNodeStyleSetHeight);
    const auto percent = width
        ? (minimum ? YGNodeStyleSetMinWidthPercent : maximum ? YGNodeStyleSetMaxWidthPercent : YGNodeStyleSetWidthPercent)
        : (minimum ? YGNodeStyleSetMinHeightPercent : maximum ? YGNodeStyleSetMaxHeightPercent : YGNodeStyleSetHeightPercent);

    if (length.type == UiLengthType::Pixels) {
        pixels(node, length.value);
    } else if (length.type == UiLengthType::Percent) {
        percent(node, length.value);
    } else if (length.type == UiLengthType::Grow && !minimum && !maximum) {
        YGNodeStyleSetFlexGrow(node, length.value);
    }
}

} // namespace

struct UiWidget::Impl { YGNodeRef node = nullptr; };

struct UiContext::Impl {
    UiSize viewport {};
    bool dirty = true;
    bool ownsTtf = false;
    std::unordered_map<std::string, TTF_Font*> fonts;

    ~Impl()
    {
        for (auto& [key, font] : fonts) {
            static_cast<void>(key);
            TTF_CloseFont(font);
        }
        if (ownsTtf) {
            TTF_Quit();
        }
    }

    [[nodiscard]] TTF_Font* font(const UiTextStyle& style)
    {
        if (style.fontPath.empty()) {
            return nullptr;
        }
        if (TTF_WasInit() == 0) {
            if (!TTF_Init()) {
                return nullptr;
            }
            ownsTtf = true;
        }
        const std::string key = style.fontPath + "#" + std::to_string(style.fontSize);
        if (const auto found = fonts.find(key); found != fonts.end()) {
            return found->second;
        }
        for (const auto& path : {std::filesystem::path(style.fontPath), std::filesystem::path("..") / style.fontPath}) {
            if (TTF_Font* opened = TTF_OpenFont(path.string().c_str(), style.fontSize); opened != nullptr) {
                fonts.emplace(key, opened);
                return opened;
            }
        }
        return nullptr;
    }
};

UiWidget::UiWidget(UiContext& context, const UiWidgetType type)
    : _context(context), _type(type), _impl(std::make_unique<Impl>())
{
    _impl->node = YGNodeNew();
    YGNodeSetContext(_impl->node, this);
    if (type == UiWidgetType::Row) {
        YGNodeStyleSetFlexDirection(_impl->node, YGFlexDirectionRow);
    } else {
        YGNodeStyleSetFlexDirection(_impl->node, YGFlexDirectionColumn);
    }
}

UiWidget::~UiWidget() = default;
UiWidgetType UiWidget::type() const { return _type; }
UiLayout& UiWidget::layout() { _context.markLayoutDirty(); return _layout; }
const UiLayout& UiWidget::layout() const { return _layout; }
const UiRect& UiWidget::rect() const { return _rect; }
UiSize UiWidget::intrinsicSize() const
{
    if (_type != UiWidgetType::Text && _type != UiWidgetType::Button) return {};
    TTF_Font* font = _context._impl->font(_textStyle);
    int width = 0;
    int height = 0;
    if (font != nullptr) TTF_GetStringSize(font, _text.c_str(), _text.size(), &width, &height);
    if (_type == UiWidgetType::Button) { width += 32; height += 20; }
    return {static_cast<float>(width), static_cast<float>(height)};
}

UiWidget& UiWidget::addChild(const UiWidgetType type)
{
    auto child = std::unique_ptr<UiWidget>(new UiWidget(_context, type));
    YGNodeInsertChild(_impl->node, child->_impl->node, YGNodeGetChildCount(_impl->node));
    _children.push_back(std::move(child));
    _context.markLayoutDirty();
    return *_children.back();
}
UiWidget& UiWidget::addPanel() { return addChild(UiWidgetType::Panel); }
UiWidget& UiWidget::addRow() { return addChild(UiWidgetType::Row); }
UiWidget& UiWidget::addColumn() { return addChild(UiWidgetType::Column); }
UiWidget& UiWidget::addOverlay() { return addChild(UiWidgetType::Overlay); }
UiWidget& UiWidget::addText(std::string content, UiTextStyle style)
{
    UiWidget& child = addChild(UiWidgetType::Text);
    child._text = std::move(content);
    child._textStyle = std::move(style);
    return child;
}
UiWidget& UiWidget::addButton(std::string label, std::function<void()> onClick, UiButtonStyle style)
{
    UiWidget& child = addChild(UiWidgetType::Button);
    child._text = std::move(label);
    child._onClick = std::move(onClick);
    child._buttonStyle = style;
    return child;
}
void UiWidget::setText(std::string content) { _text = std::move(content); _context.markLayoutDirty(); }
void UiWidget::setTextStyle(UiTextStyle style) { _textStyle = std::move(style); _context.markLayoutDirty(); }
void UiWidget::setButtonStyle(UiButtonStyle style) { _buttonStyle = style; }

UiContext::UiContext()
    : _impl(std::make_unique<Impl>()), _root(std::unique_ptr<UiWidget>(new UiWidget(*this, UiWidgetType::Column)))
{
    _root->_layout.width = UiLength::grow();
    _root->_layout.height = UiLength::grow();
}

UiContext::~UiContext()
{
    if (_root && _root->_impl->node) {
        YGNodeFreeRecursive(_root->_impl->node);
        _root->_impl->node = nullptr;
    }
}

UiWidget& UiContext::root() { return *_root; }
void UiContext::setViewport(const UiSize viewport)
{
    if (_impl->viewport.width != viewport.width || _impl->viewport.height != viewport.height) {
        _impl->viewport = viewport;
        markLayoutDirty();
    }
}
UiSize UiContext::viewport() const { return _impl->viewport; }
void UiContext::markLayoutDirty() { _impl->dirty = true; }

static YGSize measureWidget(YGNodeConstRef node, float, YGMeasureMode, float, YGMeasureMode)
{
    const auto* widget = static_cast<const UiWidget*>(YGNodeGetContext(node));
    if (widget == nullptr) {
        return {0.0F, 0.0F};
    }
    const UiSize size = widget->intrinsicSize();
    return {size.width, size.height};
}

void UiContext::update()
{
    const SDL_FPoint mouse = cbit2d::core::Input::getMousePosition();
    const bool pressed = cbit2d::core::Input::isMouseButtonPressed(cbit2d::core::MouseButton::Left);
    std::function<bool(UiWidget&, UiRect)> visit = [&](UiWidget& widget, const UiRect clip) {
        if (!widget._layout.visible) return false;
        const UiRect effectiveClip = widget._layout.clipChildren ? intersect(clip, widget._rect) : clip;
        for (auto iterator = widget._children.rbegin(); iterator != widget._children.rend(); ++iterator) {
            if (visit(**iterator, effectiveClip)) return true;
        }
        if (widget._type != UiWidgetType::Button) return false;
        widget._hovered = contains(intersect(widget._rect, clip), mouse);
        widget._pressed = widget._hovered && cbit2d::core::Input::isMouseButtonHeld(cbit2d::core::MouseButton::Left);
        if (pressed && widget._hovered && widget._onClick) {
            widget._onClick();
            return true;
        }
        return false;
    };
    visit(*_root, {0.0F, 0.0F, _impl->viewport.width, _impl->viewport.height});
}

void UiContext::render(SDL_Renderer* renderer)
{
    if (renderer == nullptr || _impl->viewport.width <= 0.0F || _impl->viewport.height <= 0.0F) return;
    std::function<void(UiWidget&)> apply = [&](UiWidget& widget) {
        YGNodeRef node = widget._impl->node;
        YGNodeStyleSetDisplay(node, widget._layout.visible ? YGDisplayFlex : YGDisplayNone);
        YGNodeStyleSetPadding(node, YGEdgeLeft, widget._layout.padding.left);
        YGNodeStyleSetPadding(node, YGEdgeTop, widget._layout.padding.top);
        YGNodeStyleSetPadding(node, YGEdgeRight, widget._layout.padding.right);
        YGNodeStyleSetPadding(node, YGEdgeBottom, widget._layout.padding.bottom);
        YGNodeStyleSetGap(node, YGGutterAll, widget._layout.gap);
        applyDimension(node, widget._layout.width, true);
        applyDimension(node, widget._layout.height, false);
        applyDimension(node, widget._layout.minWidth, true, true);
        applyDimension(node, widget._layout.minHeight, false, true);
        applyDimension(node, widget._layout.maxWidth, true, false, true);
        applyDimension(node, widget._layout.maxHeight, false, false, true);
        const bool row = widget._type == UiWidgetType::Row;
        YGNodeStyleSetJustifyContent(node, toYogaJustify(row ? widget._layout.horizontalAlignment : widget._layout.verticalAlignment));
        YGNodeStyleSetAlignItems(node, toYogaAlign(row ? widget._layout.verticalAlignment : widget._layout.horizontalAlignment));
        if (widget._type == UiWidgetType::Text || widget._type == UiWidgetType::Button) YGNodeSetMeasureFunc(node, measureWidget);
        for (auto& child : widget._children) apply(*child);
    };
    if (_impl->dirty) {
        apply(*_root);
        YGNodeCalculateLayout(_root->_impl->node, _impl->viewport.width, _impl->viewport.height, YGDirectionLTR);
        std::function<void(UiWidget&, UiRect)> store = [&](UiWidget& widget, const UiRect parent) {
            YGNodeRef node = widget._impl->node;
            widget._rect = {parent.x + YGNodeLayoutGetLeft(node), parent.y + YGNodeLayoutGetTop(node), YGNodeLayoutGetWidth(node), YGNodeLayoutGetHeight(node)};
            widget._clipRect = widget._layout.clipChildren ? intersect(parent, widget._rect) : parent;
            for (auto& child : widget._children) store(*child, widget._clipRect);
        };
        store(*_root, {0.0F, 0.0F, _impl->viewport.width, _impl->viewport.height});
        _impl->dirty = false;
    }

    std::function<void(UiWidget&)> draw = [&](UiWidget& widget) {
        if (!widget._layout.visible) return;
        const SDL_FRect rect {widget._rect.x, widget._rect.y, widget._rect.width, widget._rect.height};
        if (widget._type == UiWidgetType::Panel || widget._type == UiWidgetType::Button) {
            UiColor color = widget._buttonStyle.backgroundColor;
            if (widget._type == UiWidgetType::Button && widget._hovered) color = widget._pressed ? widget._buttonStyle.pressedColor : widget._buttonStyle.hoverColor;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderFillRect(renderer, &rect);
            const UiColor border = widget._buttonStyle.borderColor;
            SDL_SetRenderDrawColor(renderer, border.r, border.g, border.b, border.a);
            SDL_RenderRect(renderer, &rect);
        }
        if (widget._type == UiWidgetType::Text || widget._type == UiWidgetType::Button) {
            if (TTF_Font* font = _impl->font(widget._textStyle); font != nullptr) {
                if (SDL_Surface* surface = TTF_RenderText_Blended(font, widget._text.c_str(), widget._text.size(), toSdlColor(widget._textStyle.color)); surface != nullptr) {
                    if (SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface); texture != nullptr) {
                        SDL_FRect destination {rect.x, rect.y, static_cast<float>(surface->w), static_cast<float>(surface->h)};
                        destination.x += (rect.w - destination.w) * 0.5F;
                        destination.y += (rect.h - destination.h) * 0.5F;
                        SDL_RenderTexture(renderer, texture, nullptr, &destination);
                        SDL_DestroyTexture(texture);
                    }
                    SDL_DestroySurface(surface);
                }
            }
        }
        for (auto& child : widget._children) draw(*child);
    };
    draw(*_root);
}

} // namespace cbit::ui
