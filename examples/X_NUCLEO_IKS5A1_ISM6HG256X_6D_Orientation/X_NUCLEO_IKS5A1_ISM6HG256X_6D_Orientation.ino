/*
   @file    X_NUCLEO_IKS5A1_ISM6HG256X_6D_Orientation.ino
   @author  STMicroelectronics
   @brief   Example to use the ISM6HG256X 6D Orientation
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

#define INT1_pin 5

ISM6HG256XSensor ISM6HG256X(&Wire);
//Interrupts.
volatile int mems_event = 0;

char report[256];

void INT1Event_cb();
void sendOrientation();

void setup()
{

  // Initlialize serial.
  Serial.begin(115200);
  delay(1000);

  // Initlialize Led.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initlialize i2c.
  Wire.begin();

  // Enable INT1 pin.
  attachInterrupt(INT1_pin, INT1Event_cb, RISING);

  // Initlialize components.
  ISM6HG256X.begin();
  ISM6HG256X.Enable_X();

  // Enable 6D Orientation.
  ISM6HG256X.Enable_6D_Orientation(ISM6HG256X_INT1_PIN);
}

void loop()
{
  if (mems_event) {
    mems_event = 0;
    ISM6HG256X_Event_Status_t status;
    ISM6HG256X.Get_X_Event_Status(&status);

    if (status.D6DOrientationStatus) {
      // Send 6D Orientation
      sendOrientation();

      // Led blinking.
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void INT1Event_cb()
{
  mems_event = 1;
}

void sendOrientation()
{
  uint8_t xl = 0;
  uint8_t xh = 0;
  uint8_t yl = 0;
  uint8_t yh = 0;
  uint8_t zl = 0;
  uint8_t zh = 0;

  ISM6HG256X.Get_6D_Orientation_XL(&xl);
  ISM6HG256X.Get_6D_Orientation_XH(&xh);
  ISM6HG256X.Get_6D_Orientation_YL(&yl);
  ISM6HG256X.Get_6D_Orientation_YH(&yh);
  ISM6HG256X.Get_6D_Orientation_ZL(&zl);
  ISM6HG256X.Get_6D_Orientation_ZH(&zh);

  if (xl == 0 && yl == 0 && zl == 0 && xh == 0 && yh == 1 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |  *             | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 1 && yl == 0 && zl == 0 && xh == 0 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |             *  | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 0 && zl == 0 && xh == 1 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |  *             | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 1 && zl == 0 && xh == 0 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |                | " \
            "\r\n |             *  | " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 0 && zl == 0 && xh == 0 && yh == 0 && zh == 1) {
    sprintf(report, "\r\n  __*_____________  " \
            "\r\n |________________| \r\n");
  }

  else if (xl == 0 && yl == 0 && zl == 1 && xh == 0 && yh == 0 && zh == 0) {
    sprintf(report, "\r\n  ________________  " \
            "\r\n |________________| " \
            "\r\n    *               \r\n");
  }

  else {
    sprintf(report, "None of the 6D orientation axes is set in accelerometer.\r\n");
  }

  Serial.print(report);
}
