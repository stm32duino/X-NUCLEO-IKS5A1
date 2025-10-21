/*
   @file    X_NUCLEO_IKS5A1_ISM6HG256X_FIFO_Polling.ino
   @author  STMicroelectronics
   @brief   Example to use the ISM6HG256X library with FIFO in polling mode.
 *******************************************************************************
   Copyright (c) 2025, STMicroelectronics
   All rights reserved.

   This software component is licensed by ST under BSD 3-Clause license,
   the "License"; You may not use this file except in compliance with the
   License. You may obtain a copy of the License at:
                          opensource.org/licenses/BSD-3-Clause

 *******************************************************************************
*/
#include <ISM6HG256XSensor.h>

#define SENSOR_ODR 104.0f // In Hertz
#define ACC_FS 2 // In g
#define GYR_FS 2000 // In dps
#define MEASUREMENT_TIME_INTERVAL (1000.0f/SENSOR_ODR) // In ms
#define FIFO_SAMPLE_THRESHOLD 199
#define FLASH_BUFF_LEN 8192

ISM6HG256XSensor AccGyr(&Wire);
volatile uint8_t fullFlag = 0; // FIFO full flag
uint8_t status = 0;
unsigned long timestamp_count = 0;
bool acc_available = false;
bool gyr_available = false;
ISM6HG256X_Axes_t acc_value;
ISM6HG256X_Axes_t gyr_value;
char buff[FLASH_BUFF_LEN];
uint32_t pos = 0;

void Read_FIFO_Data();

void setup()
{

  Serial.begin(115200);
  Wire.begin();

  // Initialize ISM6HG256X.
  AccGyr.begin();
  status |= AccGyr.Enable_X();
  status |= AccGyr.Enable_G();

  // Configure ODR and FS of the acc and gyro
  status |= AccGyr.Set_X_OutputDataRate(SENSOR_ODR);
  status |= AccGyr.Set_X_FullScale(ACC_FS);
  status |= AccGyr.Set_G_OutputDataRate(SENSOR_ODR);
  status |= AccGyr.Set_G_FullScale(GYR_FS);

  // Configure FIFO BDR for acc and gyro
  status |= AccGyr.FIFO_X_Set_BDR(SENSOR_ODR);
  status |= AccGyr.FIFO_G_Set_BDR(SENSOR_ODR);

  // Set Set FIFO watermark level
  status |= AccGyr.FIFO_Set_Watermark_Level(FIFO_SAMPLE_THRESHOLD);
  // Set FIFO stop on watermark level
  status |= AccGyr.FIFO_Set_Stop_On_Fth(1);

  // Set FIFO in Continuous mode
  status |= AccGyr.FIFO_Set_Mode(ISM6HG256X_STREAM_MODE);

  if (status != ISM6HG256X_OK) {
    Serial.println("ISM6HG256XSensor failed to init/configure");
    while (1);
  }
  Serial.println("ISM6HG256XFIFO Demo");
}

void loop()
{
  uint16_t fifo_samples;

  // Check the number of samples inside FIFO
  if (AccGyr.FIFO_Get_Num_Samples(&fifo_samples) != ISM6HG256X_OK) {
    Serial.println("ISM6HG256XSensor failed to get number of samples inside FIFO");
    while (1);
  }
  // If we reach the threshold we can empty the FIFO
  if (fifo_samples == FIFO_SAMPLE_THRESHOLD) {

    // Empty the FIFO
    Read_FIFO_Data();

    // Print FIFO data
    Serial.print(buff);
  }
}

void Read_FIFO_Data()
{
  uint16_t i;
  uint16_t samples_to_read;

  // Check the number of samples inside FIFO
  if (AccGyr.FIFO_Get_Num_Samples(&samples_to_read) != ISM6HG256X_OK) {
    Serial.println("ISM6HG256XSensor failed to get number of samples inside FIFO");
    while (1);
  }

  for (i = 0; i < samples_to_read; i++) {
    uint8_t tag;

    // Check the FIFO tag
    if (AccGyr.FIFO_Get_Tag(&tag) != ISM6HG256X_OK) {
      Serial.println("ISM6HG256XSensor failed to get tag");
      while (1);
    }
    switch (tag) {
      // If we have a gyro tag, read the gyro data
      case 1: {
          if (AccGyr.FIFO_G_Get_Axes(&gyr_value) != ISM6HG256X_OK) {
            Serial.println("ISM6HG256XSensor failed to get gyroscope data");
            while (1);
          }
          gyr_available = true;
          break;
        }

      // If we have an acc tag, read the acc data
      case 2: {
          if (AccGyr.FIFO_X_Get_Axes(&acc_value) != ISM6HG256X_OK) {
            Serial.println("ISM6HG256XSensor failed to get accelerometer data");
            while (1);
          }
          acc_available = true;
          break;
        }

      // We can discard other tags
      default: {
          break;
        }
    }
    // If we have the measurements of both acc and gyro, we can store them with timestamp
    if (acc_available && gyr_available) {
      int num_bytes;
      num_bytes = snprintf(&buff[pos], (FLASH_BUFF_LEN - pos), "%lu %d %d %d %d %d %d\r\n", (unsigned long)((float)timestamp_count * MEASUREMENT_TIME_INTERVAL), (int)acc_value.x, (int)acc_value.y, (int)acc_value.z, (int)gyr_value.x, (int)gyr_value.y, (int)gyr_value.z);
      pos += num_bytes;
      timestamp_count++;
      acc_available = false;
      gyr_available = false;
    }
  }
  // We can add the termination character to the string, so we are ready to print it on hyper-terminal
  buff[pos] = '\0';
  pos = 0;
}

