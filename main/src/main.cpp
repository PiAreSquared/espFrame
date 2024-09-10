#include "frame.h"
#include "nvs_flash.h"

// void test_disp() {
//     Device device;
//     device.DeviceInit();
//     device.Init();
//     Display display = Display(&device);
    
//     // const UBYTE ubyteStr[] = {0x6C, 0x00};
//     const UBYTE ubyteStr[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00};

//     ESP_LOGI("DISPLAY", "Display initialized");

//     display.ClearFrame(Color::WHITE);
//     ESP_LOGI("DISPLAY", "Frame cleared");
//     display.RotateDisplay(ROTATION_270);
//     display.DrawString(20, 20, ubyteStr, Color::BLACK, Color::WHITE, &Varela64);
//     ESP_LOGI("DISPLAY", "New frame created");

//     // display.SetPixel(0, 0, Color::BLACK);

//     display.RefreshScreen();
//     ESP_LOGI("DISPLAY", "New frame displaying");
//     device.Delay(10 * 1000);
//     display.ClearFrame(Color::WHITE);
//     ESP_LOGI("DISPLAY", "Clearing Frame");
//     display.RefreshScreen();
//     ESP_LOGI("DISPLAY", "Frame Cleared 2");

// }

extern "C" void app_main(void) {
    ESP_ERROR_CHECK( nvs_flash_init() );
//     test_disp();
// }

// void test_disp() {
    Frame frame;

    frame.device.Delay(10 * 1000);

    // frame.~Frame();
}
