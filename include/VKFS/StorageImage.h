//
// Created by Alexander Blinov on 12.11.2023.
//

#ifndef VKFS_STORAGEIMAGE_H
#define VKFS_STORAGEIMAGE_H

#include <iostream>
#include "Device.h"
#include "Synchronization.h"

namespace VKFS {

    class StorageImage {
        public:
            StorageImage(VKFS::Device* device, int width, int height);
            VkDescriptorImageInfo getDescriptorImageInfoStorage();
            VkDescriptorImageInfo getDescriptorImageInfoShader();
            void copyStorageImageToShaderImage(VKFS::Synchronization* sync);

        private:
            VKFS::Device* d;

            int width_;
            int height_;
            VkImage image_;
            VkDeviceMemory imageMemory_;
            VkImageView imageView_;
            VkSampler sampler_;
            VkDescriptorImageInfo descriptorImageInfo;

            VkImage shaderImage;
            VkImageView shaderImageView;
            VkDeviceMemory shaderImageMemory;
            VkDescriptorImageInfo shaderDescriptorImageInfo;

            void createImageForShader();
    };

}

#endif //VKFS_STORAGEIMAGE_H
