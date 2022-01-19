//
// Created by wastl on 19.01.22.
//

#ifndef WEATHER_EXPORTER_WEATHER_LISTENER_H
#define WEATHER_EXPORTER_WEATHER_LISTENER_H

#include <functional>

class weather_data {
public:
    weather_data() {}

    double uv;
    double barometer;       // hPa
    double dailyrain;       // mm
    double dewpoint;        // °C
    double humidity;        // %
    double indoorhumidity;  // %
    double indoortemp;      // °C
    double rain;            // mm
    double solarradiation;  // W
    double temp;            // °C
    int winddir;            // °
    double windgust;        // mm/s
    double windspeed;       // mm/s
};

void start_listener(const std::string& uri, const std::function<void(const weather_data&)>& handler);

#endif //WEATHER_EXPORTER_WEATHER_LISTENER_H
