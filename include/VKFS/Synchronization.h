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
            VkCommandBuffer getComputeCommandBuffer();

            void resetAll();
            void resetCompute();
            void waitCompute();
            void beginRecordingCompute();
            void endRecordingCompute();
            void submit(uint32_t imageIndex);
            void beginRecordingCommands();
            void endRecordingCommands();
            void submitCompute();

            void pushWindowSize(int width, int height);

        private:
            Device* device;
            CommandBuffer* cmd;
            Swapchain* swapchain;

            std::vector<VkSemaphore> imageAvailableSemaphores;
            std::vector<VkSemaphore> renderFinishedSemaphores;
            std::vector<VkFence> inFlightFences;
            uint32_t currentFrame = 0;

            std::vector<VkFence> computeInFlightFences;
            std::vector<VkSemaphore> computeFinishedSemaphores;

            int windowWidth = -1, windowHeight = -1;
            bool computeInUse = false;
    };

}

#endif //VKFS_SYNCHRONIZATION_H
