# Weather Station Exporter

This is a simple tool written in C++ listen to a weather station for
weather data and writes it to an InfluxDB time series database for further analysis
(e.g. in Grafana).

I use it with my Bresser 7-in-1 ClearView weather station and redirect traffic that is meant
for a cloud service to this tool. Note that Bresser weather stations can only send traffic to
port 80 of a server, so I am using a transparent proxy redirect in Apache to forward traffic
to this tool listening on a different port.

I use the following statement in the apache configuration, assuming your service is listening
on IP 192.168.100.55 and port 41001:

    ProxyPass "/weatherstation/updateweatherstation.php" "http://192.168.100.55:41001/post"

The tool only parses the query arguments and ignores the rest of the call URL.
