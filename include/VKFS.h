
#ifndef VKFS_VKFS_H
#define VKFS_VKFS_H

#include "CommandBuffer.h"
#include "Device.h"
#include "Descriptor.h"
#include "Instance.h"
#include "ShaderModule.h"
#include "Swapchain.h"
#include "Synchronization.h"
#include "VertexBuffer.h"

namespace VKFS {

    uint32_t prepareFrame(Synchronization* sync);
    void begin(Synchronization* sync);
    void end(Synchronization* sync, uint32_t imageIndex);
}

#endif //VKFS_VKFS_H
