//
// Created by Alexander Blinov on 11.11.2023.
//

#ifndef VKFS_COMPUTEPIPELINE_H
#define VKFS_COMPUTEPIPELINE_H

#include <vulkan/vulkan.h>
#include "Device.h"
#include "ShaderModule.h"
#include "Descriptor.h"
#include "__utils.h"

namespace VKFS {

    class ComputePipeline {
        public:
            ComputePipeline(VKFS::Device* device, VKFS::ShaderModule* computeShader, std::vector<VKFS::Descriptor*> descriptors);

            void dispatch(VKFS::Synchronization* sync, int workingGroupCountX, int workingGroupCountY, int workingGroupCountZ);

        private:
            VKFS::Device* device;

            VkPipeline pipeline;
            VkPipelineLayout pipelineLayout;

            std::vector<VKFS::Descriptor*> descriptors;

    };

}


#endif //VKFS_COMPUTEPIPELINE_H
