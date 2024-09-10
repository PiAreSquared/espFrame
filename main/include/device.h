#ifndef DEVICE_H
#define DEVICE_H

#ifdef __cplusplus
extern "C" {
#endif
// START OF FILE: src/device.h
#define INCLUDE_vTaskDelay 1

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "soc/gpio_struct.h"
#include "driver/gpio.h"


// Type definitions
#define UBYTE           uint8_t
#define UWORD           uint16_t
#define UDOUBLE         uint32_t
#define UDOUBLEDOUBLE   uint64_t

// GPIO consts
#define SCK_PIN  GPIO_NUM_13
#define MOSI_PIN GPIO_NUM_14
#define CS_PIN   GPIO_NUM_15
#define BUSY_PIN GPIO_NUM_25
#define RST_PIN  GPIO_NUM_26
#define DC_PIN   GPIO_NUM_27

#define GPIO_PIN_SET   1
#define GPIO_PIN_RESET 0

#define PANEL_SETTING                               0x00
#define POWER_SETTING                               0x01
#define POWER_OFF                                   0x02
#define POWER_OFF_SEQUENCE_SETTING                  0x03
#define POWER_ON                                    0x04
#define POWER_ON_MEASURE                            0x05
#define BOOSTER_SOFT_START                          0x06
#define DEEP_SLEEP                                  0x07
#define DATA_START_TRANSMISSION_1                   0x10
#define DATA_STOP                                   0x11
#define DISPLAY_REFRESH                             0x12
#define DATA_START_TRANSMISSION_2                   0x13
#define DUAL_SPI                                    0x15
#define LUT_FOR_VCOM                                0x20 
#define LUT_W2W                                     0x21
#define LUT_B2W                                     0x22
#define LUT_W2B                                     0x23
#define LUT_B2B                                     0x24
// #define LUT_RED_0                                   0x25
// #define LUT_RED_1                                   0x26
// #define LUT_RED_2                                   0x27
// #define LUT_RED_3                                   0x28
// #define LUT_XON                                     0x29
#define PLL_CONTROL                                 0x30
#define TEMPERATURE_CALIBRATION                     0x40
#define TEMPERATURE_SENSOR_SELECT_COMMAND           0x41
#define TEMPERATURE_SENSOR_WRITE                    0x42
#define TEMPERATURE_SENSOR_READ                     0x43
#define VCOM_AND_DATA_INTERVAL_SETTING              0x50
#define LOW_POWER_DETECTION                         0x51
#define TCON_SETTING                                0x60
#define TCON_RESOLUTION                             0x61
#define GATE_SOURCE_START_SETTING                   0x65
#define REVISION                                    0x70
#define GET_STATUS                                  0x71
#define AUTO_MEASUREMENT_VCOM                       0x80
#define READ_VCOM_VALUE                             0x81
#define VCM_DC_SETTING                              0x82

#define DISP_HEIGHT 480
#define DISP_WIDTH  800

class SPIDevice {
    public:
    SPIDevice(void);
    ~SPIDevice(void);

    static int DeviceInit(void);
    static void DigitalWrite(gpio_num_t pin, UBYTE value);
    static UBYTE DigitalRead(gpio_num_t pin);
    static void Delay(UDOUBLE ms);
    static void SpiTransfer(UBYTE data);
};


enum Color {
    BLACK,
    WHITE
};

enum DeviceInitMode {
    NORMAL,
    FAST,
    PARTIAL,
};

class Device : public SPIDevice {
    public:
        Device(void);
        ~Device(void);

        UBYTE Init(DeviceInitMode mode = NORMAL);
        void SendCommand(UBYTE command);
        void SendData(UBYTE data);
        void WaitUntilIdle(void);
        void Reset(void);
        void RefreshScreen(void);
        void ClearFrame(Color color = Color::WHITE);
        void DisplayFrame(const UBYTE* frame_buffer_black);
        void DisplayWindow(const UBYTE* frame_buffer_black, UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end);
        void Sleep(void);

    private:
        gpio_num_t reset_pin;
        gpio_num_t dc_pin;
        gpio_num_t cs_pin;
        gpio_num_t busy_pin;
};


// END OF FILE: src/device.h
#ifdef __cplusplus
}
#endif

#endif // DEVICE_H