#include <cmath>
#include <bitset>

#include <ze/common/test_entrypoint.h>
#include <ze/common/versioned_slot_handle.h>

TEST(VersionedSlotHandle, test)
{
	using VIdx = ze::VersionedSlotHandle<uint32_t, 8, 24>;
	VIdx v1;
	EXPECT_EQ(sizeof(VIdx), 4);

	v1.slot = 1;
	v1.version = 1;
	std::cout << std::bitset<32>(v1.handle) << std::endl;
	EXPECT_EQ(v1.handle, 1 + (1<<8));

	v1.slot = VIdx::maxSlot();
	v1.version = VIdx::maxVersion();
	std::cout << std::bitset<32>(v1.handle) << std::endl;
	EXPECT_EQ(v1.handle, std::numeric_limits<uint32_t>::max());

	VIdx v2;
	EXPECT_EQ(v2.handle, 0);

	VIdx v3(10);
	EXPECT_EQ(v3.handle, 10);

	VIdx v4(10, 2);
	EXPECT_EQ(v4.slot, 10);
	EXPECT_EQ(v4.version, 2);
}

ZE_UNITTEST_ENTRYPOINT
