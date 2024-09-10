#ifndef GRAPHICS_H
#define GRAPHICS_H

#ifdef __cplusplus
extern "C" {
#endif
// START OF FILE: src/graphics.h
#include <stdint.h>

typedef struct {
    const uint8_t* data;
    const uint8_t size;
} graphic_t;


extern graphic_t thunderstorm_icon50;
extern graphic_t thunderstorm_icon150;
extern graphic_t cloudy_icon50;
extern graphic_t cloudy_icon150;
extern graphic_t sunrise_icon25;
extern graphic_t sunrise_icon50;
extern graphic_t sunrise_icon150;
extern graphic_t windy_icon50;
extern graphic_t windy_icon150;
extern graphic_t partly_cloudy_icon50;
extern graphic_t partly_cloudy_icon150;
extern graphic_t heavy_rain_icon50;
extern graphic_t heavy_rain_icon150;
extern graphic_t foggy_icon50;
extern graphic_t foggy_icon150;
extern graphic_t sunny_icon50;
extern graphic_t sunny_icon150;
extern graphic_t rain_icon50;
extern graphic_t rain_icon150;
extern graphic_t snow_icon50;
extern graphic_t snow_icon150;
extern graphic_t sunset_icon25;
extern graphic_t sunset_icon50;
extern graphic_t sunset_icon150;

// END OF FILE: src/graphics.h
#ifdef __cplusplus
}
#endif

#endif // GRAPHICS_H