//
// Created by wastl on 19.01.22.
//
#include <chrono>
#include <thread>
#include <cpprest/http_msg.h>
#include <cpprest/http_listener.h>
#include <glog/logging.h>

#include "weather_listener.h"

double fahrenheit2celsius(double fahrenheit) {
    return 5 * (fahrenheit - 32) / 9;
}

double mph2ms(double mph) {
    double kmh = mph / 0.6213712;
    return kmh * 1000 / 3600;
}

double inch2mm(double inch) {
    return inch / 0.03937007874;
}

using web::http::http_request;
using web::http::experimental::listener::http_listener;
void start_listener(const std::string& uri, const std::function<void(const weather_data&)>& handler) {
    auto listener = http_listener(uri);

    listener.support([=](http_request request){
        auto uri = request.relative_uri();
        auto q = web::uri::split_query(uri.query());

        LOG(INFO) << "Wetterdaten erhalten";

        try {
            weather_data data;
            data.uv = std::stod(q["UV"]);
            data.barometer = inch2mm(std::stod(q["baromin"])) * 1.33322;
            data.dailyrain = inch2mm(std::stod(q["dailyrainin"]));
            data.dewpoint = fahrenheit2celsius(std::stod(q["dewptf"]));
            data.humidity = std::stod(q["humidity"]);
            data.indoorhumidity = std::stod(q["indoorhumidity"]);
            data.indoortemp = fahrenheit2celsius(std::stod(q["indoortempf"]));
            data.rain = inch2mm(std::stod(q["rainin"]));
            data.solarradiation = std::stod(q["solarradiation"]);
            data.temp = fahrenheit2celsius(std::stod(q["tempf"]));
            data.winddir = std::stoi(q["winddir"]);
            data.windgust = mph2ms(std::stod(q["windgustmph"]));
            data.windspeed = mph2ms(std::stod(q["windspeedmph"]));

            handler(data);
        } catch(const std::exception &e) {
            LOG(ERROR) << "Error while logging data: " << e.what();
        }
    });

    listener.open().get();

    using namespace std::chrono_literals;
    while(true) {
        std::this_thread::sleep_for(1s);
    }
}