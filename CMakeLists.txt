cmake_minimum_required(VERSION 3.21)
project(weather_exporter)

set(CMAKE_CXX_STANDARD 14)

add_executable(weather_exporter main.cpp weather_listener.cpp weather_listener.h weather_influx.cpp weather_influx.h)
target_link_libraries(weather_exporter PUBLIC cpprest pthread ssl crypto yaml-cpp glog)

