

#ifndef VKFS_VERTEXBUFFER_H
#define VKFS_VERTEXBUFFER_H

#include "Device.h"
#include "Synchronization.h"


namespace VKFS {

    template<typename Vertex>
    class VertexBuffer {
        public:
            VertexBuffer(Device* device, std::vector<Vertex> vertices, std::vector<uint32_t> indices);
            std::vector<Vertex> vertices;
            std::vector<uint32_t> indices;

            void pushDescriptorSet(VkDescriptorSet set);
            void draw(Synchronization* sync, VkPipelineLayout layout, VkPipeline pipeline, VkExtent2D viewportSize);



            VkBuffer getIndexBuffer();
            VkBuffer getVertexBuffer();

        private:
            Device* device;
            VkBuffer vertexBuffer;
            VkDeviceMemory vertexBufferMemory;
            VkBuffer indexBuffer;
            VkDeviceMemory indexBufferMemory;

            std::vector<VkDescriptorSet> layouts;

            void createVertexBuffer();
            void createIndexBuffer();

    };

    template<typename Vertex>
    VKFS::VertexBuffer<Vertex>::VertexBuffer(Device *device, std::vector<Vertex> vertices, std::vector<uint32_t> indices) : device(device) {
        this->vertices = vertices;
        this->indices = indices;


        createVertexBuffer();
        createIndexBuffer();
    }

    template<typename Vertex>
    void VKFS::VertexBuffer<Vertex>::createVertexBuffer() {
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

    template<typename Vertex>
    void VKFS::VertexBuffer<Vertex>::createIndexBuffer() {
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

    template<typename Vertex>
    VkBuffer VKFS::VertexBuffer<Vertex>::getVertexBuffer() {
        return this->vertexBuffer;
    }

    template<typename Vertex>
    VkBuffer VKFS::VertexBuffer<Vertex>::getIndexBuffer() {
        return this->indexBuffer;
    }



    template<typename Vertex>
    void VKFS::VertexBuffer<Vertex>::pushDescriptorSet(VkDescriptorSet set) {
        this->layouts.push_back(set);
    }

    template<typename Vertex>
    void VertexBuffer<Vertex>::draw(Synchronization* sync, VkPipelineLayout layout, VkPipeline pipeline, VkExtent2D viewportSize) {
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

        vkCmdDrawIndexed(sync->getCommandBuffer(), static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
        layouts.clear();
    }


}


#endif //VKFS_VERTEXBUFFER_H
