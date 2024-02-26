//
// Created by Alexander Blinov on 24.05.2023.
//

#include "../include/VKFS/VKFS.h"

uint32_t VKFS::prepareFrame(Synchronization* sync) {
    sync->waitForFences();
    return sync->acquireNextImage();
}

void VKFS::begin(Synchronization* sync) {
    sync->resetAll();
    sync->beginRecordingCommands();
}

void VKFS::end(Synchronization* sync, uint32_t imageIndex) {
    sync->endRecordingCommands();
    sync->submit(imageIndex);
}

void VKFS::prepareCompute(VKFS::Synchronization *sync) {
    sync->waitCompute();
}

void VKFS::beginCompute(VKFS::Synchronization *sync) {
    sync->resetCompute();
    sync->beginRecordingCompute();
}

void VKFS::endCompute(VKFS::Synchronization *sync) {
    sync->endRecordingCompute();
    sync->submitCompute();
}
