/*
   @file    X_NUCLEO_IKS5A1_HelloWorld.ino
   @author  STMicroelectronics
   @brief   Example to use the STMicrolectronics X-NUCLEO-IKS5A1
 *******************************************************************************
   Copyright (c) 2025, STMicroelectronics
   All rights reserved.
   This software component is licensed by ST under BSD 3-Clause license,
   the "License"; You may not use this file except in compliance with the
   License. You may obtain a copy of the License at:
                          opensource.org/licenses/BSD-3-Clause
 *******************************************************************************
*/

// Includes
#include <ILPS22QSSensor.h>
#include <ISM6HG256XSensor.h>
#include <ISM330ISSensor.h>
#include <IIS2DULPXSensor.h>
#include <IIS2MDCSensor.h>

#ifdef ARDUINO_SAM_DUE
#define DEV_I2C Wire1
#elif defined(ARDUINO_ARCH_STM32)
#define DEV_I2C Wire
#elif defined(ARDUINO_ARCH_AVR)
#define DEV_I2C Wire
#else
#define DEV_I2C Wire
#endif
#define SerialPort Serial

// Components
ILPS22QSSensor PressTemp(&DEV_I2C);
ISM6HG256XSensor AccGyr(&DEV_I2C);
ISM330ISSensor AccGyr2(&DEV_I2C);
IIS2DULPXSensor Acc3(&DEV_I2C);
IIS2MDCSensor Mag(&DEV_I2C);

void setup() {
  // Led.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize serial for output.
  SerialPort.begin(115200);
  
  // Initialize I2C bus.
  DEV_I2C.begin();

  PressTemp.begin();
  PressTemp.Enable();
  AccGyr.begin();
  AccGyr.Enable_X();
  AccGyr.Enable_G();
  AccGyr2.begin();
  AccGyr2.Enable_X();
  AccGyr2.Enable_G();
  Acc3.begin();
  Acc3.Enable_X();
  Mag.begin();
  Mag.Enable();
}

void loop() {
  // Led blinking.
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  // Read pressure and temperature.
  float pressure = 0, temperature = 0;
  PressTemp.GetPressure(&pressure);
  PressTemp.GetTemperature(&temperature);

  // Read accelerometer and gyroscope.
  ISM6HG256X_Axes_t accelerometer;
  ISM6HG256X_Axes_t gyroscope;
  AccGyr.Get_X_Axes(&accelerometer);
  AccGyr.Get_G_Axes(&gyroscope);

  // Read accelerometer and gyroscope.
  ISM330IS_Axes_t accelerometer2;
  ISM330IS_Axes_t gyroscope2;
  AccGyr2.Get_X_Axes(&accelerometer2);
  AccGyr2.Get_G_Axes(&gyroscope2);

  //Read accelerometer
  IIS2DULPX_Axes_t accelerometer3;
  Acc3.Get_X_Axes(&accelerometer3);

  //Read magnetometer
  int32_t magnetometer[3];
  Mag.GetAxes(magnetometer);

  // Output data.
  SerialPort.print("| Pres[hPa]: ");
  SerialPort.print(pressure, 2);
  SerialPort.print(" | Temp[C]: ");
  SerialPort.print(temperature, 2);
  SerialPort.println(" |");
  SerialPort.print("| Acc[mg]: ");
  SerialPort.print(accelerometer.x);
  SerialPort.print(" ");
  SerialPort.print(accelerometer.y);
  SerialPort.print(" ");
  SerialPort.print(accelerometer.z);
  SerialPort.print(" | Gyr[mdps]: ");
  SerialPort.print(gyroscope.x);
  SerialPort.print(" ");
  SerialPort.print(gyroscope.y);
  SerialPort.print(" ");
  SerialPort.print(gyroscope.z);
  SerialPort.println(" |");
  SerialPort.print("| Acc2[mg]: ");
  SerialPort.print(accelerometer2.x);
  SerialPort.print(" ");
  SerialPort.print(accelerometer2.y);
  SerialPort.print(" ");
  SerialPort.print(accelerometer2.z);
  SerialPort.print(" | Gyr2[mdps]: ");
  SerialPort.print(gyroscope2.x);
  SerialPort.print(" ");
  SerialPort.print(gyroscope2.y);
  SerialPort.print(" ");
  SerialPort.print(gyroscope2.z);
  SerialPort.println(" |");
  SerialPort.print("| Acc3[mg]: ");
  SerialPort.print(accelerometer3.x);
  SerialPort.print(" ");
  SerialPort.print(accelerometer3.y);
  SerialPort.print(" ");
  SerialPort.print(accelerometer3.z);
  SerialPort.println(" |");
  SerialPort.print("| Mag[mGauss]: ");
  SerialPort.print(magnetometer[0]);
  SerialPort.print(" ");
  SerialPort.print(magnetometer[1]);
  SerialPort.print(" ");
  SerialPort.print(magnetometer[2]);
  SerialPort.println(" |");
  SerialPort.println("");
}
