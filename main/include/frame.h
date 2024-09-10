#ifndef FRAME_H
#define FRAME_H

#ifdef __cplusplus
extern "C" {
#endif
// START OF FILE: src/frame.h
#include "display.h"

#define BACKGROUND_COLOR Color::BLACK
#define FOREGROUND_COLOR Color::WHITE

enum WeatherType {
    SUNRISE,
    SUNSET,

    SUNNY,
    WINDY,

    PARTLY_CLOUDY,
    CLOUDY,

    RAINY,
    HEAVY_RAINY,
    THUNDERSTORM,

    SNOWY,
    HEAVY_SNOWY,

    FOGGY,
    UNKNOWN
};

enum RisingType {
    RISING,
    FALLING,
    STEADY
};

typedef struct {
    WeatherType weather_type;
    UBYTE time;
    float temp;
} WeatherForecast;

typedef struct {
    WeatherType weather_type;
    RisingType temp_rising;
    float current_temp;
    float high_temp;
    float low_temp;
    float feels_like_temp;

} WeatherInfo;

typedef enum _font_weight_t {
    LIGHT,
    REGULAR,
    SEMI_BOLD,
    BOLD
} font_weight_t;

class Frame {
    public:
        Frame(void);
        ~Frame(void);

        void InitScreen(void);
        void SetWeatherInfo(const char* weather_info);
        Point WriteString(int x, int y, const char* text, font_weight_t font_weight);
        Point WriteStringVertCentered(int x, int y, const char *text, font_weight_t font_weight);
        Point DrawGraphic(int x, int y, const graphic_t *graphic);

        // private:
        Device device;
        Display display = Display(&device);
        UBYTE* frame_buffer;
        Rotation angle_rotation;
};


// END OF FILE: src/frame.h
#ifdef __cplusplus
}
#endif

#endif // FRAME_H