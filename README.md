# VKFS - Vulkan Fast Setup Framework

![Logo](logo.PNG)

## VKFS is a cross-platform and window system independ C++ framework that allows you to quickly create basic Vulkan objects for further work, such as Instance, Physical Device, Device, Command queues, Command buffer and synchronization objects.

#### The framework is fully working. If you notice a bug/missing functionality, write about it in issues tab. I will add new features as needed.

## Tutorials:
- ["Hello Triangle" app](tutorials/VKFS-Hello-Triangle-Tutorial.md)
- [Using ImGui with VKFS](tutorials/VKFS-Imgui-Tutorial.md)

#### Note: These tutorials assume that you already have a working environment with a configured compiler and linked libraries.

## Brief description of objects:

### Instance:
The object that creates an Instance of your application.

Example:
```cpp
   auto instance = new VKFS::Instance("Application Name", "Engine name", [instanceExtensions: std::vector<const char*>], [enableValidationLayers: bool], [OPTIONAL API_VERSION=VK_API_VERSION_1_2: uint32_t]);
```

### Device:
The object that picks up VkPhysicalDevice, creates VkDevice based on it, as well as command queues.

Example:
```cpp
   auto device = new VKFS::Device(instance, [deviceExtensions: std::vector<const char*>]);
```

### Swapchain
An object that creates a swap chain and renderpass for it.

Example:
```cpp
   auto swapchain = new VKFS::Swapchain(device, [windowWidth: int], [windowHeight: int]);
```

### Descriptor
The object that creates VkDescriptorSetLayout, VkDescriptorSet and everything necessary for this. Allows you to create a Descriptor for UBO, Sampler or Storage Buffer in just two lines.

Example:
```cpp
   auto descriptor = new VKFS::Descriptor(device, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, [shaderStage: VkShaderStageFlagBits]);
   descriptor->createUBOSet(sizeof(YourUBOStructure));
```
OR

```cpp
   auto descriptor = new VKFS::Descriptor(device, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, [shaderStage: VkShaderStageFlagBits]);
   descriptor->createSamplerSet([imageInfo: VkDescriptorImageInfo]);
```

### Shader Module
An object that allows you to conveniently load and create a VkShaderModule

Example:
```cpp
   auto vertex = new VKFS::ShaderModule(device, "path/to/spv");
```

### Pipeline
A simple abstraction over ```VkPipeline``` and ```VkPipelineLayout``` objects. Perfect for those who want 
to get rid of the confusing boilerplate code and those who do not need a lot of settings

Example:
```cpp
   auto pipeline = new VKFS::Pipeline(device, [vertexBindingDescription: VkVertexInputBindingDescription], [attributesDescription: std::vector<VkVertexInputAttributeDescription>],
      [renderPass: VkRenderPass], [descriptors: std::vector<VKFS::Descriptor*>]);

   pipeline->addShader(VKFS::VERTEX, vertex);
   pipeline->addShader(VKFS::FRAGMENT, fragment);
   ...
   pipeline->addShader([type: VKFS::ShaderType], [vertexShader: VKFS::ShaderModule*]); // You can add vertex, fragment and geometry shaders to pipeline

   pipeline->build(); // Build the pipeline
```

After building you can get your ready to use objects:
```cpp
   pipeline->getPipelineLayout(); // Returns VkPipelineLayout
   pipeline->getPipeline(); // Returns VkPipeline
```

You can also set some additional parameters(before ```pipeline->build();```):
```cpp
   pipeline->enablePushConstants(sizeof(YourPushConstantsStruct), [shaderType: VKFS::ShaderType]); // Enables push constants for your pipeline.
   pipeline->enableDepthTest([state: bool]); // Enables or disables depth test. Enabled by default
   pipeline->setCullMode([mode: VKFS::CullMode]); // Enables specified culling mode
   pipeline->disableAttachment([attachment: VKFS::Attachment]); // Disables color or depth attachment. For example, you can disable color attachment if you need pipeline for shadow mapping
   pipeline->enableAlphaChannel([state: bool]); // Enables or disables alpha blending
   pipeline->setPolygonMode([mode: VKFS::PolygonMode]); // Changes polygon mode. VKFS::FILL by default
```


### Vertex Buffer
This object allows you to create a buffer of vertices and indices using your vertex structure

Example:
```cpp
   auto vb = new VKFS::VertexBuffer<YourVertexStruct>(device, [vertices: std::vector<YourVertexStruct>], [indices: std::vector<uint32_t>]);
```

You can also use this with Push Constants:
```cpp
   auto vb = new VKFS::VertexBuffer<YourVertexStruct, YourPushConstantsStruct>(device, [vertices: std::vector<YourVertexStruct>], [indices: std::vector<uint32_t>]);
```
Next, before you draw, call
```cpp
   vb->pushPushConstants(yourPushConstants);
```



### Tested on
|Platform|Status |
|-------|:-: |
|Windows(MSYS2)| ✔️ |
|MacOS  | ✔️ |
|Linux(Debian 11)  | ✔️ |


### Building
To build a framework, use CMake:

```
git clone https://github.com/MHDtA-dev/VKFS.git
cd VKFS
mkdir build
cd build
cmake -DVULKAN_INCLUDE_PATH=[Vulkan SDK include path] ..
make
```

### TODO
- [ ] "Context" class
- [ ] Offscreen Renderer class
- [ ] Screen post processing rect class
- [ ] Objects cleanup

### EXTENSIONS TODO
- [ ] Text renderer class
- [ ] Model importing via assimp
