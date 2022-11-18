# This instruction describes how to make two-way communications with Zumo bot.
## Install broker to your PC: 

1. [debian](https://mosquitto.org/blog/2013/01/mosquitto-debian-repository/)
2. ubuntu: Mosquitto is available in the Ubuntu repositories so you can install as with any other package. If you are on an earlier version of Ubuntu or want a more recent version of mosquitto, add the mosquitto-dev PPA to your repositories list - see the [link](https://launchpad.net/~mosquitto-dev/+archive/ubuntu/mosquitto-ppa) for details. mosquitto can then be installed from your package manager.
   
   ```sudo apt-add-repository ppa:mosquitto-dev/mosquitto-ppa```

    ```sudo apt-get update```
3. Windows
    
    - [mosquitto-2.0.15-install-windows-x64.exe](https://mosquitto.org/files/binary/win64/mosquitto-2.0.15-install-windows-x64.exe) (64-bit build, Windows Vista and up, built with Visual Studio Community 2019)
    - [mosquitto-2.0.15-install-windows-x32.exe](https://mosquitto.org/files/binary/win32/mosquitto-2.0.15-install-windows-x86.exe) (32-bit build, Windows Vista and up, built with Visual Studio Community 2019)
    
    Older installers can be found at https://mosquitto.org/files/binary/. See also README-windows.md after installing.

## Set up the broker
In `/usr/local/etc/mosquitto/mosquitto.conf` file set following: <br>
```
listener 1883 
listener 9001 
protocol wesockets
socket_domain ipv4
allow_anonymous true
```
Please note, that depending on your OS location of this file may be different.

## Change configuration in Zumo project
In [zumo_config.h](/g1_map_robot.cydsn/zumo_config.h) change the following:
```c
#define START_MQTT 1

/* The following definitions must all be in double quotes 

Your home router/access point newtwork name*/
#define NETWORK_SSID "Your_router_network"

/* Your home router/access point newtwork password */
#define NETWORK_PASSWORD "Password"

/* IP address of your computer on which you've installed the mqtt broker
 for example mosquitto */
#define MQTT_BROKER "0.0.0.0"

/* Name of the client in this case name of zumo. Can be any name*/
#define MQTT_CLIENT_ID "Zumo101"
```

## Getting messagies from Zumo
Run in terminal:
for all topics - #
```
mosquitto_sub -t "#"
```
for current topics:
```
mosquitto_sub -t "t_status" or
mosquitto_sub -t "Zumo01/debug"
```
## Sending messages to Zumo
Currently Zumo is [subscribed here](g1_map_robot.cydsn/ZumoLibrary/mqtt_sender.c) on the line 105 to one topic: **settings**. Send something with *different* topic:
```
mosquitto_pub -t "different" -m "Hello, Zumo!"
```
For Windows command for mosquitto is different.
