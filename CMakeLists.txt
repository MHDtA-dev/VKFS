cmake_minimum_required(VERSION 3.20)
project(VKFS)

set(CMAKE_CXX_STANDARD 17)

find_package(Vulkan REQUIRED)
include_directories(${Vulkan_INCLUDE_DIRS})

add_library(VKFS src/Instance.cpp include/VKFS/Instance.h src/Device.cpp include/VKFS/Device.h src/Swapchain.cpp include/VKFS/Swapchain.h src/ShaderModule.cpp include/VKFS/ShaderModule.h src/CommandBuffer.cpp include/VKFS/CommandBuffer.h src/Synchronization.cpp include/VKFS/Synchronization.h include/VKFS/VKFS.h src/VertexBuffer.cpp include/VKFS/VertexBuffer.h src/Descriptor.cpp include/VKFS/Descriptor.h src/VKFS.cpp src/Pipeline.cpp include/VKFS/Pipeline.h src/Offscreen.cpp include/VKFS/Offscreen.h src/Image.cpp include/VKFS/Image.h src/Extensions/ShapeConstructor.cpp include/VKFS/Extensions/ShapeConstructor.h include/VKFS/VKFS_Extensions.h include/VKFS/__utils.h src/ComputePipeline.cpp include/VKFS/ComputePipeline.h src/StorageImage.cpp include/VKFS/StorageImage.h)
target_link_libraries(VKFS ${Vulkan_LIBRARIES})