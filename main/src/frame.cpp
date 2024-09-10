#include "frame.h"

Frame::Frame(void) {
    device.DeviceInit();
    device.Init();

    InitScreen();
}

Frame::~Frame(void) {
    display.ClearFrame(BACKGROUND_COLOR);
    display.~Display();
    device.~Device();

    display = NULL;
}

Point Frame::WriteString(int x, int y, const char* text, font_weight_t font_weight) {
    const UBYTE* ubyteStr = reinterpret_cast<const UBYTE*>(text);

    switch (font_weight) {
        case LIGHT:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_Light20);
        case REGULAR:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_SemiBold36);
        case SEMI_BOLD:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_SemiBold36);
        case BOLD:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_Bold128);
        default:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_SemiBold36);
    }
}

Point Frame::WriteStringVertCentered(int x, int y, const char* text, font_weight_t font_weight) {
    const UBYTE* ubyteStr = reinterpret_cast<const UBYTE*>(text);

    // count length of string in pixels
    int length = 0;
    switch (font_weight) {
        case LIGHT:
            for (int i = 0; i < strlen(text); i++)
                length += Nunito_Light20.widths[ubyteStr[i] - START_CHAR];
            break;
        case REGULAR:
            for (int i = 0; i < strlen(text); i++)
                length += Nunito_SemiBold36.widths[ubyteStr[i] - START_CHAR];
            break;    
        case SEMI_BOLD:
            for (int i = 0; i < strlen(text); i++)
                length += Nunito_SemiBold36.widths[ubyteStr[i] - START_CHAR];
            break;
        case BOLD:
            for (int i = 0; i < strlen(text); i++)
                    length += Nunito_Bold128.widths[ubyteStr[i] - START_CHAR];
            break;
        default:
            for (int i = 0; i < strlen(text); i++)
                    length += Nunito_SemiBold36.widths[ubyteStr[i] - START_CHAR];
    }

    // calculate x position to center text
    x -= length / 2;

    switch (font_weight) {
        case LIGHT:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_Light20);
        case REGULAR:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_SemiBold36);
        case SEMI_BOLD:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_SemiBold36);
        case BOLD:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_Bold128);
        default:
            return display.DrawString(x, y, ubyteStr, FOREGROUND_COLOR, BACKGROUND_COLOR, &Nunito_SemiBold36);
    }
}

Point Frame::DrawGraphic(int x, int y, const graphic_t* graphic) {
    Point p = {x, y};
    display.DrawGraphic(x, y, FOREGROUND_COLOR, BACKGROUND_COLOR, graphic);
    p.x += graphic->size;
    p.y += graphic->size;

    return p;
}

void Frame::InitScreen(void) {
    Point p1, p2;
    display.ClearFrame(BACKGROUND_COLOR);
    display.RotateDisplay(ROTATION_270);

    // WriteString(148, 63, "WEATHER", SEMI_BOLD);
    WriteStringVertCentered(240, 63, "WEATHER", SEMI_BOLD);
    DrawGraphic(44, 145, &sunny_icon150);
    WriteStringVertCentered(335, 115, "74", BOLD);
    p1 = WriteStringVertCentered(340 - 15, 267, "37 MIN TILL", LIGHT);
    DrawGraphic(p1.x + 6, 267, &sunset_icon25);

    p1 = DrawGraphic(44, 360, &rain_icon50);
    p2 = WriteString(p1.x + 12, 360, "71", SEMI_BOLD);
    WriteString(75, p1.y, "4 PM", LIGHT);

    WriteString(177, 497, "24 BUS", SEMI_BOLD);
    WriteString(182, 538, "TO CAMPUS", LIGHT);

    display.RefreshScreen();
}




// {
//     // const UBYTE ubyteStr[] = {0x6C, 0x00};
//     const UBYTE ubyteStr[] = {0x48, 0x65, 0x6C, 0x6C, 0x6F, 0x20, 0x57, 0x6F, 0x72, 0x6C, 0x64, 0x21, 0x00};


//     display.ClearFrame(FOREGROUND_COLOR);
//     display.RotateDisplay(ROTATION_270);
//     display.DrawString(20, 20, ubyteStr, BACKGROUND_COLOR, FOREGROUND_COLOR, &Varela64);


//     display.RefreshScreen();
//     device.Delay(10 * 1000);

//     display.ClearFrame(FOREGROUND_COLOR);
//     display.RefreshScreen();
// }