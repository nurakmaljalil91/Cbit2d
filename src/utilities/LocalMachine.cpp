//
// Created by User on 23/7/2024.
//

#include "LocalMachine.h"

const char *LocalMachine::getFontPath() {
    // check the operating system
#ifdef _WIN32
    return "C:\\Windows\\Fonts\\arial.ttf";
#elif __APPLE__
    return "/Library/Fonts/Arial.ttf";
#elif __linux__
        return "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf";
#else
const char* getFontPath() {
    std::cerr << "Unsupported OS" << std::endl;
    return nullptr;
#endif
}
