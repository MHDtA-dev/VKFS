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

#ifndef VKFS_IMAGE_H
#define VKFS_IMAGE_H

#include <iostream>
#include <vector>
#include <cmath>
#include <vulkan/vulkan.h>
#include "Device.h"


namespace VKFS {

    enum ImageFilter {
        IMG_NEAREST, IMG_LINEAR
    };

    class Image {
        public:
            Image(Device* device, int width, int height, void* pixels, bool generateMipmaps = true, ImageFilter filter = ImageFilter::IMG_LINEAR);
            ~Image();

            VkImage getImage();
            VkImageView getImageView();
            VkSampler getSampler();
            VkDescriptorImageInfo getDescriptorImageInfo();
            uint32_t getMipLevels();

        private:
            Device* d;
            ImageFilter f;

            VkImage image;
            VkDeviceMemory imageMemory;
            VkImageView imageView;
            VkSampler sampler;
            VkDescriptorImageInfo imageInfo;
            uint32_t mipLevels;

            void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkFormat format,
                             VkImageTiling tiling, VkImageUsageFlags usage,
                             VkMemoryPropertyFlags properties, VkImage &image,
                             VkDeviceMemory &imageMemory);

            void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout,
                                       uint32_t mipLevels);

            void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
            void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
            VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
            void changeLayout();
    };

}


#endif //VKFS_IMAGE_H
