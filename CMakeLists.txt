cmake_minimum_required(VERSION 3.20)
project(VKFS)

set(CMAKE_CXX_STANDARD 17)
set(VULKAN_INCLUDE_PATH "" CACHE PATH "Path to Vulkan SDK include path")

set_property(CACHE VULKAN_INCLUDE_PATH PROPERTY STRINGS "")

if (NOT VULKAN_INCLUDE_PATH)
    message(FATAL_ERROR "Vulkan SDK include dir is not set!")
endif ()

include_directories(${VULKAN_INCLUDE_PATH})



add_library(VKFS src/Instance.cpp include/VKFS/Instance.h src/Device.cpp include/VKFS/Device.h src/Swapchain.cpp include/VKFS/Swapchain.h src/ShaderModule.cpp include/VKFS/ShaderModule.h src/CommandBuffer.cpp include/VKFS/CommandBuffer.h src/Synchronization.cpp include/VKFS/Synchronization.h include/VKFS/VKFS.h src/VertexBuffer.cpp include/VKFS/VertexBuffer.h src/Descriptor.cpp include/VKFS/Descriptor.h src/VKFS.cpp src/Pipeline.cpp include/VKFS/Pipeline.h src/Offscreen.cpp include/VKFS/Offscreen.h src/Image.cpp include/VKFS/Image.h src/Extensions/ShapeConstructor.cpp include/VKFS/Extensions/ShapeConstructor.h include/VKFS/VKFS_Extensions.h include/VKFS/__utils.h)
target_link_libraries(VKFS vulkan)