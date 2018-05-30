#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <aurora/utils.h>
#include <aurora/errors/AuroraError.h>
#include <vector>

using namespace aurora;
using namespace testing; 

namespace {
TEST(UTILStest, littleendian_to_uint){
  char first = 10;
  char second = 0;
  EXPECT_EQ(littleendian_to_uint(first, second), 10);
  first = 10;
  second = 20;
  EXPECT_EQ(littleendian_to_uint(first, second), 5130);

}

TEST(UTILStest, rms){
  Buffer buf;
  for(int i=0; i<255; i++){
    buf.push_back(0);
  }
  EXPECT_EQ(rms(2, buf), 0);
  buf.clear();

  for(int i=0; i<255; i++){
    buf.push_back(127);
    buf.push_back(0);
  }
  EXPECT_EQ(rms(2, buf), 127);
  buf.clear();

  for(int i=0; i<10; i++){
    buf.push_back(i);
    buf.push_back(0);
  }
  EXPECT_NEAR(rms(2, buf), 5.33854, 0.1);
}

}