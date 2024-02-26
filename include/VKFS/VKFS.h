/*
(c) Copyright 2023 MHDtA

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

   1. Redistributions of source code must retain the above copyright notice, this
      list of conditions and the following disclaimer.


THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT,
INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED
OF THE POSSIBILITY OF SUCH DAMAGE.
*/


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
#include "Pipeline.h"
#include "Offscreen.h"
#include "Image.h"
#include "ComputePipeline.h"
#include "StorageImage.h"

namespace VKFS {

    uint32_t prepareFrame(Synchronization* sync);
    void begin(Synchronization* sync);
    void end(Synchronization* sync, uint32_t imageIndex);

    void prepareCompute(Synchronization* sync);
    void beginCompute(Synchronization* sync);
    void endCompute(Synchronization* sync);
}

#endif //VKFS_VKFS_H
