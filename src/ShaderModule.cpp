

#include "../include/ShaderModule.h"

VKFS::ShaderModule::ShaderModule(VKFS::Device *device, std::string path) : device(device) {
    auto code = readFile(path);
    shader = createShaderModule(code);
}

std::vector<char> VKFS::ShaderModule::readFile(const std::string &filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);

    if (!file.is_open()) {
        throw std::runtime_error("[VKFS] Failed to open file!");
    }

    size_t fileSize = (size_t) file.tellg();
    std::vector<char> buffer(fileSize);

    file.seekg(0);
    file.read(buffer.data(), fileSize);

    file.close();

    return buffer;
}

VkShaderModule VKFS::ShaderModule::createShaderModule(const std::vector<char> &code) {
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.codeSize = code.size();
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;
    if (vkCreateShaderModule(device->getDevice(), &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        throw std::runtime_error("[VKFS] Failed to create shader module!");
    }

    return shaderModule;
}

VkShaderModule VKFS::ShaderModule::getShaderModule() {
    return this->shader;
}
