# VKFS - Vulkan Fast Setup Library

![Logo](logo.PNG)

## VKFS is a cross-platform C++ library that allows you to quickly create basic Vulkan objects for further work, such as Instance, Physical Device, Device, Command queues, Command buffer and synchronization objects.

Note: The library is currently under development and may contain bugs or some missing functionality.

## Objects:

### Instance:
The object that creates an Instance of your application.

Example:
`auto instance = new VKFS::Instance("Application Name", "Engine name", [std::vector<const char*>: instanceExtensions], [bool: enableValidationLayers], [OPTIONAL uint32_t API_VERSION=VK_API_VERSION_1_2]);`

### Device:
The object that picks up VkPhysicalDevice, creates VkDevice based on it, as well as command queues.

Example:
`auto device = new VKFS::Device(instance, [std::vector<const char*>: deviceExtensions]);`

### Swapchain
An object that creates a swap chain and renderpass for it.

Example:
`auto swapchain = new VKFS::Swapchain(device, [int: windowWidth], [int: windowHeight]);`

### Descriptor
The object that creates VkDescriptorSetLayout, VkDescriptorSet and everything necessary for this. Allows you to create a Descriptor for UBO, Sampler or Storage Buffer in just two lines.

Example:
```cpp
   auto descriptor = new VKFS::Descriptor(device, [VkDescriptorType: descriptorType], [VkShaderStageFlagBits: shaderStage]);
   descriptor->createUBOSet(sizeof(YourUBOStructure));
```
