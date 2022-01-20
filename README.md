# Weather Station Exporter

This is a simple tool written in C++ listen to a weather station for
weather data and writes it to an InfluxDB time series database for further analysis
(e.g. in Grafana).

## Setup (Bresser ClearView 7-in-1)

I use it with my Bresser 7-in-1 ClearView weather station and redirect traffic that is meant
for a cloud service to this tool. The following description gives a brief overview over the
necessary steps:


### Weather Station Configuration

The Bresser 7-in-1 ClearView weather station supports uploading data to some cloud services
like Weathercloud and Weather Underground, but unfortunately doesn't offer an API that allows
querying from inside the local network. So instead, you need to configure your own local
server as the cloud server in the Bresser configuration. In order to do this:

1. Switch the weather station to Access Point mode
2. Connect to the weather station WIFI and point your browser to the admin page at 
   192.168.1.1
3. Enter the address of your local server as destination server for the weather station
4. Submit the configuration, the weather station will automatically switch back to normal mode

Note that Bresser weather stations can only send traffic to port 80 of a server, so never
mind trying to specify a different port. I am using a transparent proxy redirect in Apache 
to forward traffic to the weather exporter tool listening on a different port (see below).

### Weather Exporter Setup

Very straightforward. Create the Influx database, configure listen address and 
Influx parameters in the `weather_exporter.yml` file, and start weather exporter with

    weather_exporter weather_exporter.yml

It'll automatically start to listen on the port and interface configured in the file. If
you are not running a webserver on port 80 already, you can simply use port 80, run as root,
and you're done. If you prefer running on a different port, or not as a privileged user, 
continue to next section to configure Apache to redirect weather traffic sent by the station.

### Apache Setup (optional)

If you're already running Apache on port 80 (like most servers will do), you need to redirect
traffic from the weather station to the weather exporter tool. I use the following statement 
in the apache configuration, assuming your service is listening on IP 192.168.100.55 and 
port 41001:

    ProxyPass "/weatherstation/updateweatherstation.php" "http://192.168.100.55:41001/post"

The tool only parses the query arguments and ignores the rest of the call URL, so it doesn't
really matter what path you specify.

### Systemd Setup (optional)

You can use the `weather-exporter.service` file provided in the repository to configure
the weather exporter to run as a service and start automatically on boot. Modify the file as 
needed (especially user, path to binary and path to config). copy to /etc/systemd/system,
and test with 

    service start weather-exporter

If it works, you can enable automatic start as usual with

    systemctl enable weather-exporter

