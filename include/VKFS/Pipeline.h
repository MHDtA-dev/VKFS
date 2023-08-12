//
// Created by Alexander Blinov on 12.08.2023.
//

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
