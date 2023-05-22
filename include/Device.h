

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
