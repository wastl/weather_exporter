#include <iostream>
#include <yaml-cpp/yaml.h>

#include "weather_listener.h"
#include "weather_influx.h"

class config {
public:
    config() {}

    // weather configuration
    std::string listen_server;

    // Influx configuration
    std::string influx_server = "127.0.0.1";
    int influx_port = 8086;
    std::string influx_db = "senec";
    std::string influx_user = "";
    std::string influx_password = "";
};

config* load_config(const std::string& file) {
    YAML::Node yaml = YAML::LoadFile(file);

    config* c = new config();
    if (!yaml["listen_server"]) {
        std::cerr << "Listen Server nicht konfiguriert" << std::endl;
        return c;
    }
    c->listen_server = yaml["listen_server"].as<std::string>();

    if (yaml["influx_server"]) {
        c->influx_server = yaml["influx_server"].as<std::string>();
    }
    if (yaml["influx_db"]) {
        c->influx_db = yaml["influx_db"].as<std::string>();
    }
    if (yaml["influx_port"]) {
        c->influx_port = yaml["influx_port"].as<int>();
    }
    if (yaml["influx_user"]) {
        c->influx_user = yaml["influx_user"].as<std::string>();
    }
    if (yaml["influx_password"]) {
        c->influx_password = yaml["influx_password"].as<std::string>();
    }

    return c;
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: weather_exporter <path-to-config.yml>" << std::endl;
        return 1;
    }

    config* cfg = load_config(argv[1]);
    std::cout << "Listen Address: " << cfg->listen_server << std::endl;
    std::cout << "Influx Server: " << cfg->influx_server << std::endl;

    auto handler = wastlnet::weather::build_influx_writer(cfg->influx_server, cfg->influx_port, cfg->influx_db);

    start_listener(cfg->listen_server, [=](const weather_data& data) {
        std::cout << "Temperatur:       " << data.temp << "°C" << std::endl;
        std::cout << "Luftfeuchtigkeit: " << data.humidity << "%" << std::endl;
        std::cout << "Luftdruck:        " << data.barometer << "mm" << std::endl;
        std::cout << std::endl;

        handler(data);
    });

    return 0;
}
