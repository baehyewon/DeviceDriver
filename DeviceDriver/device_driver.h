#pragma once
#include "flash_memory_device.h"

class DeviceDriver
{
public:
    DeviceDriver(FlashMemoryDevice* hardware);
    int read(long address);
    void write(long address, int data);
    void postConditionCheck(int ret, long address);
    void preconditionCheck(long address);

protected:
    FlashMemoryDevice* m_hardware;
    int TestTryCount = 4;
};