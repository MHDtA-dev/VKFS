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

#include "../../include/VKFS/Extensions/ShapeConstructor.h"

VKFS::EXT::ShapeInfo VKFS::EXT::ShapeConstructor::createSphere(int segments, int rings) {
    ShapeInfo sphereInfo;

    for (int ring = 0; ring <= rings; ++ring) {
        float phi = static_cast<float>(ring) / rings * M_PI;
        for (int segment = 0; segment <= segments; ++segment) {
            float theta = static_cast<float>(segment) / segments * 2.0f * M_PI;

            float x = cos(theta) * sin(phi);
            float y = cos(phi);
            float z = sin(theta) * sin(phi);

            ShapeVertex vertex;
            vertex.position[0] = x;
            vertex.position[1] = y;
            vertex.position[2] = z;

            vertex.normal[0] = x;
            vertex.normal[1] = y;
            vertex.normal[2] = z;

            vertex.texCoords[0] = static_cast<float>(segment) / segments;
            vertex.texCoords[1] = static_cast<float>(ring) / rings;

            sphereInfo.vertices.push_back(vertex);
        }
    }

    for (int ring = 0; ring < rings; ++ring) {
        for (int segment = 0; segment < segments; ++segment) {
            uint32_t current = ring * (segments + 1) + segment;
            uint32_t next = current + segments + 1;

            sphereInfo.indices.push_back(current);
            sphereInfo.indices.push_back(next);
            sphereInfo.indices.push_back(current + 1);

            sphereInfo.indices.push_back(current + 1);
            sphereInfo.indices.push_back(next);
            sphereInfo.indices.push_back(next + 1);
        }
    }

    return sphereInfo;
}
