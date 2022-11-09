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
In `/usr/local/etc/mosquitto/mosquitto.conf` file set following:
