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

#ifndef VKFS_OFFSCREEN_H
#define VKFS_OFFSCREEN_H

#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>
#include "Device.h"
#include "Synchronization.h"


namespace VKFS {

    enum OffscreenType {
        OFFSCR_COLOR, OFFSCR_DEPTH, OFFSCR_COLOR_AND_DEPTH
    };

    enum OffscreenImageFilter {
        OFFSCR_LINEAR, OFFSRC_NEAREST
    };

    class Offscreen {
        public:
            Offscreen(Device* device, Synchronization* sync, OffscreenType type, int width, int height, OffscreenImageFilter imageFilter = OffscreenImageFilter::OFFSCR_LINEAR);

            void beginRenderpass(float clearR = 0, float clearG = 0, float clearB = 0, float clearA = 0);
            void endRenderpass();

            VkRenderPass getRenderPass();
            VkExtent2D getExtent();
            VkFramebuffer getFramebuffer();
            VkSampler getSampler();
            VkDescriptorImageInfo getImageInfo();

        private:
            Device* d;
            OffscreenType t;
            OffscreenImageFilter filter;
            Synchronization* sync;

            int w, h;
            VkExtent2D size;

            VkImage colorImage;
            VkDeviceMemory colorImageMemory;
            VkImageView colorImageView;

            VkImage depthImage;
            VkDeviceMemory depthImageMemory;
            VkImageView depthImageView;

            VkFramebuffer framebuffer;
            VkRenderPass renderPass;
            VkSampler sampler;
            VkDescriptorImageInfo imageInfo;

            std::vector<VkAttachmentDescription> attachments;

            void createColorImage();
            void createDepthImage();
            void createSampler();

    };

}


#endif //VKFS_OFFSCREEN_H
