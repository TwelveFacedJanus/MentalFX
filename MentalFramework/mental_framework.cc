#include "mental_framework.h"

mental_framework::MFW_C_Window::MFW_C_Window() {

}

mental_framework::MFW_C_Window::~MFW_C_Window() {

}

#ifdef MENTAL_ENABLE_GLFW

[[clang::always_inline]]
bool mental_framework::MFW_C_Window::windowShouldClose() {
    #ifdef MENTAL_ENABLE_GLFW
        return glfwWindowShouldClose();
    #endif
}

void mental_framework::MFW_C_Window::run() {
    while (!glfwWindowShouldClose())
    {
        glfwPollEvents()
    }
}

#else


bool mental_framework::MFW_C_Window::windowShouldClose() {
    return false;
}


void mental_framework::MFW_C_Window::run() {
    while (!this->windowShouldClose())
    {

    }
}

#endif
