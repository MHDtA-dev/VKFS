//
// Created by Alexander Blinov on 24.08.2023.
//

#include "../include/VKFS/Offscreen.h"

VKFS::Offscreen::Offscreen(VKFS::Device *device, VKFS::Synchronization* sync, int colorAttachmentsCount, bool enableDepthAttachment, int width, int height, OffscreenImageFilter imageFilter) : d(device), w(width), h(height), filter(imageFilter), sync(sync) {
    this->colorAttachmentsCount = colorAttachmentsCount;
    this->enableDepthAttachment = enableDepthAttachment;

    size.width = width;
    size.height = height;

    VkFormat fbDepthFormat = d->findDepthFormat();

    for (int i = 0; i < colorAttachmentsCount; i++) {
        colorImages.push_back(createColorImage());

        VkAttachmentDescription att;
        att.format = VK_FORMAT_R8G8B8A8_UNORM;
        att.samples = VK_SAMPLE_COUNT_1_BIT;
        att.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        att.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
        att.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        att.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        att.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        att.finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        att.flags = 0;

        attachments.push_back(att);
    }

    if (enableDepthAttachment) {
        createDepthImage();

        VkAttachmentDescription datt;
        datt.format = fbDepthFormat;
        datt.samples = VK_SAMPLE_COUNT_1_BIT;
        datt.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
        datt.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        datt.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
        datt.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
        datt.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
        datt.finalLayout = (colorAttachmentsCount >= 1 ? VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
                                                       : VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
        datt.flags = 0;

        attachments.push_back(datt);
    }


    createSampler();

    std::vector<VkAttachmentReference> colorAttachmentReferences;

    for (int i = 0; i < colorAttachmentsCount; i++) {
        VkAttachmentReference colorReference = { static_cast<uint32_t>(i), VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
        colorAttachmentReferences.push_back(colorReference);
    }

    VkAttachmentReference depthReference = { static_cast<uint32_t>(colorAttachmentReferences.size()), VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };




    VkSubpassDescription subpassDescription = {};
    subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpassDescription.colorAttachmentCount = colorAttachmentsCount;
    subpassDescription.pColorAttachments = (colorAttachmentsCount >= 1) ? colorAttachmentReferences.data() : nullptr;
    subpassDescription.pDepthStencilAttachment = (enableDepthAttachment) ? &depthReference : nullptr;


    std::array<VkSubpassDependency, 2> dependencies;

    dependencies[0].srcSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[0].dstSubpass = 0;
    dependencies[0].srcStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[0].dstStageMask = (colorAttachmentsCount >= 1 || (colorAttachmentsCount >= 1 && enableDepthAttachment)) ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT : VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    dependencies[0].srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[0].dstAccessMask = (colorAttachmentsCount >= 1 || (colorAttachmentsCount >= 1 && enableDepthAttachment)) ? VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT : VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[0].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    dependencies[1].srcSubpass = 0;
    dependencies[1].dstSubpass = VK_SUBPASS_EXTERNAL;
    dependencies[1].srcStageMask = (colorAttachmentsCount >= 1 || (colorAttachmentsCount >= 1 && enableDepthAttachment)) ? VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT : VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    dependencies[1].dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    dependencies[1].srcAccessMask = (colorAttachmentsCount >= 1 || (colorAttachmentsCount >= 1 && enableDepthAttachment)) ? VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT : VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    dependencies[1].dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
    dependencies[1].dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

    VkRenderPassCreateInfo renderPassInfo = {};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    renderPassInfo.attachmentCount = static_cast<uint32_t>(attachments.size());
    renderPassInfo.pAttachments = attachments.data();
    renderPassInfo.subpassCount = 1;
    renderPassInfo.pSubpasses = &subpassDescription;
    renderPassInfo.dependencyCount = static_cast<uint32_t>(dependencies.size());
    renderPassInfo.pDependencies = dependencies.data();

    vkCreateRenderPass(d->getDevice(), &renderPassInfo, nullptr, &renderPass);

    clearQueue.push_function([=] () {
        vkDestroyRenderPass(d->getDevice(), renderPass, nullptr);
    });

    std::vector<VkImageView> fbAttachments;

    for (__OffscreenImage img : colorImages) {
        fbAttachments.push_back(img.imageView);
    }

    if (enableDepthAttachment || (enableDepthAttachment && colorAttachmentsCount >= 1)) fbAttachments.push_back(depthImageView);

    VkFramebufferCreateInfo fbufCreateInfo = {};
    fbufCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fbufCreateInfo.renderPass = renderPass;
    fbufCreateInfo.attachmentCount = fbAttachments.size();
    fbufCreateInfo.pAttachments = fbAttachments.data();
    fbufCreateInfo.width = width;
    fbufCreateInfo.height = height;
    fbufCreateInfo.layers = 1;

    vkCreateFramebuffer(d->getDevice(), &fbufCreateInfo, nullptr, &framebuffer);

    clearQueue.push_function([=] () {
        vkDestroyFramebuffer(d->getDevice(), framebuffer, nullptr);
    });

    for (__OffscreenImage &img : colorImages) {
        img.imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        img.imageInfo.imageView = (colorAttachmentsCount >= 1 || (enableDepthAttachment && colorAttachmentsCount >= 1)) ? img.imageView : depthImageView;
        img.imageInfo.sampler = sampler;
    }

    depthImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    depthImageInfo.imageView = depthImageView;
    depthImageInfo.sampler = sampler;

}

VKFS::__OffscreenImage VKFS::Offscreen::createColorImage() {
    __OffscreenImage ret;

    VkImageCreateInfo image = {};
    image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = VK_FORMAT_R8G8B8A8_UNORM;
    image.extent.width = w;
    image.extent.height = h;
    image.extent.depth = 1;
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    image.usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_SAMPLED_BIT;

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs;

    vkCreateImage(d->getDevice(), &image, nullptr, &ret.image);
    vkGetImageMemoryRequirements(d->getDevice(), ret.image, &memReqs);
    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = d->findMemoryType(memReqs.memoryTypeBits,
                                                      VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(d->getDevice(), &memAlloc, nullptr, &ret.imageMemory);
    vkBindImageMemory(d->getDevice(), ret.image, ret.imageMemory, 0);

    clearQueue.push_function([=] () {
        vkDestroyImage(d->getDevice(), ret.image, nullptr);
        vkFreeMemory(d->getDevice(), ret.imageMemory, nullptr);
    });

    VkImageViewCreateInfo colorImageViewInfo = {};
    colorImageViewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    colorImageViewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    colorImageViewInfo.format = VK_FORMAT_R8G8B8A8_UNORM;
    colorImageViewInfo.subresourceRange = {};
    colorImageViewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    colorImageViewInfo.subresourceRange.baseMipLevel = 0;
    colorImageViewInfo.subresourceRange.levelCount = 1;
    colorImageViewInfo.subresourceRange.baseArrayLayer = 0;
    colorImageViewInfo.subresourceRange.layerCount = 1;
    colorImageViewInfo.image = ret.image;
    vkCreateImageView(d->getDevice(), &colorImageViewInfo, nullptr, &ret.imageView);

    clearQueue.push_function([=] () {
        vkDestroyImageView(d->getDevice(), ret.imageView, nullptr);
    });

    return ret;
}

void VKFS::Offscreen::createDepthImage() {
    VkFormat fbDepthFormat = d->findDepthFormat();

    VkImageCreateInfo image = {};
    image.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
    image.imageType = VK_IMAGE_TYPE_2D;
    image.format = fbDepthFormat;
    image.extent.width = w;
    image.extent.height = h;
    image.extent.depth = 1;
    image.mipLevels = 1;
    image.arrayLayers = 1;
    image.samples = VK_SAMPLE_COUNT_1_BIT;
    image.tiling = VK_IMAGE_TILING_OPTIMAL;
    image.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;

    if (enableDepthAttachment && colorAttachmentsCount == 0) {
        image.usage |= VK_IMAGE_USAGE_SAMPLED_BIT;
    }

    vkCreateImage(d->getDevice(), &image, nullptr, &depthImage);

    VkMemoryAllocateInfo memAlloc = {};
    memAlloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    VkMemoryRequirements memReqs;

    vkGetImageMemoryRequirements(d->getDevice(), depthImage, &memReqs);

    memAlloc.allocationSize = memReqs.size;
    memAlloc.memoryTypeIndex = d->findMemoryType(memReqs.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    vkAllocateMemory(d->getDevice(), &memAlloc, nullptr, &depthImageMemory);
    vkBindImageMemory(d->getDevice(), depthImage, depthImageMemory, 0);

    clearQueue.push_function([=] () {
        vkDestroyImage(d->getDevice(), depthImage, nullptr);
        vkFreeMemory(d->getDevice(), depthImageMemory, nullptr);
    });

    VkImageViewCreateInfo depthStencilView = {};
    depthStencilView.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    depthStencilView.viewType = VK_IMAGE_VIEW_TYPE_2D;
    depthStencilView.format = fbDepthFormat;
    depthStencilView.flags = 0;
    depthStencilView.subresourceRange = {};
    depthStencilView.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
    if (fbDepthFormat >= VK_FORMAT_D16_UNORM_S8_UINT) {
        depthStencilView.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
    }
    depthStencilView.subresourceRange.baseMipLevel = 0;
    depthStencilView.subresourceRange.levelCount = 1;
    depthStencilView.subresourceRange.baseArrayLayer = 0;
    depthStencilView.subresourceRange.layerCount = 1;
    depthStencilView.image = depthImage;
    vkCreateImageView(d->getDevice(), &depthStencilView, nullptr, &depthImageView);

    clearQueue.push_function([=] () {
        vkDestroyImageView(d->getDevice(), depthImageView, nullptr);
    });

}

void VKFS::Offscreen::createSampler() {
    VkSamplerCreateInfo samplerInfo = {};
    samplerInfo.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
    samplerInfo.magFilter = (filter == OffscreenImageFilter::OFFSCR_LINEAR) ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    samplerInfo.minFilter = (filter == OffscreenImageFilter::OFFSCR_LINEAR) ? VK_FILTER_LINEAR : VK_FILTER_NEAREST;
    samplerInfo.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
    samplerInfo.addressModeU = VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE;
    samplerInfo.addressModeV = samplerInfo.addressModeU;
    samplerInfo.addressModeW = samplerInfo.addressModeU;
    samplerInfo.mipLodBias = 0.0f;
    samplerInfo.maxAnisotropy = 1.0f;
    samplerInfo.minLod = 0.0f;
    samplerInfo.maxLod = 1.0f;
    samplerInfo.borderColor = VK_BORDER_COLOR_FLOAT_OPAQUE_WHITE;
    vkCreateSampler(d->getDevice(), &samplerInfo, nullptr, &sampler);

    clearQueue.push_function([=] () {
        vkDestroySampler(d->getDevice(), sampler, nullptr);
    });
}

VkRenderPass VKFS::Offscreen::getRenderPass() {
    return renderPass;
}

VkExtent2D VKFS::Offscreen::getExtent() {
    return size;
}

VkFramebuffer VKFS::Offscreen::getFramebuffer() {
    return framebuffer;
}

VkSampler VKFS::Offscreen::getSampler() {
    return sampler;
}

VkDescriptorImageInfo VKFS::Offscreen::getImageInfo(int attachmentIndex) {
    return colorImages.size() >= 1 ? colorImages[attachmentIndex].imageInfo : depthImageInfo;
}

void VKFS::Offscreen::beginRenderpass(float clearR, float clearG, float clearB, float clearA) {
    VkRenderPassBeginInfo renderPassInfo{};
    renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
    renderPassInfo.renderPass = renderPass;
    renderPassInfo.framebuffer = framebuffer;
    renderPassInfo.renderArea.offset = {0, 0};
    renderPassInfo.renderArea.extent = size;

    std::vector<VkClearValue> clearValues{};

    for (int i = 0; i < colorAttachmentsCount; i++) {
        if (i == 0) {
            VkClearValue c1;
            c1.color = {{clearR, clearG, clearB, clearA}};
            clearValues.push_back(c1);
        } else {
            VkClearValue c1;
            c1.color = {{0, 0, 0, 1}};
            clearValues.push_back(c1);
        }
    }

    if (enableDepthAttachment) {
        VkClearValue c2;
        c2.depthStencil = {1.0f, 0};
        clearValues.push_back(c2);
    }

    renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
    renderPassInfo.pClearValues = clearValues.data();
    vkCmdBeginRenderPass(sync->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

}

void VKFS::Offscreen::endRenderpass() {
    vkCmdEndRenderPass(sync->getCommandBuffer());
}

VKFS::Offscreen::~Offscreen() {
    clearQueue.flush();
}
