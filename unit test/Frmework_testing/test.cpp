#include "pch.h"
#include "include/util/enRangeIndexTracker.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(test_class_enRangeIndexTracker, valid_Indexes_signed)
{
  enRangeIndexTracker<long,3> instance;

  auto index = instance.getFirstUnusedIndex(true);
  EXPECT_EQ(index,0);

  index = instance.getFirstUnusedIndex(true);
  EXPECT_EQ(index,1);

  index = instance.getFirstUnusedIndex(true);
  EXPECT_EQ(index,2);

  index = instance.getFirstUnusedIndex(true);
  ASSERT_EQ(index, -1);
}

TEST(test_class_enRangeIndexTracker, valid_Indexes_unsigned)
{
  enRangeIndexTracker<unsigned long,3> instance;

  auto index = instance.getFirstUnusedIndex(true);
  EXPECT_EQ(index,0);

  index = instance.getFirstUnusedIndex(true);
  EXPECT_EQ(index,1);

  index = instance.getFirstUnusedIndex(true);
  EXPECT_EQ(index,2);

  index = instance.getFirstUnusedIndex(true);
  ASSERT_EQ(index, -1);
}


TEST(test_class_enRangeIndexTracker, valid_size_return)
{
  enRangeIndexTracker<long, 3> instance;
  size_t rangeSize = instance.getSizeOfRange();
  ASSERT_EQ(rangeSize, 3);

  enRangeIndexTracker<long, 0>instance3;
  rangeSize = instance3.getSizeOfRange();
  ASSERT_EQ(rangeSize, 0);
}


TEST(test_class_enRangeIndexTracker, registering_indexes_as_used_test)
{
  enRangeIndexTracker<long, 3> instance;

  for( size_t i = 0; i < instance.getSizeOfRange(); ++i )
  {
    auto index = instance.getFirstUnusedIndex(false);
    bool isRegisteringSuccessful = instance.registerIndexAsUsed(index);
    ASSERT_EQ(isRegisteringSuccessful, true);
  }

  bool shouldFail = instance.registerIndexAsUsed(instance.getSizeOfRange() + 1);
  ASSERT_EQ(shouldFail, false);

  shouldFail = instance.registerIndexAsUsed(-1);
  ASSERT_EQ(shouldFail, false);

}

TEST(test_class_enRangeIndexTracker, registering_indexes_as_available_test)
{
  enRangeIndexTracker<long, 3> instance;

  for( size_t i = 0; i < instance.getSizeOfRange(); ++i )
  {
    instance.getFirstUnusedIndex(true);
  }

  for( size_t i = 0; i < instance.getSizeOfRange(); ++i )
  {
    bool const isSuccessful = instance.registerIndexAsAvailable(i);
    ASSERT_EQ(isSuccessful, true);
  }

    bool shouldFail = instance.registerIndexAsAvailable(instance.getSizeOfRange() + 1);
    ASSERT_EQ(shouldFail, false);

    shouldFail = instance.registerIndexAsAvailable(-1);
    ASSERT_EQ(shouldFail, false);

}
