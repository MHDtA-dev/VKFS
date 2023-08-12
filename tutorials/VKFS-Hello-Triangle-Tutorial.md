## VKFS - Hello Triangle App tutorial
In this tutorial, we will create a simple application that will display a colored triangle on the screen.

![Triangle](images/triangle.png)

Required libraries:
- Vulkan v1.2
- VKFS v1.0
- SDL2 with Vulkan module
- GLM

### Step 1. Include all necessary libraries
```cpp
#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include <vulkan/vulkan.h>
#include <VKFS/VKFS.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
```

### Step 2. Initialize SDL2
Note: Now we are working in int main()
```cpp
    SDL_Init(SDL_INIT_VIDEO); // Init video
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN); // Set window Vulkan flag
    
    SDL_Window* window = SDL_CreateWindow(
        "VKFS - Hello Triangle",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        1920,
        1080,
        window_flags
    );
    
    bool quit = false; // App close flag
    SDL_Event event;
```

### Step 3. Get instance extensions
```cpp
    uint32_t extensionCount = 0;
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, nullptr);
    std::vector<const char *> extensions(extensionCount);

#ifdef __APPLE__ //If we are on macOS, we should use Vulkan Portability
    extensions.push_back("VK_KHR_portability_enumeration");
#endif

    extensions.push_back("VK_EXT_debug_utils");

```

### Step 4. Get device extensions
```cpp
    std::vector<const char *> deviceExts;
    deviceExts.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
    
#ifdef __APPLE__ //If we are on macOS, we should use Vulkan Portability
    deviceExts.push_back("VK_KHR_portability_subset");
#endif
    
    SDL_Vulkan_GetInstanceExtensions(window, &extensionCount, extensions.data());

```


### Step 5. Create app instance
```cpp
    auto instance = new VKFS::Instance("Hello Triangle", "No Engine", extensions, true);
```

### Step 6. Create Vulkan surface and pass it to instance
```cpp
    VkSurfaceKHR s;
    SDL_Vulkan_CreateSurface(window, instance->getNative(), &s);
    instance->setSurface(s);
```

### Step 7. Create device
```cpp
    auto device = new VKFS::Device(instance, deviceExts);
```

### Step 8. Get window size and create swap chain
```cpp
    int windowWidth, windowHeight;
    SDL_GetWindowSize(window, &windowWidth, &windowHeight);

    auto swapchain = new VKFS::Swapchain(device, windowWidth, windowHeight);
```

### Step 9. Create command buffer and synchronization object
```cpp
    auto commandBuffer = new VKFS::CommandBuffer(device);
    auto sync = new VKFS::Synchronization(device, commandBuffer, swapchain);
```

### Step 10. Write vertex and fragment shaders and compile them

#### Vertex:
```glsl
#version 460

layout(location = 0) in vec3 inPosition;
layout(location = 0) out vec3 Position;

void main() {
    gl_Position = vec4(inPosition, 1);
    Position = inPosition;
}
```

#### Fragment:
```glsl
#version 460

layout(location = 0) in vec3 Position;
layout(location = 0) out vec4 FragColor;

void main() {
    FragColor = vec4(Position, 1);
}
```

### Step 11. Load shaders
```cpp
    auto vertex = new VKFS::ShaderModule(device, "vertex.spv");
    auto fragment = new VKFS::ShaderModule(device, "fragment.spv");
```

### Step 12. Declare Vertex struct
```cpp
    struct Vertex {
        glm::vec3 pos;
    
        static VkVertexInputBindingDescription getBindingDescription() {
            VkVertexInputBindingDescription bindingDescription{};
            bindingDescription.binding = 0;
            bindingDescription.stride = sizeof(Vertex);
            bindingDescription.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
    
            return bindingDescription;
        }
    
        static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions() {
            std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
            attributeDescriptions.resize(1);
    
            attributeDescriptions[0].binding = 0;
            attributeDescriptions[0].location = 0;
            attributeDescriptions[0].format = VK_FORMAT_R32G32B32_SFLOAT;
            attributeDescriptions[0].offset = offsetof(Vertex, pos);
    
            return attributeDescriptions;
        }
    
    };
```

### Step 13. Create pipeline
```cpp
    std::vector<VKFS::Descriptor*> descriptors; // If you have descriptors, you need to list them in this vector
    auto p = new VKFS::Pipeline(device, Vertex::getBindingDescription(), Vertex::getAttributeDescriptions(), swapchain->getRenderPass(), descriptors);
    p->addShader(VKFS::VERTEX, vertex);
    p->addShader(VKFS::FRAGMENT, fragment);
    p->build();
```

### Step 14. Create vertices and indices vectors
```cpp
    std::vector<Vertex> vertices = {
                {{0.f, -1.f, 0.f}},
                {{1.f, 1.f, 0.f}},
                {{-1.f, 1.f, 1.f}}
    };
    
    std::vector<uint32_t> indices = {0, 1, 2};
```

### Step 15. Create Vertex buffer
```cpp
    auto VB = new VKFS::VertexBuffer<Vertex>(device, vertices, indices);
```


### Step 16. Finally, create main loop
```cpp
    while (!quit) {
        while (SDL_PollEvent(&event) != 0) { // Getting events
            if (event.type == SDL_QUIT) {
                quit = true;
            }
        }

        SDL_GetWindowSize(window, &windowWidth, &windowHeight);
        sync->pushWindowSize(windowWidth, windowHeight); // Pass window size to sync object every frame
        
        uint32_t imageIndex = VKFS::prepareFrame(sync); // Getting image index
        VKFS::begin(sync); // Beginning rendering
        
        // Beginning render pass
        VkRenderPassBeginInfo renderPassInfo{};
        renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
        renderPassInfo.renderPass = swapchain->getRenderPass();
        renderPassInfo.framebuffer = swapchain->getFramebuffer(imageIndex);
        renderPassInfo.renderArea.offset = {0, 0};
        renderPassInfo.renderArea.extent = swapchain->getExtent();
        
        std::array<VkClearValue, 2> clearValues{};
        clearValues[0].color = {{0.0f, 0.0f, 0.0f, 1.0f}};
        clearValues[1].depthStencil = {1.0f, 0};
        
        renderPassInfo.clearValueCount = static_cast<uint32_t>(clearValues.size());
        renderPassInfo.pClearValues = clearValues.data();
        
        vkCmdBeginRenderPass(sync->getCommandBuffer(), &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

        VB->draw(sync, p->getPipelineLayout(), p->getPipeline(), swapchain->getExtent()); // Drawing

        // Finishing rendering
        vkCmdEndRenderPass(sync->getCommandBuffer());
        VKFS::end(sync, imageIndex);
    }
```

#### And as a result, you should have something like this:
![Triangle](images/triangle.png)
