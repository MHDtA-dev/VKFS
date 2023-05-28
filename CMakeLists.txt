cmake_minimum_required(VERSION 3.20)
project(VKFS)

set(CMAKE_CXX_STANDARD 17)
#set(VULKAN_INCLUDE_PATH "" CACHE PATH "Path to Vulkan SDK include path")

#set_property(CACHE VULKAN_INCLUDE_PATH PROPERTY STRINGS "")

#if (NOT VULKAN_INCLUDE_PATH)
#    message(FATAL_ERROR "Vulkan SDK include dir is not set!")
#endif ()

#include_directories(${VULKAN_INCLUDE_PATH})
include_directories(/Users/user/VulkanSDK/1.3.243.0/macOS/include)
add_library(VKFS src/Instance.cpp include/Instance.h src/Device.cpp include/Device.h src/Swapchain.cpp include/Swapchain.h src/ShaderModule.cpp include/ShaderModule.h src/CommandBuffer.cpp include/CommandBuffer.h src/Synchronization.cpp include/Synchronization.h include/VKFS.h src/VertexBuffer.cpp include/VertexBuffer.h src/Descriptor.cpp include/Descriptor.h src/VKFS.cpp)
target_link_libraries(VKFS vulkan)