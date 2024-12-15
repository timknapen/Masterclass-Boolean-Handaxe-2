# A Boolean Handaxe 2

Masterclass at Sint Lucas Antwerpen 2024 about creating small interactive prototypes using a microcontroller and a tiny memory LCD display.

9 - 13  december 2024

# Teams

### 1 Emma and Kjeld
Wii Nunchuck drawing and thermal printer (title?):


### 2 Sera and Kobe
Scream to screen:
Creating an image by screaming into a microphone

### 3 Kaat and Jan
Chased by time: Complete a set of tasks by checking boxes before the deadline on Friday the 13th.

### 4 Carlo and Robbe
Sentence building: drawing with a joystick and a limited amount of ink

### 5 Antje, Nammu and Wout
Touch sensor + ball on the wavy sea



# Troubleshooting and setting up

Each project is configured to be compiled from Visual Studio Code with Platformio using the esp32-arduino platform. 
The hardware used is the ESP32-S3 Devkit C1 from Espressif. See the included image for the pinout of this dev board.

## Windows

For some windows machines the serial port didn't seem to work properly.
We fixed this by installing drivers and selecting a specific COM port.

1. Download and install the Silabs windows drivers. From here: [Silabs Drivers](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)
And click CP210x VCP Windows
2. Restart your computer (might not be necessary)
3. Find out the name of your COM port via PlatformIO > Quick Access > Devices
4. Uncomment the `WINDOWS SETTINGS` section and set the correct COM port in your platformio.ini file

        ; === WINDOWS SETTINGS ===
        upload_port = COM1
        monitor_port = COM1


Another problem was that git doesn't come with Visual Studio Code. Because of this our custom sharpmem-display library woulnd't load. So we installed git on those windows machines from https://git-scm.com/downloads


## MacOS

One student had python errors when trying to compile for the very first time. Restarting her computer fixed the problem.
