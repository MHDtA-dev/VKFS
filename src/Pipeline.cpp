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

#include "../include/VKFS/Pipeline.h"

VKFS::Pipeline::Pipeline(VKFS::Device *device, VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attribDescription, VkRenderPass renderPass, std::vector<Descriptor*> descriptors) : d(device), renderPass(renderPass) {
    attributes = attribDescription;
    bindings = bindingDescription;
    this->descriptors = descriptors;
}

void VKFS::Pipeline::addShader(VKFS::ShaderType type, VKFS::ShaderModule *shader, std::string funcname) {
    VkPipelineShaderStageCreateInfo shaderStageInfo{};
    shaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderStageInfo.stage = getVulkanStage(type);
    shaderStageInfo.module = shader->getShaderModule();
    shaderStageInfo.pName = funcname.c_str();


    stages.push_back(shaderStageInfo);
}

VkShaderStageFlagBits VKFS::Pipeline::getVulkanStage(VKFS::ShaderType type) {
    switch (type) {
        case ShaderType::VERTEX:
            return VK_SHADER_STAGE_VERTEX_BIT;

        case ShaderType::FRAGMENT:
            return VK_SHADER_STAGE_FRAGMENT_BIT;

        case ShaderType::GEOMETRY:
            return VK_SHADER_STAGE_GEOMETRY_BIT;
    }
}

VkPipeline VKFS::Pipeline::getPipeline() {
    return this->pipeline;
}

VkPipelineLayout VKFS::Pipeline::getPipelineLayout() {
    return this->layout;
}

void VKFS::Pipeline::build() {
    VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
    vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertexInputInfo.vertexBindingDescriptionCount = 1;
    vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>(attributes.size());
    vertexInputInfo.pVertexBindingDescriptions = &bindings;
    vertexInputInfo.pVertexAttributeDescriptions = attributes.data();

    VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
    inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    inputAssembly.primitiveRestartEnable = VK_FALSE;

    VkPipelineViewportStateCreateInfo viewportState{};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.viewportCount = 1;
    viewportState.scissorCount = 1;

    VkPipelineRasterizationStateCreateInfo rasterizer{};
    rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    rasterizer.depthClampEnable = VK_FALSE;
    rasterizer.rasterizerDiscardEnable = VK_FALSE;
    rasterizer.polygonMode = (polygonMode == PolygonMode::FILL ? VK_POLYGON_MODE_FILL : polygonMode == PolygonMode::LINE ? VK_POLYGON_MODE_LINE : VK_POLYGON_MODE_POINT);
    rasterizer.lineWidth = lineWidth;
    rasterizer.cullMode = (cullMode == CullMode::BACK ? VK_CULL_MODE_BACK_BIT : cullMode == CullMode::FRONT ? VK_CULL_MODE_FRONT_BIT : VK_CULL_MODE_NONE);
    rasterizer.frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE;
    rasterizer.depthBiasEnable = VK_FALSE;

    VkPipelineMultisampleStateCreateInfo multisampling{};
    multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisampling.sampleShadingEnable = VK_FALSE;
    multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;

    VkPipelineDepthStencilStateCreateInfo depthStencil{};
    depthStencil.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
    depthStencil.depthTestEnable = (depthTest ? VK_TRUE : VK_FALSE);
    depthStencil.depthWriteEnable = VK_TRUE;
    depthStencil.depthCompareOp = VK_COMPARE_OP_LESS;
    depthStencil.depthBoundsTestEnable = VK_FALSE;
    depthStencil.stencilTestEnable = VK_FALSE;

    VkPipelineColorBlendAttachmentState colorBlendAttachment{};
    colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    colorBlendAttachment.blendEnable = (alphaChannel ? VK_TRUE : VK_FALSE);

    if (alphaChannel) {
        colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
        colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
        colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
        colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;
    }

    VkPipelineColorBlendStateCreateInfo colorBlending{};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &colorBlendAttachment;
    colorBlending.blendConstants[0] = 0.0f;
    colorBlending.blendConstants[1] = 0.0f;
    colorBlending.blendConstants[2] = 0.0f;
    colorBlending.blendConstants[3] = 0.0f;

    std::vector<VkDynamicState> dynamicStates = {
            VK_DYNAMIC_STATE_VIEWPORT,
            VK_DYNAMIC_STATE_SCISSOR
    };

    if (lineWidth != 1.0) {
        dynamicStates.push_back(VK_DYNAMIC_STATE_LINE_WIDTH);
    }

    VkPipelineDynamicStateCreateInfo dynamicState{};
    dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
    dynamicState.pDynamicStates = dynamicStates.data();

    std::vector<VkDescriptorSetLayout> lays;

    for (Descriptor* descriptor : descriptors) {
        lays.push_back(descriptor->getDescriptorSetLayout());
    }

    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = lays.size();
    pipelineLayoutInfo.pSetLayouts = lays.data();

    if (pushConstantsEnabled) {
        VkPushConstantRange pushConsts;
        pushConsts.offset = 0;
        pushConsts.size = pushConstantsSize;
        pushConsts.stageFlags = getVulkanStage(pushConstantsShader);
        pipelineLayoutInfo.pPushConstantRanges = &pushConsts;
        pipelineLayoutInfo.pushConstantRangeCount = 1;
    }

    if (vkCreatePipelineLayout(d->getDevice(), &pipelineLayoutInfo, nullptr, &layout) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create pipeline layout!");
    }

    VkGraphicsPipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.stageCount = stages.size();
    pipelineInfo.pStages = stages.data();
    pipelineInfo.pVertexInputState = &vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &rasterizer;
    pipelineInfo.pMultisampleState = &multisampling;
    pipelineInfo.pDepthStencilState = &depthStencil;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDynamicState = &dynamicState;
    pipelineInfo.layout = layout;
    pipelineInfo.renderPass = renderPass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;

    if (disableAtt) {
        switch (attachmentToDisable) {
            case Attachment::COLOR:
                pipelineInfo.pColorBlendState = nullptr;
                break;

            case Attachment::DEPTH:
                pipelineInfo.pDepthStencilState = nullptr;
                break;
        }
    }

    if (vkCreateGraphicsPipelines(d->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create graphics pipeline!");
    }
}

void VKFS::Pipeline::enablePushConstants(size_t sizeOf, ShaderType shader) {
    pushConstantsEnabled = true;
    this->pushConstantsSize = sizeOf;
    pushConstantsShader = shader;
}

void VKFS::Pipeline::enableDepthTest(bool state) {
    depthTest = state;
}

void VKFS::Pipeline::setCullMode(VKFS::CullMode mode) {
    cullMode = mode;
}

void VKFS::Pipeline::setLineWidth(float width) {
    this->lineWidth = width;
}

void VKFS::Pipeline::disableAttachment(VKFS::Attachment att) {
    disableAtt = true;
    attachmentToDisable = att;
}

void VKFS::Pipeline::setPolygonMode(VKFS::PolygonMode mode) {
    polygonMode = mode;
}

void VKFS::Pipeline::enableAlphaChannel(bool state) {
    alphaChannel = state;
}

