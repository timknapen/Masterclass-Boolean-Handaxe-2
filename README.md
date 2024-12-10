# A Boolean Handaxe 2

Masterclass Sint Lucas Antwerpen 2024

# Troubleshooting

## Windows

For some windows machines the serial port doesn't seem to work properly.
Fix by doing this:

1. Download and install the Silabs windows drivers. From here: [Silabs Drivers](https://www.silabs.com/developer-tools/usb-to-uart-bridge-vcp-drivers?tab=downloads)
And click CP210x VCP Windows
2. Restart your computer
3. Find out the name of your COM port via PlatformIO > Quick Access > Devices
4. Uncomment the `WINDOWS SETTINGS` section and set the correct COM port in your platformio.ini file

        ; === WINDOWS SETTINGS ===
        upload_port = COM1
        monitor_port = COM1


## MacOS

One student had python errors when trying to compile. Restarting her computer fixed the problem
