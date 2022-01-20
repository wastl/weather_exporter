//
// Created by wastl on 19.01.22.
//
#include <chrono>
#include <influxdb.hpp>
#include <iostream>
#include <glog/logging.h>

#include "weather_influx.h"

using namespace std::chrono;

std::function<void(const weather_data &)>
wastlnet::weather::build_influx_writer(const std::string &server, int port, const std::string &db) {
    return [=](const weather_data& data) {
        long long ts = time_point_cast<nanoseconds>(system_clock::now()).time_since_epoch().count();

        std::string response;

        influxdb_cpp::server_info si(server, port, db);
        int result = influxdb_cpp::builder()
                .meas("weather")
                .field("temperature", data.temp)
                .field("barometer", data.barometer)
                .field("humidity", data.humidity)
                .field("dewpoint", data.dewpoint)
                .field("rain", data.rain)
                .field("dailyrain", data.dailyrain)
                .timestamp(ts)
                .post_http(si, &response);

        if (result != 0) {
            LOG(ERROR) << "Error writing to Influx DB: " << response;
        }

        result = influxdb_cpp::builder()
                .meas("indoor")
                .field("temperature", data.indoortemp)
                .field("humidity", data.indoorhumidity)
                .timestamp(ts)
                .post_http(si, &response);

        if (result != 0) {
            LOG(ERROR) << "Error writing to Influx DB: " << response;
        }

        result = influxdb_cpp::builder()
                .meas("wind")
                .field("direction", data.winddir)
                .field("speed", data.windspeed)
                .field("gusts", data.windgust)
                .timestamp(ts)
                .post_http(si, &response);

        if (result != 0) {
            LOG(ERROR) << "Error writing to Influx DB: " << response;
        }

        result = influxdb_cpp::builder()
                .meas("solar")
                .field("radiation", data.solarradiation)
                .field("uv", data.uv)
                .timestamp(ts)
                .post_http(si, &response);

        if (result != 0) {
            LOG(ERROR) << "Error writing to Influx DB: " << response;
        }
    };
}
