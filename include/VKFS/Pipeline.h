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

#ifndef VKFS_PIPELINE_H
#define VKFS_PIPELINE_H

#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

#include "Device.h"
#include "ShaderModule.h"
#include "Descriptor.h"


namespace VKFS {

    enum ShaderType {
        VERTEX, FRAGMENT, GEOMETRY
    };

    enum CullMode {
        FRONT, BACK, NONE
    };

    enum Attachment {
        COLOR, DEPTH
    };

    enum PolygonMode {
        FILL, LINE, POINT
    };

    class Pipeline {
        public:
            Pipeline(Device* device, VkVertexInputBindingDescription bindingDescription, std::vector<VkVertexInputAttributeDescription> attribDescription, VkRenderPass renderpass, std::vector<Descriptor*> descriptors);
            void addShader(ShaderType type, ShaderModule* shader, std::string funcname = "main");
            void enablePushConstants(size_t sizeOf, ShaderType shader);
            void enableDepthTest(bool state);
            void setCullMode(CullMode mode);
            void setLineWidth(float width);
            void disableAttachment(Attachment att);
            void setPolygonMode(PolygonMode mode);
            void enableAlphaChannel(bool state);
            void build();

            VkPipeline getPipeline();
            VkPipelineLayout getPipelineLayout();

        private:
            Device* d;
            VkVertexInputBindingDescription bindingDesc;
            std::vector<VkPipelineShaderStageCreateInfo> stages;
            std::vector<VkVertexInputAttributeDescription> attributes;
            VkVertexInputBindingDescription bindings;
            static VkShaderStageFlagBits getVulkanStage(ShaderType type);

            VkPipeline pipeline;
            VkPipelineLayout layout;

            VkRenderPass renderPass;

            bool pushConstantsEnabled = false;
            size_t pushConstantsSize;
            ShaderType pushConstantsShader;

            std::vector<Descriptor*> descriptors;

            bool depthTest = true;
            CullMode cullMode = CullMode::NONE;
            float lineWidth = 1.f;

            bool disableAtt = false;
            Attachment attachmentToDisable;

            PolygonMode polygonMode = PolygonMode::FILL;

            bool alphaChannel = false;
    };

}

#endif //VKFS_PIPELINE_H
