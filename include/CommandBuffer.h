

#ifndef VKFS_COMMANDBUFFER_H
#define VKFS_COMMANDBUFFER_H

#include "Device.h"

namespace VKFS {

    class CommandBuffer {
        public:
            CommandBuffer(Device* device);
            std::vector<VkCommandBuffer> commandBuffers;
        private:
            Device* device;
    };

}


#endif //VKFS_COMMANDBUFFER_H
