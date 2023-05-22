# VKFS - Vulkan Fast Setup Library

![Logo](logo.PNG)

## VKFS is a cross-platform library that allows you to quickly create basic Vulkan objects for further work, such as Instance, Physical Device, Device, Command queues, Command buffer and synchronization objects.

Note: The library is currently under development and may contain bugs or some missing functionality.

## Objects:

### Instance:
The object that creates an Instance of your application

Example:
`auto instance = VKFS::Instance("Application Name", "Engine name", [std::vector<const char*>: instanceExtensions], [bool: enableValidationLayers], [OPTIONAL uint32_t API_VERSION=VK_API_VERSION_1_2]`
