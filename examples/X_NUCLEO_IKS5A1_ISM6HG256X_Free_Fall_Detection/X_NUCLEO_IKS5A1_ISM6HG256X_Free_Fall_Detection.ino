/*
   @file    X_NUCLEO_IKS5A1_ISM6HG256X_Free_Fall_Detection.ino
   @author  STMicroelectronics
   @brief   Example to use the ISM6HG256X Free Fall Detection
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
void INT1Event_cb();


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

  // Enable Free Fall Detection.
  ISM6HG256X.Enable_Free_Fall_Detection(ISM6HG256X_INT1_PIN);
}

void loop()
{
  if (mems_event) {
    mems_event = 0;
    ISM6HG256X_Event_Status_t status;
    ISM6HG256X.Get_X_Event_Status(&status);

    if (status.FreeFallStatus) {
      // Led blinking.
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Free Fall Detected!");
    }
  }
}

void INT1Event_cb()
{
  mems_event = 1;
}
