#include "mental_glfw_window.h"

#include <GLFW/glfw3.h>
#include <stdexcept>

MFW_C_Window::MFW_C_Window() {
    if (!glfwInit()) throw std::runtime_error("Failed to initialize GLFW library.");

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, MENTAL_GLFW_CONTEXT_VERSION_MAJOR);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, MENTAL_GLFW_CONTEXT_VERSION_MINOR);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    GLFWwindow* _window_ptr = glfwCreateWindow((int)this->_size[0], (int)this->_size[1], "MentalFX example", nullptr, nullptr);
    this->_window_uptr.reset(_window_ptr);
    glfwSetWindowUserPointer(_window_ptr, this);

    if (!this->_window_uptr) {
        throw std::runtime_error("Failed to create GLFW window!");
    }

}

MFW_C_Window::~MFW_C_Window() {
    if (this->_window_uptr != nullptr) glfwDestroyWindow(this->_window_uptr.get());
    glfwTerminate();
}


void MFW_C_Window::run() {
    while (!this->windowShouldClose())
    {
        glfwPollEvents();
        glfwSwapBuffers(this->_window_uptr.get());
    }
}
