/*
(c) Copyright 2023 MHDtA

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef VKFS_DEVICE_H
#define VKFS_DEVICE_H

#include "Instance.h"
#include <optional>
#include <set>

namespace VKFS {

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;
        std::optional<uint32_t> presentFamily;

        bool isComplete() {
            return graphicsFamily.has_value() && presentFamily.has_value();
        }
    };

    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR capabilities;
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR> presentModes;
    };

    class Device {
        public:
            Device(VKFS::Instance* instance, std::vector<const char*> deviceExtensions);

            VkPhysicalDevice getPhysicalDevice();
            VkDevice getDevice();

            VkQueue getGraphicsQueue();
            VkQueue getPresentQueue();
            VkCommandPool getCommandPool();

            VkCommandBuffer beginSingleTimeCommands();
            void endSingleTimeCommands(VkCommandBuffer commandBuffer);
            void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);

            Instance* getInstance();

            SwapChainSupportDetails getSwapchainSupport();
            QueueFamilyIndices findQueueFamilies();
            VkFormat findDepthFormat();
            uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
            void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);

        private:
            Instance* instance;
            std::vector<const char*> deviceExtensions;

            VkPhysicalDevice physicalDevice;
            VkDevice device;

            VkQueue graphicsQueue = nullptr;
            VkQueue presentQueue = nullptr;
            VkCommandPool commandPool;

            void createLogicalDevice();
            void createCommandPool();

            bool isDeviceSuitable(VkPhysicalDevice device);
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
            bool checkDeviceExtensionSupport(VkPhysicalDevice device);
            SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device);
            VkFormat findSupportedFormat(const std::vector<VkFormat>& candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
    };

}


#endif //VKFS_DEVICE_H
