

#ifndef VKFS_SHADERMODULE_H
#define VKFS_SHADERMODULE_H

#include "Device.h"
#include <fstream>

namespace VKFS {

    class ShaderModule {
        public:
            ShaderModule(Device* device, std::string path);
            VkShaderModule getShaderModule();

        private:
            Device* device;
            VkShaderModule shader;
            static std::vector<char> readFile(const std::string& filename);
            VkShaderModule createShaderModule(const std::vector<char>& code);
    };

}

#endif //VKFS_SHADERMODULE_H
