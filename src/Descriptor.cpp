//
// Created by Alexander Blinov on 22.05.2023.
//

#include "../include/Descriptor.h"

VKFS::Descriptor::Descriptor(VKFS::Device *device, VkDescriptorType type, VkShaderStageFlagBits shaderStage) : device(device) {

    // Create bindings

    VkDescriptorSetLayoutBinding layoutBinding{};
    layoutBinding.binding = 0;
    layoutBinding.descriptorCount = 1;
    layoutBinding.descriptorType = type;
    layoutBinding.pImmutableSamplers = nullptr;
    layoutBinding.stageFlags = shaderStage;

    std::array<VkDescriptorSetLayoutBinding, 1> bindings = {layoutBinding};
    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    layoutInfo.pBindings = bindings.data();

    if (vkCreateDescriptorSetLayout(device->getDevice(), &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create descriptor set layout!");
    }

    // Create pool

    std::array<VkDescriptorPoolSize, 1> poolSizes{};
    poolSizes[0].type = type;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(2);

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(2);

    if (vkCreateDescriptorPool(device->getDevice(), &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create descriptor pool!");
    }

}

VkDescriptorSetLayout VKFS::Descriptor::getDescriptorSetLayout() {
    return this->descriptorSetLayout;
}

VkDescriptorPool VKFS::Descriptor::getDescriptorPool() {
    return this->descriptorPool;
}

void VKFS::Descriptor::createUBOSet(unsigned int sizeOf) {
    VkDeviceSize bufferSize = sizeOf;

    uniformBuffers.resize(2);
    uniformBuffersMemory.resize(2);
    uniformBuffersMapped.resize(2);

    for (size_t i = 0; i < 2; i++) {
        device->createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(device->getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }


    std::vector<VkDescriptorSetLayout> layouts(2, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(2);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(2);
    if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < 2; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeOf;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void VKFS::Descriptor::createSamplerSet(VkDescriptorImageInfo sampler) {
    std::vector<VkDescriptorSetLayout> layouts(2, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(2);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(2);
    if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < 2; i++) {
        sampler.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        sampler.imageView = sampler.imageView;
        sampler.sampler = sampler.sampler;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pImageInfo = &sampler;

        vkUpdateDescriptorSets(device->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}

void *VKFS::Descriptor::getBufferForUpdate(VKFS::Synchronization *sync) {
    return this->uniformBuffersMapped[sync->getCurrentFrame()];
}

VkDescriptorSet VKFS::Descriptor::getSet(VKFS::Synchronization *sync) {
    return this->descriptorSets[sync->getCurrentFrame()];
}

void VKFS::Descriptor::createStorageBufferSet(unsigned int sizeOf) {
    VkDeviceSize bufferSize = sizeOf * 1000;

    uniformBuffers.resize(2);
    uniformBuffersMemory.resize(2);
    uniformBuffersMapped.resize(2);

    for (size_t i = 0; i < 2; i++) {
        device->createBuffer(bufferSize, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);

        vkMapMemory(device->getDevice(), uniformBuffersMemory[i], 0, bufferSize, 0, &uniformBuffersMapped[i]);
    }


    std::vector<VkDescriptorSetLayout> layouts(2, descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(2);
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(2);
    if (vkAllocateDescriptorSets(device->getDevice(), &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < 2; i++) {
        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeOf * 1000;

        std::array<VkWriteDescriptorSet, 1> descriptorWrites{};

        descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrites[0].dstSet = descriptorSets[i];
        descriptorWrites[0].dstBinding = 0;
        descriptorWrites[0].dstArrayElement = 0;
        descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
        descriptorWrites[0].descriptorCount = 1;
        descriptorWrites[0].pBufferInfo = &bufferInfo;

        vkUpdateDescriptorSets(device->getDevice(), static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);
    }
}
