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

#ifndef VKFS_SWAPCHAIN_H
#define VKFS_SWAPCHAIN_H

#include "Device.h"
#include <array>
#include <algorithm>
#include <cmath>

namespace VKFS {

    class Swapchain {
        public:
            Swapchain(Device* device, int windowWidth, int windowHeight);
            VkRenderPass getRenderPass();
            VkSwapchainKHR getSwapchain();
            void recreate(int windowWidth, int windowHeight);
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
            void create();
    };

}

#endif //VKFS_SWAPCHAIN_H
