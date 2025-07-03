#include "gmock/gmock.h"
#include "device_driver.h"

class MockDriver : public DeviceDriver {
public:

};

TEST(DeviceDriver, ReadFromHW) {
	// TODO : replace hardware with a Test Double
	FlashMemoryDevice* hardware = nullptr;
	DeviceDriver driver{ hardware };
	int data = driver.read(0xFF);
	EXPECT_EQ(0, data);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}