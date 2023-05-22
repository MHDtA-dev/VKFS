

#ifndef VKFS_SYNCHRONIZATION_H
#define VKFS_SYNCHRONIZATION_H

#include "Device.h"
#include "CommandBuffer.h"
#include "Swapchain.h"

namespace VKFS {

    class Synchronization {
        public:
            Synchronization(Device* device, CommandBuffer* cmd, Swapchain* swapchain);

            void waitForFences();
            uint32_t acquireNextImage();
            uint32_t getCurrentFrame();

            VkCommandBuffer getCommandBuffer();

            void resetAll();
            void submit(uint32_t imageIndex);
            void beginRecordingCommands();
            void endRecordingCommands();

        private:
            Device* device;
            CommandBuffer* cmd;
            Swapchain* swapchain;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;
            uint32_t currentFrame = 0;
    };

}

#endif //VKFS_SYNCHRONIZATION_H
