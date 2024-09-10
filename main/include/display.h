#ifndef DISPLAY_H
#define DISPLAY_H


#ifdef __cplusplus
extern "C" {
#endif
// START OF FILE: src/display.h
#include <stdlib.h>
#include <stdio.h>
#include "fonts.h"
#include "graphics.h"
#include "device.h"

// Type definitions
#define UBYTE           uint8_t
#define UWORD           uint16_t
#define UDOUBLE         uint32_t
#define UDOUBLEDOUBLE   uint64_t

enum Rotation {
    ROTATION_0,
    ROTATION_90,
    ROTATION_180,
    ROTATION_270
};

struct Point {
    int x;
    int y;
};


class Display {
    public:
        Display(Device* device);
        ~Display();
        void ClearFrame(Color color);
        void RefreshScreen();
        void RefreshScreen(UWORD x_start, UWORD y_start, UWORD x_end, UWORD y_end);
        void RotateDisplay(Rotation angle);

        void SetPixel(int x, int y, Color color);
        void DrawChar(int x, int y, UBYTE c, Color color_fg, Color color_bg, const font_t* font);
        void DrawGraphic(int x, int y, Color color_fg, Color color_bg, const graphic_t* graphic);
        Point DrawString(int x, int y, const UBYTE* str, Color color_fg, Color color_bg, const font_t* font);

    private:
        UBYTE* frame_buffer;
        Device* device_ref;
        Rotation angle_rotation;
};




// END OF FILE: src/display.h
#ifdef __cplusplus
}
#endif

#endif // DISPLAY_H