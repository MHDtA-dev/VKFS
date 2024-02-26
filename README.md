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
to get rid of the confusing boilerplate code and those who do not need a lot of settings.

Example:
```cpp
   auto pipeline = new VKFS::Pipeline(device, [vertexBindingDescription: VkVertexInputBindingDescription], [attributesDescription: std::vector<VkVertexInputAttributeDescription>],
      [renderPass: VkRenderPass], [descriptors: std::vector<VKFS::Descriptor*>], [colorAttachmentCount = 1: int]);

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

### Offscreen renderer
An object representing simple implementation of the offscreen renderer. Supports
multiple color attachments(for example, if you need select bright fragment for bloom effect
via two outputs of the fragment shader). You can also use it for shadow mapping(set
color attachment count to 0 and enableDepthAttachment to true).

Example:
```cpp
auto offscreenRenderer = VKFS::Offscreen(device, [synchronization: VKFS::Synchronization*], [colorAttachmentsCount: int], [enableDepthAttachment: bool], [width: int], [height: int], [imageFilter = OFFSCR_LINEAR: VKFS::OffscreenImageFilter]);
```

To begin renderpass just type:
```cpp
offscreenRenderer->beginRenderpass([clearColorR = 0: int], [clearColorG = 0: int], [clearColorB = 0: int], [clearColorA = 0: int]);

... Your draw calls

offscreenRenderer->endRenderpass();
```

To get VkDescriptorImageInfo of specified attachment, use:
```cpp
offscreenRenderer->getImageInfo([attachmentIndex = 0: int]);
```

If there no color attachments but depth attachment enabled, it returns descriptor image info to the depth attachment

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

### Image
This object allows you to quickly upload image to Vulkan and use it in the future

Example:
```cpp
   auto image = new VKFS::Image(device, [imageWidth: int], [imageHeight: int], [pixelsRGBA32: void*], [generateMipMaps = true: bool], [imageFilter = VKFS::Linear: VKFS::ImageFilter]);

   image->getDescriptorImageInfo(); // Returns VkDescriptorImageInfo of created image
   image->getImage(); // Returns VkImage
   image->getImageView(); // Returns VkImageView
   image->getSampler(); // Returns VkSampler
   image->getMipLevels(); // Returns mipmap levels as uint32_t

```

You can also create descriptor for image:

```cpp
   auto imageDescriptor = VKFS::Descriptor(device, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
   imageDescriptor->createSamplerSet(image->getDescriptorImageInfo());

   // To get VkDescriptorSet for current frame use
   VkDescriptorSet setForShader = imageDescriptor->getSet(sync);

```

Example of uploading image using `SDL_image.h` module:

```cpp

    SDL_Surface* image = IMG_Load("path/to/image.jpg");
    image = SDL_ConvertSurfaceFormat(image, SDL_PIXELFORMAT_RGBA32, 0); // Strictly use RGBA32 format
    void* pixels = image->pixels;
    int width = image->w;
    int height = image->h;

   auto vulkanImage = new VKFS::Image(device, width, height, pixels);
   auto imageDescriptor = VKFS::Descriptor(device, VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT);
   imageDescriptor->createSamplerSet(vulkanImage->getDescriptorImageInfo());

```

## Extensions:

Extensions are an additional module to the main functionality of the framework. They can be removed from the project 
without causing any errors. Include extensions: `#include <VKFS_Extensions.h>`

### VKFS_EXT_SHAPE_CONSTRUCTOR:
Allows you to quickly create vertices and indexes to them of shapes such as sphere, cube, pyramid, cylinder and cone. At the moment, this extension is still in development

## General Info:

### Tested on
|Platform|Status |
|-------|:-: |
|Windows(MSYS2 MinGW64)| ✔️ |
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
- [x] Offscreen Renderer class
- [x] Images
- [ ] Screen post processing rect class
- [ ] Objects cleanup

### EXTENSIONS TODO
- [ ] Shape Constructor
- [ ] Text renderer class
- [ ] Model importing via assimp
