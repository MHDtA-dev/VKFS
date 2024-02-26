//
// Created by Alexander Blinov on 11.11.2023.
//

#include "../include/VKFS/ComputePipeline.h"

VKFS::ComputePipeline::ComputePipeline(VKFS::Device *device, VKFS::ShaderModule *computeShader,
                                       std::vector<VKFS::Descriptor *> descriptors) : device(device), descriptors(descriptors) {
    VkPipelineShaderStageCreateInfo computeShaderStageInfo{};
    computeShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    computeShaderStageInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    computeShaderStageInfo.module = computeShader->getShaderModule();
    computeShaderStageInfo.pName = "main";

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;

    std::vector<VkDescriptorSetLayout> lays;

    for (VKFS::Descriptor* desc : descriptors) {
        lays.push_back(desc->getDescriptorSetLayout());
    }

    pipelineLayoutInfo.setLayoutCount = lays.size();
    pipelineLayoutInfo.pSetLayouts = lays.data();

    if (vkCreatePipelineLayout(device->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create compute pipeline layout!");
    }

    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.layout = pipelineLayout;
    pipelineInfo.stage = computeShaderStageInfo;

    if (vkCreateComputePipelines(device->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create compute pipeline!");
    }
}

void VKFS::ComputePipeline::dispatch(VKFS::Synchronization *sync, int workingGroupCountX, int workingGroupCountY,
                                     int workingGroupCountZ) {
    vkCmdBindPipeline(sync->getComputeCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, pipeline);

    for (int i = 0; i < descriptors.size(); i++) {

        VkDescriptorSet set = descriptors[i]->getSet(sync);

        vkCmdBindDescriptorSets(sync->getComputeCommandBuffer(), VK_PIPELINE_BIND_POINT_COMPUTE, pipelineLayout, i, 1, &set, 0, nullptr);
    }

    vkCmdDispatch(sync->getComputeCommandBuffer(), workingGroupCountX, workingGroupCountY, workingGroupCountZ);
}
