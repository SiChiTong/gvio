#include "gvio/munit.hpp"
#include "gvio/imu/mpu6050.hpp"

namespace gvio {

#define TEST_CONFIG "test_configs/imu/config.yaml"
int test_MPU6050_configure() {
  int retval;
  MPU6050 imu;

  retval = imu.configure(TEST_CONFIG);
  MU_CHECK_EQ(0, retval);

  return 0;
}

void test_suite() { MU_ADD_TEST(test_MPU6050_configure); }

} // namespace gvio

MU_RUN_TESTS(gvio::test_suite);
