//
// Created by wastl on 19.01.22.
//

#ifndef WEATHER_EXPORTER_WEATHER_INFLUX_H
#define WEATHER_EXPORTER_WEATHER_INFLUX_H
#include <string>
#include "weather_listener.h"

namespace wastlnet {
    namespace weather {
        std::function<void(const weather_data&)> build_influx_writer(const std::string &server, int port, const std::string &db);
    }
}


#endif //WEATHER_EXPORTER_WEATHER_INFLUX_H
