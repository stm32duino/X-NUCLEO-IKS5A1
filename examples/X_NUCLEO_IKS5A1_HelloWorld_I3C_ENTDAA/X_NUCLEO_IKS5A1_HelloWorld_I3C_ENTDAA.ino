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
  if (!I3C.isI3CDeviceReady(IIS2DULPX_I3C_ADD_H)) {
    Serial.println("isI3CDeviceReady() failed");
    while (1) {}
  }
  I3CDiscoveredDevice devices[8] = {};
  size_t found = 0;
  if (I3C.discover(devices, 8, &found)) {
    Serial.println("discover() failed");
    while (1) {}
  }

  uint8_t iisDynAddr = 0U;
  uint8_t ilpDynAddr = 0U;
  uint8_t ismDynAddr = 0U;

  for (size_t index = 0; index < found; ++index) {
    Serial.println(devices[index].pid, HEX);
    if (devices[index].pid == IIS2DULPX_I3C_PID_H) {
      iisDynAddr = devices[index].dynAddr;
      Serial.print("iisDynAddr=");
      Serial.println(iisDynAddr, HEX);
    } else if (devices[index].pid == ILPS22QS_I3C_PID) {
      ilpDynAddr = devices[index].dynAddr;
      Serial.print("ilpDynAddr=");
      Serial.println(ilpDynAddr, HEX);
    } else if (devices[index].pid == ISM6HG256X_I3C_PID_L) {
      ismDynAddr = devices[index].dynAddr;
      Serial.print("ismDynAddr=");
      Serial.println(ismDynAddr, HEX);
    }

    if (iisDynAddr != 0U && ilpDynAddr != 0U && ismDynAddr != 0U) {
      break;
    }
  }

  if (iisDynAddr == 0U) {
    Serial.println("IIS2DULPX not found");
    while (1) {}
  }
  if (ilpDynAddr == 0U) {
    Serial.println("ILPS22QS not found");
    while (1) {}
  }
  if (ismDynAddr == 0U) {
    Serial.println("ISM6HG256X not found");
    while (1) {}
  }

  if (!I3C.setClock(12500000)) {
    Serial.println("setClock() failed");
    while (1) {}
  }
  if (Acc.begin(iisDynAddr) != IIS2DULPX_OK) {
    Serial.println("Acc.begin() failed");
    while (1) {}
  }
  if (PressTemp.begin(ilpDynAddr) != ILPS22QS_OK) {
    Serial.println("PressTemp.begin() failed");
    while (1) {}
  }
  if (AccGyr.begin(ismDynAddr) != ISM6HG256X_OK) {
    Serial.println("AccGyr.begin() failed");
    while (1) {}
  }
  if (Acc.Enable_X() != IIS2DULPX_OK) {
    Serial.println("IIS2DULPX Enable_X() failed");
    while (1) {}
  }
  if (PressTemp.Enable() != ILPS22QS_OK) {
    Serial.println("ILPS22QS Enable() failed");
    while (1) {}
  }
  if (AccGyr.Enable_X() != ISM6HG256X_OK) {
    Serial.println("ISM6HG256X Enable_X() failed");
    while (1) {}
  }
  if (AccGyr.Enable_G() != ISM6HG256X_OK) {
    Serial.println("ISM6HG256X Enable_G() failed");
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
