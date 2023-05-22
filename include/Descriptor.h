//
// Created by Alexander Blinov on 22.05.2023.
//

#ifndef VKFS_DESCRIPTOR_H
#define VKFS_DESCRIPTOR_H

#include "Device.h"
#include "Synchronization.h"
#include <array>

namespace VKFS {

    class Descriptor {
        public:
            Descriptor(Device* device, VkDescriptorType type, VkShaderStageFlagBits shaderStage);

            void createUBOSet(unsigned int sizeOf);
            void createSamplerSet(VkDescriptorImageInfo sampler);

            void* getUBOforUpdate(Synchronization* sync);
            VkDescriptorSet getSet(Synchronization* sync);

            VkDescriptorSetLayout getDescriptorSetLayout();
            VkDescriptorPool getDescriptorPool();
        private:
            Device* device;

            VkDescriptorSetLayout descriptorSetLayout;
            VkDescriptorPool descriptorPool;

            std::vector<VkDescriptorSet> descriptorSets;

            std::vector<VkBuffer> uniformBuffers;
            std::vector<VkDeviceMemory> uniformBuffersMemory;
            std::vector<void*> uniformBuffersMapped;
    };

}

#endif //VKFS_DESCRIPTOR_H
