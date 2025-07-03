#include "device_driver.h"
#include <exception>

class ReadFiveTimeFail : public std::exception {};
class WriteWhenRead0xFF : public std::exception {};

DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{}

int DeviceDriver::read(long address) {
    int result = (int)(m_hardware->read(address));
    postConditionCheck(result, address);
    return result;
}

void DeviceDriver::postConditionCheck(int ret, long address) {
    for (int turn = 0; turn < TestTryCount; turn++) {
        int testValue = (int)(m_hardware->read(address));
        if (ret != testValue) {
            throw ReadFiveTimeFail();
        }
    }
}

void DeviceDriver::write(long address, int data)
{
    preconditionCheck(address);
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::preconditionCheck(long address)
{
    int testValue = (int)(m_hardware->read(address));
    if (testValue != 0xFF) {
        throw WriteWhenRead0xFF();
    }
}