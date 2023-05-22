

#ifndef VKFS_INSTANCE_H
#define VKFS_INSTANCE_H

#include <iostream>
#include <vector>
#include <vulkan/vulkan.h>

namespace VKFS {

    class Instance {
        public:
            Instance(std::string appName, std::string engineName, std::vector<const char*> instanceExtensions, bool enableValidationLayers, uint32_t APIVersion = VK_API_VERSION_1_2);

            void setSurface(VkSurfaceKHR surface);
            VkSurfaceKHR getSurface();

            VkInstance getNative();
            bool isUseDebug();

            std::vector<const char*> validationLayers = {
                    "VK_LAYER_KHRONOS_validation"
            };

        private:
            VkInstance instance;
            VkDebugUtilsMessengerEXT debugMessenger;
            VkSurfaceKHR surface;
            bool useDebug = false;

            bool checkValidationLayerSupport();
            void populateDebugMessengerCreateInfo(VkDebugUtilsMessengerCreateInfoEXT& createInfo);
            void setupDebugMessenger();
            static VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData);
        VkResult CreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger);


    };


}

#endif //VKFS_INSTANCE_H
