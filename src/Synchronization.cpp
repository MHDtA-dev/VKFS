#include "../include/VKFS/Synchronization.h"

VKFS::Synchronization::Synchronization(VKFS::Device *device, VKFS::CommandBuffer *cmd, Swapchain* swapchain) : device(device), cmd(cmd), swapchain(swapchain) {
    imageAvailableSemaphores.resize(2);
    renderFinishedSemaphores.resize(2);
    inFlightFences.resize(2);

    VkSemaphoreCreateInfo semaphoreInfo{};
    semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    VkFenceCreateInfo fenceInfo{};
    fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

    for (size_t i = 0; i < 2; i++) {
        if (vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &imageAvailableSemaphores[i]) != VK_SUCCESS ||
            vkCreateSemaphore(device->getDevice(), &semaphoreInfo, nullptr, &renderFinishedSemaphores[i]) != VK_SUCCESS ||
            vkCreateFence(device->getDevice(), &fenceInfo, nullptr, &inFlightFences[i]) != VK_SUCCESS) {
            throw std::runtime_error("[VKFS] Failed to create synchronization objects for a frame!");
        }
    }
}

void VKFS::Synchronization::waitForFences() {
    vkWaitForFences(device->getDevice(), 1, &inFlightFences[currentFrame], VK_TRUE, UINT64_MAX);
}

uint32_t VKFS::Synchronization::acquireNextImage() {
    uint32_t imageIndex;
    VkResult result = vkAcquireNextImageKHR(device->getDevice(), swapchain->getSwapchain(), UINT64_MAX, imageAvailableSemaphores[currentFrame], VK_NULL_HANDLE, &imageIndex);

    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        swapchain->recreate(windowWidth, windowHeight);
        return -1;
    } else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        throw std::runtime_error("[VKFS] Failed to acquire swapchain image!");
    }

    return imageIndex;
}

VkCommandBuffer VKFS::Synchronization::getCommandBuffer() {
    return cmd->commandBuffers[currentFrame];
}

void VKFS::Synchronization::resetAll() {
    vkResetFences(device->getDevice(), 1, &inFlightFences[currentFrame]);
    vkResetCommandBuffer(cmd->commandBuffers[currentFrame], 0);
}

void VKFS::Synchronization::submit(uint32_t imageIndex) {

    if (windowWidth == -1 or windowHeight == -1) {
        throw std::runtime_error("[VKFS] The window size must be passed to the Sync object using the pushWindowSize() method every frame!");
    }

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

    VkSemaphore waitSemaphores[] = {imageAvailableSemaphores[currentFrame]};
    VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
    submitInfo.waitSemaphoreCount = 1;
    submitInfo.pWaitSemaphores = waitSemaphores;
    submitInfo.pWaitDstStageMask = waitStages;

    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &cmd->commandBuffers[currentFrame];

    VkSemaphore signalSemaphores[] = {renderFinishedSemaphores[currentFrame]};
    submitInfo.signalSemaphoreCount = 1;
    submitInfo.pSignalSemaphores = signalSemaphores;

    if (vkQueueSubmit(device->getGraphicsQueue(), 1, &submitInfo, inFlightFences[currentFrame]) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to submit draw command buffer!");
    }

    VkPresentInfoKHR presentInfo{};
    presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pWaitSemaphores = signalSemaphores;

    VkSwapchainKHR swapChains[] = {swapchain->getSwapchain()};
    presentInfo.swapchainCount = 1;
    presentInfo.pSwapchains = swapChains;

    presentInfo.pImageIndices = &imageIndex;

    VkResult result = vkQueuePresentKHR(device->getPresentQueue(), &presentInfo);

    if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
        swapchain->recreate(windowWidth, windowHeight);
    } else if (result != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to present swap chain image!");
    }

    currentFrame = (currentFrame + 1) % 2;
}

void VKFS::Synchronization::beginRecordingCommands() {
    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;

    if (vkBeginCommandBuffer(getCommandBuffer(), &beginInfo) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to begin recording command buffer!");
    }
}

void VKFS::Synchronization::endRecordingCommands() {
    if (vkEndCommandBuffer(getCommandBuffer()) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to record command buffer!");
    }
}

uint32_t VKFS::Synchronization::getCurrentFrame() {
    return this->currentFrame;
}

void VKFS::Synchronization::pushWindowSize(int width, int height) {
    this->windowWidth = width;
    this->windowHeight = height;
}
