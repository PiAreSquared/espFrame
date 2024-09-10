#ifndef FONTS_H
#define FONTS_H

#ifdef __cplusplus
extern "C" {
#endif
// START OF FILE: src/fonts.h
#include <stdint.h>

#define START_CHAR ' '
#define END_CHAR '~'    

typedef struct {
    const uint8_t* data;
    const uint8_t* widths;
    const uint8_t max_width;
    const uint8_t height;
} font_t;


extern font_t Varela64;
extern font_t Nunito_SemiBold36;
extern font_t Nunito_Bold128;
extern font_t Nunito_Bold64;
extern font_t Nunito_Light20;


// END OF FILE: src/fonts.h
#ifdef __cplusplus
}
#endif

#endif // FONTS_H