#include "device.h"
#include "esp_log.h"

UBYTE Voltage_Frame[]={
	0x6, 0x3F, 0x3F, 0x11, 0x24, 0x7, 0x17,
};


static spi_device_handle_t spi;

SPIDevice::SPIDevice(void) {

}

SPIDevice::~SPIDevice(void) {

}

int SPIDevice::DeviceInit(void) {
    // spi_bus_free(HSPI_HOST);

    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = ((uint64_t)1<<(uint64_t)DC_PIN) | ((uint64_t)1<<(uint64_t)RST_PIN);
    io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    ESP_ERROR_CHECK(gpio_config(&io_conf));

    gpio_config_t i_conf;
    i_conf.intr_type = GPIO_INTR_DISABLE;
    i_conf.mode = GPIO_MODE_INPUT;
    i_conf.pin_bit_mask = ((uint64_t)1<<(uint64_t)BUSY_PIN);
    i_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    i_conf.pull_up_en = GPIO_PULLUP_DISABLE;
    ESP_ERROR_CHECK(gpio_config(&i_conf));

    // gpio_config_t c_conf;
    // c_conf.intr_type = GPIO_INTR_DISABLE;
    // c_conf.mode = GPIO_MODE_OUTPUT;
    // c_conf.pin_bit_mask = ((uint64_t)1<<(uint64_t)CS_PIN | (uint64_t)1<<(uint64_t)SCK_PIN | (uint64_t)1<<(uint64_t)MOSI_PIN);
    // c_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;
    // c_conf.pull_up_en = GPIO_PULLUP_ENABLE;
    // ESP_ERROR_CHECK(gpio_config(&c_conf));

    // gpio_set_direction(DC_PIN, GPIO_MODE_OUTPUT);
    // gpio_set_direction(RST_PIN, GPIO_MODE_OUTPUT);
    // gpio_set_direction(BUSY_PIN, GPIO_MODE_INPUT); 

    esp_err_t ret;

    spi_bus_config_t buscfg;
    memset(&buscfg, 0, sizeof(buscfg));
    buscfg.mosi_io_num = MOSI_PIN;
    buscfg.sclk_io_num = SCK_PIN;
    buscfg.miso_io_num = -1;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;

    //Initialize the SPI bus
    ret=spi_bus_initialize(HSPI_HOST, &buscfg, 0);
    switch (ret) {
        case ESP_ERR_INVALID_ARG:
            ESP_LOGE("EPDIF", "INVALID ARG");
            break;
        case ESP_ERR_INVALID_STATE:
            ESP_LOGE("EPDIF", "INVALID STATE");
            break;
        case ESP_ERR_NO_MEM:
            ESP_LOGE("EPDIF", "INVALID NO MEMORY");
            break;
        case ESP_OK:
            ESP_LOGE("EPDIF", "All OK");
    }
    assert(ret==ESP_OK);

    spi_device_interface_config_t devcfg;
    memset(&devcfg, 0, sizeof(devcfg));
    devcfg.command_bits = 0;
    devcfg.address_bits = 0;
    devcfg.dummy_bits = 0;
    devcfg.clock_speed_hz = 2*1000*1000;
    devcfg.mode = 0;
    devcfg.spics_io_num = CS_PIN;
    devcfg.queue_size = 1;

    //Attach the EPD to the SPI bus
    ret=spi_bus_add_device(HSPI_HOST, &devcfg, &spi);
    assert(ret==ESP_OK);

    return 0;
}

void SPIDevice::SpiTransfer(unsigned char data) {
    esp_err_t ret;
    spi_transaction_t t;
    memset(&t, 0, sizeof(t));       //Zero out the transaction
    t.flags = SPI_TRANS_USE_TXDATA;
    t.length = 8;        // transaction length is in bits
    t.tx_data[0] = data;
    t.tx_data[1] = data;
    t.tx_data[2] = data;
    t.tx_data[3] = data;
    ret = spi_device_transmit(spi, &t);  //Transmit!
    assert(ret==ESP_OK);            //Should have had no issues.
}


void SPIDevice::Delay(unsigned int delaytime_in_ms) {
    vTaskDelay(delaytime_in_ms / portTICK_PERIOD_MS);
}


void SPIDevice::DigitalWrite(gpio_num_t pin, UBYTE value) {
    // ESP_LOGI("EPDIF", "Set Pin %i: %i", pin, value);
    gpio_set_level(pin, value);
}


UBYTE SPIDevice::DigitalRead(gpio_num_t pin) {
    int level = gpio_get_level(pin);
    return level;
}

Device::Device(void) {
    reset_pin = RST_PIN;
    dc_pin = DC_PIN;
    cs_pin = CS_PIN;
    busy_pin = BUSY_PIN;
}

Device::~Device(void) {

}

UBYTE Device::Init(DeviceInitMode mode) {
    Reset();

    if (mode == NORMAL) {
        SendCommand(POWER_SETTING);
            SendData(0x07); // BD_EN = 0, VSR_EN = 1, VS_EN = 1, VG_EN = 1
            SendData(0x07); // VGH = 20V, VDH = 20V
            SendData(0x3A); // VDH = +14V
            SendData(0x3A); // VDL = -14V

        SendCommand(VCM_DC_SETTING);
            SendData(0x24); // VCOM = -1.9V

        SendCommand(BOOSTER_SOFT_START);
            SendData(0x27); // phase A: 10ms period, strength 5, 6.58ms min off
            SendData(0x27); // phase B: 10ms period, strength 5, 6.58ms min off
            SendData(0x2F); // phase C1: strength 6, 6.58ms min off
            SendData(0x17); // phase C2: disable

        SendCommand(POWER_ON);
        Delay(100);
        WaitUntilIdle();

        SendCommand(PANEL_SETTING);
            SendData(0x1F); // LUT from OTP, B/W mode
            // SendData(0x0F); // LUT from OTP, B/W/R mode
            // SendData(0x3F); // LUT from register, B/W mode

        SendCommand(TCON_RESOLUTION);
            SendData(0x03); // hres 800
            SendData(0x20); // hres ^^^
            SendData(0x01); // vres 480
            SendData(0xE0); // vres ^^^

        SendCommand(DUAL_SPI);
            SendData(0x00); // disable dual spi

        SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
            SendData(0x10); // LUTKW is used
            SendData(0x00); // 17 hsync VCOM and data interval

        SendCommand(TCON_SETTING);
            SendData(0x22); // gate scan set to 12 (default)

        SendCommand(GATE_SOURCE_START_SETTING);
            SendData(0x00); // start at 0
            SendData(0x00); // start at 0
            SendData(0x00); // start at 0
            SendData(0x00); // start at 0
    } else if (mode == FAST) {
        SendCommand(PANEL_SETTING);
            SendData(0x1F); // LUT from OTP, B/W mode
            // SendData(0x0F); // LUT from OTP, B/W/R mode
            // SendData(0x3F); // LUT from register, B/W mode

        SendCommand(VCOM_AND_DATA_INTERVAL_SETTING);
            SendData(0x10); // LUTKW is used
            SendData(0x00); // 17 hsync VCOM and data interval

        SendCommand(POWER_ON);
        Delay(100);
        WaitUntilIdle();

        SendCommand(BOOSTER_SOFT_START);
            SendData(0x27); // phase A: 10ms period, strength 5, 6.58ms min off
            SendData(0x27); // phase B: 10ms period, strength 5, 6.58ms min off
            SendData(0x2F); // phase C1: strength 6, 6.58ms min off
            SendData(0x17); // phase C2: disable

    } else if (mode == PARTIAL) {
        
    } else {
        return -1;
    }
    
    return 0;
}

void Device::SendCommand(unsigned char data) {
    DigitalWrite(dc_pin, 0);
    SpiTransfer(data);
}

void Device::SendData(unsigned char data) {
    DigitalWrite(dc_pin, 1);
    SpiTransfer(data);
}

void Device::Reset(void) {
    DigitalWrite(reset_pin, 1);
    Delay(200);
    DigitalWrite(reset_pin, 0);
    Delay(2); 
    DigitalWrite(reset_pin, 1);
    Delay(200);    
}

void Device::WaitUntilIdle(void) {
    do {
        SendCommand(GET_STATUS);
    } while(!(DigitalRead(busy_pin) & 0x01));

    Delay(200);
}

void Device::RefreshScreen(void) {
    SendCommand(DISPLAY_REFRESH);
    Delay(5);
    WaitUntilIdle();
}

// not even supported for V2?
// void Device::SetLutBw(void) {
//     UBYTE count;

// 	SendCommand(LUT_FOR_VCOM);
// 	for(count=0; count<60; count++) SendData(VCOM_LUT[count]);

// 	SendCommand(LUT_W2W);
// 	for(count=0; count<60; count++) SendData(W2W_LUT[count]);

// 	SendCommand(LUT_B2W);
// 	for(count=0; count<60; count++) SendData(B2W_LUT[count]);

// 	SendCommand(LUT_W2B);
// 	for(count=0; count<60; count++) SendData(W2B_LUT[count]);

// 	SendCommand(LUT_B2B);
// 	for(count=0; count<60; count++) SendData(B2B_LUT[count]);
// }

void Device::ClearFrame(Color color) {
    UWORD width, height, i;
    width = (DISP_WIDTH % 8 == 0) ? (DISP_WIDTH / 8 ): (DISP_WIDTH / 8 + 1);
    height = DISP_HEIGHT;

    SendCommand(DATA_START_TRANSMISSION_1);
    for (i = 0; i < width * height; i++) SendData(color == WHITE ? 0x00 : 0xFF);

    SendCommand(DATA_START_TRANSMISSION_2);
    for (i = 0; i < width * height; i++) SendData(color == WHITE ? 0xFF : 0x00);

    RefreshScreen();
}

void Device::DisplayFrame(const UBYTE* frame_buffer) {
    UWORD width, height, i, j;
    // width = (DISP_WIDTH % 8 == 0) ? (DISP_WIDTH / 8 ): (DISP_WIDTH / 8 + 1);
    width = DISP_WIDTH / 8;
    height = DISP_HEIGHT;

    SendCommand(DATA_START_TRANSMISSION_1);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            SendData(frame_buffer[j + i * width]);
        }
    }

    SendCommand(DATA_START_TRANSMISSION_2);
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {
            SendData(frame_buffer[j + i * width]);
        }
    }

    RefreshScreen();
}

void Device::DisplayWindow(const UBYTE* frame_buffer, UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end) {
    UWORD width, height, i;
    width = (x_end - x_start) / 8;
    width += (x_end - x_start) % 8 ? 1 : 0;
    height = y_end - y_start;



    RefreshScreen();
}

void Device::Sleep(void) {
    SendCommand(POWER_OFF);
    WaitUntilIdle();
    SendCommand(DEEP_SLEEP);
    SendData(0XA5);
}