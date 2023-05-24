//
// Created by Alexander Blinov on 24.05.2023.
//

#include "../include/VKFS.h"

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