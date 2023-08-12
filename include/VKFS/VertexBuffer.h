
/*
(c) Copyright 2023 MHDtA

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.

   2. Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.

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

#ifndef VKFS_VERTEXBUFFER_H
#define VKFS_VERTEXBUFFER_H

#include <type_traits>
#include "Device.h"
#include "Synchronization.h"


namespace VKFS {

    template<typename Vertex, typename PushConstantsStruct = int>
    class VertexBuffer {
        public:
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;
            VertexBuffer(Device *device, std::vector<Vertex> vertices, std::vector<uint32_t> indices) : device(device) {
                this->vertices = vertices;
                this->indices = indices;

                createVertexBuffer();
                createIndexBuffer();
            }

            VkBuffer getVertexBuffer() {
                return this->vertexBuffer;
            }

            VkBuffer getIndexBuffer() {
                return this->indexBuffer;
            }

            void pushDescriptorSet(VkDescriptorSet set) {
                this->layouts.push_back(set);
            }

            void pushPushConstants(PushConstantsStruct consts, VkShaderStageFlagBits shaderStage) {
                this->pushConstants = consts;
                this->pushConstantsShaderStage = shaderStage;
            }

            void draw(Synchronization* sync, VkPipelineLayout layout, VkPipeline pipeline, VkExtent2D viewportSize) {
                vkCmdBindPipeline(sync->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

                VkViewport viewport{};
                viewport.x = 0.0f;
                viewport.y = 0.0f;
                viewport.width = (float) viewportSize.width;
                viewport.height = (float) viewportSize.height;
                viewport.minDepth = 0.0f;
                viewport.maxDepth = 1.0f;
                vkCmdSetViewport(sync->getCommandBuffer(), 0, 1, &viewport);

                VkRect2D scissor{};
                scissor.offset = {0, 0};
                scissor.extent = viewportSize;
                vkCmdSetScissor(sync->getCommandBuffer(), 0, 1, &scissor);

                VkBuffer vertexBuffers[] = {vertexBuffer};
                VkDeviceSize offsets[] = {0};
                vkCmdBindVertexBuffers(sync->getCommandBuffer(), 0, 1, vertexBuffers, offsets);

                vkCmdBindIndexBuffer(sync->getCommandBuffer(), indexBuffer, 0, VK_INDEX_TYPE_UINT32);

                for (int i = 0; i < layouts.size(); i++) {
                    vkCmdBindDescriptorSets(sync->getCommandBuffer(), VK_PIPELINE_BIND_POINT_GRAPHICS, layout, i, 1, &layouts[i], 0, nullptr);
                }

                if (!std::is_same<PushConstantsStruct, int>::value) {
                    vkCmdPushConstants(sync->getCommandBuffer(), layout, pushConstantsShaderStage, 0, sizeof(PushConstantsStruct), &pushConstants);
                }

                vkCmdDrawIndexed(sync->getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
                layouts.clear();
            }

        private:
            Device* device;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

            PushConstantsStruct pushConstants;
            VkShaderStageFlagBits pushConstantsShaderStage;

            std::vector<VkDescriptorSet> layouts;

            void createVertexBuffer() {
                VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

                VkBuffer stagingBuffer;
                VkDeviceMemory stagingBufferMemory;
                device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

                void* data;
                vkMapMemory(device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
                memcpy(data, vertices.data(), (size_t) bufferSize);
                vkUnmapMemory(device->getDevice(), stagingBufferMemory);

                device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);

                device->copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

                vkDestroyBuffer(device->getDevice(), stagingBuffer, nullptr);
                vkFreeMemory(device->getDevice(), stagingBufferMemory, nullptr);
            }

            void createIndexBuffer() {
                VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

                VkBuffer stagingBuffer;
                VkDeviceMemory stagingBufferMemory;
                device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

                void* data;
                vkMapMemory(device->getDevice(), stagingBufferMemory, 0, bufferSize, 0, &data);
                memcpy(data, indices.data(), (size_t) bufferSize);
                vkUnmapMemory(device->getDevice(), stagingBufferMemory);

                device->createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

                device->copyBuffer(stagingBuffer, indexBuffer, bufferSize);

                vkDestroyBuffer(device->getDevice(), stagingBuffer, nullptr);
                vkFreeMemory(device->getDevice(), stagingBufferMemory, nullptr);
            }

    };


}


#endif //VKFS_VERTEXBUFFER_H
