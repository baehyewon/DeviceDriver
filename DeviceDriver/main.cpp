#include "gmock/gmock.h"
#include "device_driver.h"

using namespace testing;

class FlashMock : public FlashMemoryDevice {
public:
	// FlashMemoryDevice의 메서드를 모두 만들어 줘야함
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class MockFixture : public Test {
protected:
	NiceMock<FlashMock> mockHW;
	DeviceDriver dd{ &mockHW };	// mock injection

	const int READ_WRITE_ADDRESS = 0xA;
	const int EXPECT_READ_DATA = 0x77;
	const int UNEXPECT_READ_DATA = 0x67;
	const int DATA_IS_EMPTY = 0xFF;
	const int DATA_ALREADY_WRITE = 0xFD;
	const int WRITE_DATA = 0x33;
	const int MUST_READ_TIMES = 5;
	const int CHECK_READ_TIMES = 1;
};

TEST_F(MockFixture, Read5Times) {
	EXPECT_CALL(mockHW, read((long)READ_WRITE_ADDRESS))
		.Times(MUST_READ_TIMES)
		.WillRepeatedly(Return((int)EXPECT_READ_DATA));

	int result = dd.read(READ_WRITE_ADDRESS);
}

TEST_F(MockFixture, ReadDifferentValue) {
	EXPECT_CALL(mockHW, read((long)READ_WRITE_ADDRESS))
		.WillOnce(Return((int)EXPECT_READ_DATA))
		.WillOnce(Return((int)EXPECT_READ_DATA))
		.WillOnce(Return((int)EXPECT_READ_DATA))
		.WillOnce(Return((int)EXPECT_READ_DATA))
		.WillOnce(Return((int)UNEXPECT_READ_DATA));

	EXPECT_THROW(dd.read(READ_WRITE_ADDRESS), std::exception);
}

TEST_F(MockFixture, WritePreconditionRead) {
	EXPECT_CALL(mockHW, read((long)READ_WRITE_ADDRESS))
		.Times(CHECK_READ_TIMES)
		.WillOnce(Return(DATA_IS_EMPTY));

	dd.write((long)READ_WRITE_ADDRESS, WRITE_DATA);
}

TEST_F(MockFixture, WriteAlreadyRead) {
	EXPECT_CALL(mockHW, read((long)READ_WRITE_ADDRESS))
		.Times(CHECK_READ_TIMES)
		.WillOnce(Return(DATA_ALREADY_WRITE));

	EXPECT_THROW(dd.write((long)READ_WRITE_ADDRESS, WRITE_DATA),
		std::exception);
}

int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}