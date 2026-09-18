/*
   @file    X_NUCLEO_IKS5A1_HelloWorld_I3C.ino
   @author  STMicroelectronics
   @brief   Example to use the IIS2DULPX, ILPS22QS and ISM6HG256X sensors with I3C and SETDASA
 *******************************************************************************
   Copyright (c) 2026, STMicroelectronics
   All rights reserved.
   This software component is licensed by ST under BSD 3-Clause license,
   the "License"; You may not use this file except in compliance with the
   License. You may obtain a copy of the License at:
                          opensource.org/licenses/BSD-3-Clause
 *******************************************************************************
*/

#include <IIS2DULPXSensor.h>
#include <ILPS22QSSensor.h>
#include <ISM6HG256XSensor.h>

#define IIS2DULPX_DYNAMIC_ADDRESS 0x30
#define ILPS22QS_DYNAMIC_ADDRESS  0x33
#define ISM6HG256X_DYNAMIC_ADDRESS 0x36

IIS2DULPXSensor Acc(&I3C, IIS2DULPX_I3C_ADD_H);
ILPS22QSSensor PressTemp(&I3C, ILPS22QS_I3C_ADD);
ISM6HG256XSensor AccGyr(&I3C, ISM6HG256X_I3C_ADD_L);

void setup()
{
  Serial.begin(115200);
  while (!Serial) {}
  delay(1000);

  if (!I3C.begin(I3C_SDA, I3C_SCL, 1000000U)) {
    Serial.println("begin() failed");
    while (1) {}
  }
  if (!I3C.resetDynamicAddresses()) {
    Serial.println("resetDynamicAddresses() failed");
    while (1) {}
  }
  if (!I3C.isI3CDeviceReady(0x19)) {
    Serial.println("isI3CDeviceReady() failed");
    while (1) {}
  }
  if (!I3C.assignDynamicAddress(Acc.getStaticAddress(), IIS2DULPX_DYNAMIC_ADDRESS) ||
      !I3C.assignDynamicAddress(PressTemp.getStaticAddress(), ILPS22QS_DYNAMIC_ADDRESS) ||
      !I3C.assignDynamicAddress(AccGyr.getStaticAddress(), ISM6HG256X_DYNAMIC_ADDRESS)) {
    Serial.println("assignDynamicAddress() failed");
    while (1) {}
  }
  if (Acc.begin(IIS2DULPX_DYNAMIC_ADDRESS) != IIS2DULPX_OK ||
      PressTemp.begin(ILPS22QS_DYNAMIC_ADDRESS) != ILPS22QS_OK ||
      AccGyr.begin(ISM6HG256X_DYNAMIC_ADDRESS) != ISM6HG256X_OK) {
    Serial.println("sensor begin() failed");
    while (1) {}
  }
  if (!I3C.setClock(12500000)) {
    Serial.println("setClock() failed");
    while (1) {}
  }
  if (Acc.Enable_X() != IIS2DULPX_OK ||
      PressTemp.Enable() != ILPS22QS_OK ||
      AccGyr.Enable_X() != ISM6HG256X_OK ||
      AccGyr.Enable_G() != ISM6HG256X_OK) {
    Serial.println("sensor Enable() failed");
    while (1) {}
  }
}

void loop()
{
  IIS2DULPX_Axes_t accelerometer;
  float pressure = 0.0f;
  float temperature = 0.0f;
  ISM6HG256X_Axes_t accelerometer2;
  ISM6HG256X_Axes_t gyroscope;

  if (Acc.Get_X_Axes(&accelerometer) != IIS2DULPX_OK ||
      PressTemp.GetPressure(&pressure) != ILPS22QS_OK ||
      PressTemp.GetTemperature(&temperature) != ILPS22QS_OK ||
      AccGyr.Get_X_Axes(&accelerometer2) != ISM6HG256X_OK ||
      AccGyr.Get_G_Axes(&gyroscope) != ISM6HG256X_OK) {
    Serial.println("Read failed");
    delay(500);
    return;
  }

  Serial.print("Acc[mg]: ");
  Serial.print(accelerometer.x);
  Serial.print(" ");
  Serial.print(accelerometer.y);
  Serial.print(" ");
  Serial.print(accelerometer.z);
  Serial.print(" | Press[hPa]: ");
  Serial.print(pressure, 2);
  Serial.print(" | Temp[C]: ");
  Serial.println(temperature, 2);

  Serial.print("Acc2[mg]: ");
  Serial.print(accelerometer2.x);
  Serial.print(" ");
  Serial.print(accelerometer2.y);
  Serial.print(" ");
  Serial.print(accelerometer2.z);
  Serial.print(" | Gyr[mdps]: ");
  Serial.print(gyroscope.x);
  Serial.print(" ");
  Serial.print(gyroscope.y);
  Serial.print(" ");
  Serial.println(gyroscope.z);

  delay(500);
}
