#include "display.h"
#include "esp_log.h"

Display::Display(Device* device){
    frame_buffer = (UBYTE*)malloc(DISP_WIDTH / 8 * DISP_HEIGHT);
    device_ref = device;
    if (frame_buffer == NULL) {
        ESP_LOGE("DISPLAY", "Failed to allocate memory for frame buffer");
    }
}

Display::~Display() {
    free(frame_buffer);
    device_ref = NULL;
}

void Display::ClearFrame(Color color) {
    UWORD width, height, i;
    width = (DISP_WIDTH % 8 == 0) ? (DISP_WIDTH / 8) : (DISP_WIDTH / 8 + 1);
    height = DISP_HEIGHT;

    for (i = 0; i < width * height; i++) {
        frame_buffer[i] = color == WHITE ? 0x00 : 0xFF;
    }
    device_ref->ClearFrame(color);
}

void Display::RotateDisplay(Rotation angle) {
    angle_rotation = angle;
}

void Display::SetPixel(int x_, int y_, Color color) {

    int x, y;

    // rotate if necessary
    switch (angle_rotation) {
        case ROTATION_0:
            x = x_;
            y = y_;
            break;
        case ROTATION_90:
            x = DISP_WIDTH - y_ - 1;
            y = x_;
            break;
        case ROTATION_180:
            x = DISP_WIDTH - x_ - 1;
            y = DISP_HEIGHT - y_ - 1;
            break;
        case ROTATION_270:
            x = y_;
            y = DISP_HEIGHT - x_ - 1;
            break;
        default:
            x = x_;
            y = y_;
            break;
    }

    if (x < 0 || x >= DISP_WIDTH || y < 0 || y >= DISP_HEIGHT) {
        ESP_LOGE("DISPLAY", "Invalid pixel coordinates");
        return;
    }

    UWORD width = DISP_WIDTH / 8 + (DISP_WIDTH % 8 ? 1 : 0);

    if (color == WHITE) {
        frame_buffer[x / 8 + y * width] &= ~(0x80 >> (x % 8)); // 1 --> 0
        // ESP_LOGI("DISPLAY", "Setting pixel at %d  %d to BLACK", x / 8 + y * width, x % 8);
    } else {
        frame_buffer[x / 8 + y * width] |= (0x80 >> (x % 8));  // 0 --> 1
        // ESP_LOGI("DISPLAY", "Setting pixel at %d  %d to WHITE", x / 8 + y * width, x % 8);
    }
}

void Display::DrawChar(int x, int y, UBYTE c, Color color_fg, Color color_bg, const font_t* font) {
    if (font == NULL) {
        ESP_LOGE("DISPLAY", "Font is NULL");
        return;
    }

    if (c < START_CHAR || c > END_CHAR) {
        ESP_LOGE("DISPLAY", "Character not in font");
        return;
    }

    const UBYTE* bitmap = &font->data[(((char)c - START_CHAR) * font->height * (font->max_width / 8 + (font->max_width % 8 ? 1 : 0)))];
    // ESP_LOGI("DISPLAY", "Bitmap: jumping %d indices", (((char)c - START_CHAR) * font->height * (font->max_width / 8 + (font->max_width % 8 ? 1 : 0))));
    UBYTE width = font->widths[(c - START_CHAR)];
    UBYTE max_width = font->max_width / 8 + (font->max_width % 8 ? 1 : 0);
    UBYTE i, j;

    for (i = 0; i < font->height; i++) {
        for (j = 0; j < width; j++) {
            if (j % 8 == 0) {
                // ESP_LOGI("DISPLAY", "New byte row %d col %d: %x (%d)", i, j, bitmap[i * max_width + j / 8], i * max_width + j / 8);
            }
            if (bitmap[i * max_width + j / 8] & (0x80 >> (j % 8))) {
                SetPixel(x + j, y + i, color_fg);
            } else {
                SetPixel(x + j, y + i, color_bg);
            }
        }
    }
    ESP_LOGI("DISPLAY", "Character drawn at %d, %d with dim %d, %d", x, y, width, font->height);
}

void Display::DrawGraphic(int x, int y, Color color_fg, Color color_bg, const graphic_t* graphic) {
    if (graphic == NULL) {
        ESP_LOGE("DISPLAY", "Graphic is NULL");
        return;
    }

    UBYTE size = graphic->size;
    UBYTE max_width = size / 8 + (size % 8 ? 1 : 0);
    UBYTE i, j;

    for (i = 0; i < size; i++) {
        for (j = 0; j < size; j++) {
            if (graphic->data[i * max_width + j / 8] & (0x80 >> (j % 8))) {
                SetPixel(x + j, y + i, color_fg);
            } else {
                SetPixel(x + j, y + i, color_bg);
            }
        }
    }
}

Point Display::DrawString(int x, int y, const UBYTE* str, Color color_fg, Color color_bg, const font_t* font) {
    if (font == NULL) {
        ESP_LOGE("DISPLAY", "Font is NULL");
        return {x, y};
    }

    Point p = {x, y};
    while (*str) {
        DrawChar(p.x, p.y, *str, color_fg, color_bg, font);
        ESP_LOGI("DISPLAY", "Drawing character %c at %d, %d", *str, p.x, p.y);
        p.x += font->widths[*str - START_CHAR];
        str++;
    }

    return p;
}

void Display::RefreshScreen() {
    device_ref->DisplayFrame(frame_buffer);
    device_ref->RefreshScreen();
}