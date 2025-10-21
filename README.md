# X-NUCLEO-IKS5A1

The X-NUCLEO-IKS5A1 is a industrial motion MEMS sensor expansion board for the STM32 Nucleo.
It is equipped with Arduino UNO R3 connector layout, and is designed around the ISM6HG256X 3D accelerometer and 3D gyroscope, 
the ISM330IS 3D accelerometer and 3D gyroscope with ISPU, the IIS2DULPX 3D accelerometer, the IIS2MDC 3D magnetometer, 
and the ILPS22QS pressure sensor.
The X-NUCLEO-IKS5A1 interfaces with the STM32 microcontroller or the Arduino boards via the I²C pin.

# Examples

There are several examples with the X-NUCLEO-IKS5A1 library.
* X_NUCLEO_IKS5A1_HelloWorld: This application provides a simple example of usage of the X-NUCLEO-IKS5A1 
Expansion Board. It shows how to display on a hyperterminal the values of all on-board industrial MEMS sensors.
* X_NUCLEO_IKS5A1_IIS2DULPX_6D_Orientation: This application shows how to use IIS2DULPX sensor to find out the 6D orientation and display data on a hyperterminal.
* X_NUCLEO_IKS5A1_IIS2DULPX_Wake_Up_Detection: This application shows how to detect the wake-up event using the IIS2DULPX sensor.
* X_NUCLEO_IKS5A1_ISM6HG256X_6D_Orientation: This application shows how to use ISM6HG256X to find out the 6D orientation and display data on a hyperterminal.
* X_NUCLEO_IKS5A1_ISM6HG256X_Double_Tap_Detection: This application shows how to detect the double tap event using the ISM6HG256X.
* X_NUCLEO_IKS5A1_ISM6HG256X_FIFO_Polling: This application shows how to get data from FIFO in pooling mode and print them on terminal.
* X_NUCLEO_IKS5A1_ISM6HG256X_FIFO_Interrupt: This application shows how to get data from FIFO using interrupt and print them on terminal.
* X_NUCLEO_IKS5A1_ISM6HG256X_Free_Fall_Detection: This application shows how to detect the free fall event using the ISM6HG256X.
* X_NUCLEO_IKS5A1_ISM6HG256X_Pedometer: This application shows how to use ISM6HG256X to count steps.
* X_NUCLEO_IKS5A1_ISM6HG256X_Single_Tap_Detection: This application shows how to detect the single tap event using the ISM6HG256X.
* X_NUCLEO_IKS5A1_ISM6HG256X_Tilt_Detection: This application shows how to detect the tilt event using the ISM6HG256X.
* X_NUCLEO_IKS5A1_ISM6HG256X_Wake_Up_Detection: This application shows how to detect the wake-up event using the ISM6HG256X.
* X_NUCLEO_IKS5A1_ISM330IS_ISPU_Sensor_Fusion: This application implements the sensor fusion of the accelerometer and gyroscope,
configured in high-performance mode at 104 Hz. The configuration generates an interrupt on INT1 when the quaternion for the new sample 
is computed and available in the output registers.
* X_NUCLEO_IKS5A1_ISM330IS_ISPU_Tap: This application implements the tap detection solution based on the accelerometer data. 
The configuration generates an interrupt on INT1 when the tap event for the new sample is computed and available in the output registers.

# Dependencies

The X-NUCLEO-IKS5A1 library requires the following STM32duino libraries:

* STM32duino ISM6HG256X: https://github.com/stm32duino/ISM6HG256X
* STM32duino ISM330IS: https://github.com/stm32duino/ISM330IS
* STM32duino IIS2DULPX: https://github.com/stm32duino/IIS2DULPX
* STM32duino IIS2MDC: https://github.com/stm32duino/IIS2MDC
* STM32duino ILPS22QS: https://github.com/stm32duino/ILPS22QS

## Documentation

You can find the source files at  
https://github.com/stm32duino/X-NUCLEO-IKS5A1

The X-NUCLEO-IKS5A1 datasheet is available at  
https://www.st.com/content/st_com/en/products/evaluation-tools/product-evaluation-tools/stm32-nucleo-expansion-boards/x-nucleo-iks5a1.html
