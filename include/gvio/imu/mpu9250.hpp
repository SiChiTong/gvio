/**
 * @file
 * @ingroup imu
 */
#ifndef GVIO_IMU_MPU9250_HPP
#define GVIO_IMU_MPU9250_HPP

#include "gvio/driver/i2c.hpp"

namespace gvio {
/**
 * @addtogroup imu
 * @{
 */

/* GENERAL */
#define MPU9250_ADDRESS 0x68
#define MPU9250_ADDRESS_AD0_LOW 0x68  // addr pin low (GND) [default]
#define MPU9250_ADDRESS_AD0_HIGH 0x69 // addr pin high (VCC)

/* REGISTER ADDRESSES */
#define MPU9250_SELF_TEST_X_GYRO 0x00
#define MPU9250_SELF_TEST_Y_GYRO 0x01
#define MPU9250_SELF_TEST_Z_GYRO 0x02
#define MPU9250_SELF_TEST_X_ACCEL 0x0D
#define MPU9250_SELF_TEST_Y_ACCEL 0x0E
#define MPU9250_SELF_TEST_Z_ACCEL 0x0F
#define MPU9250_XG_OFFSET_H 0x13
#define MPU9250_XG_OFFSET_L 0x14
#define MPU9250_YG_OFFSET_H 0x15
#define MPU9250_YG_OFFSET_L 0x16
#define MPU9250_ZG_OFFSET_H 0x17
#define MPU9250_ZG_OFFSET_L 0x18
#define MPU9250_SMPLRT_DIV 0x19
#define MPU9250_CONFIG 0x1A
#define MPU9250_GYRO_CONFIG 0x1B
#define MPU9250_ACCEL_CONFIG 0x1C
#define MPU9250_ACCEL_CONFIG2 0x1D
#define MPU9250_LP_ACCEL_ODR 0x1E
#define MPU9250_WOM_THR 0x1F
#define MPU9250_FIFO_EN 0x23
#define MPU9250_I2C_MST_CTRL 0x24
#define MPU9250_I2C_SLV0_ADDR 0x25
#define MPU9250_I2C_SLV0_REG 0x26
#define MPU9250_I2C_SLV0_CTRL 0x27
#define MPU9250_I2C_SLV1_ADDR 0x28
#define MPU9250_I2C_SLV1_REG 0x29
#define MPU9250_I2C_SLV1_CTRL 0x2A
#define MPU9250_I2C_SLV2_ADDR 0x28
#define MPU9250_I2C_SLV2_REG 0x2C
#define MPU9250_I2C_SLV2_CTRL 0x2D
#define MPU9250_I2C_SLV3_ADDR 0x2E
#define MPU9250_I2C_SLV3_REG 0x2F
#define MPU9250_I2C_SLV3_CTRL 0x30
#define MPU9250_I2C_SLV4_ADDR 0x31
#define MPU9250_I2C_SLV4_REG 0x32
#define MPU9250_I2C_SLV4_DO 0x33
#define MPU9250_I2C_SLV4_CTRL 0x34
#define MPU9250_I2C_SLV4_DI 0x35
#define MPU9250_I2C_MST_STATUS 0x36
#define MPU9250_INT_PIN_CFG 0x37
#define MPU9250_INT_ENABLE 0x38
#define MPU9250_INT_STATUS 0x3A
#define MPU9250_ACCEL_XOUT_H 0x3B
#define MPU9250_ACCEL_XOUT_L 0x3C
#define MPU9250_ACCEL_YOUT_H 0x3D
#define MPU9250_ACCEL_YOUT_L 0x3E
#define MPU9250_ACCEL_ZOUT_H 0x3F
#define MPU9250_ACCEL_ZOUT_L 0x40
#define MPU9250_TEMP_OUT_H 0x41
#define MPU9250_TEMP_OUT_L 0x42
#define MPU9250_GYRO_XOUT_H 0x43
#define MPU9250_GYRO_XOUT_L 0x44
#define MPU9250_GYRO_YOUT_H 0x45
#define MPU9250_GYRO_YOUT_L 0x46
#define MPU9250_GYRO_ZOUT_H 0x47
#define MPU9250_GYRO_ZOUT_L 0x48
#define MPU9250_EXT_SENS_DATA_00 0x49
#define MPU9250_EXT_SENS_DATA_01 0x4A
#define MPU9250_EXT_SENS_DATA_02 0x4B
#define MPU9250_EXT_SENS_DATA_03 0x4C
#define MPU9250_EXT_SENS_DATA_04 0x4D
#define MPU9250_EXT_SENS_DATA_05 0x4E
#define MPU9250_EXT_SENS_DATA_06 0x4F
#define MPU9250_EXT_SENS_DATA_07 0x50
#define MPU9250_EXT_SENS_DATA_08 0x51
#define MPU9250_EXT_SENS_DATA_09 0x52
#define MPU9250_EXT_SENS_DATA_10 0x53
#define MPU9250_EXT_SENS_DATA_11 0x54
#define MPU9250_EXT_SENS_DATA_12 0x55
#define MPU9250_EXT_SENS_DATA_13 0x56
#define MPU9250_EXT_SENS_DATA_14 0x57
#define MPU9250_EXT_SENS_DATA_15 0x58
#define MPU9250_EXT_SENS_DATA_16 0x59
#define MPU9250_EXT_SENS_DATA_17 0x5A
#define MPU9250_EXT_SENS_DATA_18 0x5B
#define MPU9250_EXT_SENS_DATA_19 0x5C
#define MPU9250_EXT_SENS_DATA_20 0x5D
#define MPU9250_EXT_SENS_DATA_21 0x5E
#define MPU9250_EXT_SENS_DATA_22 0x5F
#define MPU9250_EXT_SENS_DATA_23 0x60
#define MPU9250_I2C_SLV0_DO 0x63
#define MPU9250_I2C_SLV1_DO 0x64
#define MPU9250_I2C_SLV2_DO 0x65
#define MPU9250_I2C_SLV3_DO 0x66
#define MPU9250_I2C_MST_DELAY_CTRL 0x67
#define MPU9250_SIGNAL_PATH_RESET 0x68
#define MPU9250_MOT_DETECT_CTRL 0x69
#define MPU9250_USER_CTRL 0x6A
#define MPU9250_PWR_MGMT_1 0x6B
#define MPU9250_PWR_MGMT_2 0x6C
#define MPU9250_FIFO_COUNTH 0x72
#define MPU9250_FIFO_COUNTL 0x73
#define MPU9250_FIFO_R_W 0x74
#define MPU9250_WHO_AM_I 0x75
#define MPU9250_XA_OFFSET_H 0x77
#define MPU9250_XA_OFFSET_L 0x78
#define MPU9250_YA_OFFSET_H 0x7A
#define MPU9250_YA_OFFSET_L 0x7B
#define MPU9250_ZA_OFFSET_H 0x7D
#define MPU9250_ZA_OFFSET_L 0x7E

class MPU9250Gyroscope {
public:
  float sensitivity = 0.0f;

  int16_t raw_x = 0.0f;
  int16_t raw_y = 0.0f;
  int16_t raw_z = 0.0f;

  float offset_x = 0.0f;
  float offset_y = 0.0f;
  float offset_z = 0.0f;

  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  float pitch = 0.0f;
  float roll = 0.0f;

  MPU9250Gyroscope() {}
};

class MPU9250Accelerometer {
public:
  float sensitivity = 0.0f;

  int16_t raw_x = 0.0f;
  int16_t raw_y = 0.0f;
  int16_t raw_z = 0.0f;

  float offset_x = 0.0f;
  float offset_y = 0.0f;
  float offset_z = 0.0f;

  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  float pitch = 0.0f;
  float roll = 0.0f;

  MPU9250Accelerometer() {}
};

class MPU9250Magnetometer {
public:
  float sensitivity = 0.0f;

  int16_t raw_x = 0;
  int16_t raw_y = 0;
  int16_t raw_z = 0;

  float offset_x = 0.0f;
  float offset_y = 0.0f;
  float offset_z = 0.0f;

  float x = 0.0f;
  float y = 0.0f;
  float z = 0.0f;

  float heading = 0.0f;

  MPU9250Magnetometer() {}
};

class MPU9250 {
public:
  MPU9250Gyroscope gyro;
  MPU9250Accelerometer accel;
  MPU9250Magnetometer mag;
  I2C i2c;

  float pitch = 0.0f;
  float roll = 0.0f;
  float temperature = 0.0f;

  float pitch_offset = 0.0f;
  float roll_offset = 0.0f;

  clock_t last_updated = 0;
  float sample_rate = -1.0f;

  MPU9250() {}

  int configure();
  int ping();
  int setGyroScale(int8_t scale);
  int getGyroScale();
  int setAccelScale(int8_t scale);
  int getAccelScale();
  int setAccelFchoice(int8_t fchoice);
  int getAccelFchoice();
};

/** @} group imu */
} // namespace gvio
#endif // GVIO_IMU_MPU9250_HPP
