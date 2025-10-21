/*
   @file    X_NUCLEO_IKS5A1_ISM6HG256X_Single_Tap_Detection.ino
   @author  STMicroelectronics
   @brief   Example to use the ISM6HG256X Single Tap Detection
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
  Serial.begin(115200);
  delay(1000);
  pinMode(LED_BUILTIN, OUTPUT);
  Wire.begin();

  //Interrupts.
  attachInterrupt(INT1_pin, INT1Event_cb, RISING);

  // Initlialize components.
  ISM6HG256X.begin();
  ISM6HG256X.Enable_X();

  // Enable Single Tap Detection.
  ISM6HG256X.Enable_Single_Tap_Detection(ISM6HG256X_INT1_PIN);
}

void loop()
{
  if (mems_event) {
    mems_event = 0;
    ISM6HG256X_Event_Status_t status;
    ISM6HG256X.Get_X_Event_Status(&status);
    if (status.TapStatus) {

      // Led blinking.
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
      digitalWrite(LED_BUILTIN, LOW);
      Serial.println("Single Tap Detected!");
    }
  }
}

void INT1Event_cb()
{
  mems_event = 1;
}
