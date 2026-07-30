/**
 * @file ui_context.hpp
 * @brief Retained, layout-driven screen UI for Cbit2d scenes.
 */

#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

struct SDL_Renderer;
namespace cbit2d::core { class Scene; }

namespace cbit::ui {

struct UiSize {
    float width = 0.0F;
    float height = 0.0F;
};

struct UiRect {
    float x = 0.0F;
    float y = 0.0F;
    float width = 0.0F;
    float height = 0.0F;
};

struct UiColor {
    unsigned char r = 255;
    unsigned char g = 255;
    unsigned char b = 255;
    unsigned char a = 255;
};

struct UiInsets {
    float left = 0.0F;
    float top = 0.0F;
    float right = 0.0F;
    float bottom = 0.0F;
};

enum class UiLengthType { Auto, Pixels, Percent, Grow };

struct UiLength {
    UiLengthType type = UiLengthType::Auto;
    float value = 0.0F;

    [[nodiscard]] static constexpr UiLength autoSize() { return {}; }
    [[nodiscard]] static constexpr UiLength pixels(float value) { return {UiLengthType::Pixels, value}; }
    [[nodiscard]] static constexpr UiLength percent(float value) { return {UiLengthType::Percent, value}; }
    [[nodiscard]] static constexpr UiLength grow(float value = 1.0F) { return {UiLengthType::Grow, value}; }
};

enum class UiAlignment { Start, Center, End, Stretch };
enum class UiWidgetType { Panel, Row, Column, Overlay, Text, Button };

struct UiLayout {
    UiLength width = UiLength::autoSize();
    UiLength height = UiLength::autoSize();
    UiLength minWidth = UiLength::autoSize();
    UiLength minHeight = UiLength::autoSize();
    UiLength maxWidth = UiLength::autoSize();
    UiLength maxHeight = UiLength::autoSize();
    UiInsets padding {};
    float gap = 0.0F;
    UiAlignment horizontalAlignment = UiAlignment::Start;
    UiAlignment verticalAlignment = UiAlignment::Start;
    bool clipChildren = false;
    bool visible = true;
};

struct UiTextStyle {
    std::string fontPath;
    float fontSize = 24.0F;
    UiColor color {};
    bool centered = true;
};

struct UiButtonStyle {
    UiColor backgroundColor {35, 46, 77, 255};
    UiColor hoverColor {54, 71, 118, 255};
    UiColor pressedColor {22, 29, 49, 255};
    UiColor borderColor {241, 196, 15, 255};
};

class UiContext;

class UiWidget {
public:
    UiWidget(const UiWidget&) = delete;
    UiWidget& operator=(const UiWidget&) = delete;
    UiWidget(UiWidget&&) = delete;
    UiWidget& operator=(UiWidget&&) = delete;
    ~UiWidget();

    [[nodiscard]] UiWidgetType type() const;
    [[nodiscard]] UiLayout& layout();
    [[nodiscard]] const UiLayout& layout() const;
    [[nodiscard]] const UiRect& rect() const;
    [[nodiscard]] UiSize intrinsicSize() const;

    UiWidget& addPanel();
    UiWidget& addRow();
    UiWidget& addColumn();
    UiWidget& addOverlay();
    UiWidget& addText(std::string content, UiTextStyle style = {});
    UiWidget& addButton(std::string label, std::function<void()> onClick, UiButtonStyle style = {});

    void setText(std::string content);
    void setTextStyle(UiTextStyle style);
    void setButtonStyle(UiButtonStyle style);

private:
    friend class UiContext;
    explicit UiWidget(UiContext& context, UiWidgetType type);
    UiWidget& addChild(UiWidgetType type);

    UiContext& _context;
    UiWidgetType _type;
    UiLayout _layout;
    UiRect _rect;
    UiRect _clipRect;
    std::string _text;
    UiTextStyle _textStyle;
    UiButtonStyle _buttonStyle;
    std::function<void()> _onClick;
    bool _hovered = false;
    bool _pressed = false;
    std::vector<std::unique_ptr<UiWidget>> _children;
    struct Impl;
    std::unique_ptr<Impl> _impl;
};

class UiContext {
public:
    UiContext();
    ~UiContext();
    UiContext(const UiContext&) = delete;
    UiContext& operator=(const UiContext&) = delete;

    [[nodiscard]] UiWidget& root();
    void setViewport(UiSize viewport);
    [[nodiscard]] UiSize viewport() const;
    void update();

private:
    friend class UiWidget;
    friend class cbit2d::core::Scene;
    void markLayoutDirty();
    void render(::SDL_Renderer* renderer);
    struct Impl;
    std::unique_ptr<Impl> _impl;
    std::unique_ptr<UiWidget> _root;
};

} // namespace cbit::ui
