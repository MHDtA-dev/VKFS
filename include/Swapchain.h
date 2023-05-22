

#ifndef VKFS_SWAPCHAIN_H
#define VKFS_SWAPCHAIN_H

#include "Device.h"
#include <array>

namespace VKFS {

    class Swapchain {
        public:
            Swapchain(Device* device, int windowWidth, int windowHeight);

            VkRenderPass getRenderPass();
            VkSwapchainKHR getSwapchain();
            void recreate();
            VkFramebuffer getFramebuffer(uint32_t imageIndex);
            VkExtent2D getExtent();

        private:
            Device* device;
            VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
            VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
            VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities);
            VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
            void createFramebuffers();
            void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

            VkSwapchainKHR swapchain;
            std::vector<VkImage> swapchainImages;
            VkFormat swapchainImageFormat;
            VkExtent2D swapchainExtent;
            std::vector<VkImageView> swapchainImageViews;
            std::vector<VkFramebuffer> swapchainFramebuffers;
            VkImage depthImage;
            VkDeviceMemory depthImageMemory;
            VkImageView depthImageView;

            VkRenderPass renderPass;

            int windowWidth, windowHeight;
    };

}

#endif //VKFS_SWAPCHAIN_H
