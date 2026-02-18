#ifndef MENTAL_VK_RENDERER_H
#define MENTAL_VK_RENDERER_H

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_raii.hpp>

class MFW_C_Renderer {

private:
    vk::raii::Context vkContext;

}; // class MFW_C_Renderer

#endif // MENTAL_VK_RENDERER_H