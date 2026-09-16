/*
   @file    X_NUCLEO_IKS5A1_HelloWorld_I3C_ENTDAA.ino
   @author  STMicroelectronics
   @brief   Example to use the IIS2DULPX, ILPS22QS and ISM6HG256X sensors with I3C dynamic address assignment
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

IIS2DULPXSensor Acc(&I3C);
ILPS22QSSensor PressTemp(&I3C);
ISM6HG256XSensor AccGyr(&I3C);

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
  I3CDiscoveredDevice devices[8] = {};
  size_t found = 0;
  if (I3C.discover(devices, 8, &found)) {
    Serial.println("discover() failed");
    while (1) {}
  }

  bool accFound = false;
  bool pressTempFound = false;
  bool accGyrFound = false;
  for (size_t index = 0; index < found; ++index) {
    if (!accFound && Acc.begin(devices[index].dynAddr) == IIS2DULPX_OK) {
      accFound = true;
    }
    if (!pressTempFound && PressTemp.begin(devices[index].dynAddr) == ILPS22QS_OK) {
      pressTempFound = true;
    }
    if (!accGyrFound && AccGyr.begin(devices[index].dynAddr) == ISM6HG256X_OK) {
      accGyrFound = true;
    }
  }

  if (!accFound || !pressTempFound || !accGyrFound) {
    Serial.println("One or more sensors not found");
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
