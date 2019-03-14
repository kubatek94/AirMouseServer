AirMouse
===================
AirMouse is a WiFi remote control application, which allows you to control your computer from Android smartphones over WiFi.

I have created AirMouse, as I wanted to control Kodi on Raspberry Pi using my phone, however existing virtual mouse/keyboard solutions didn't work on a Linux host.
It can act like a TouchPad on a laptop, or another mode is the AirRemote, which uses the accelerometer+gyroscope to control the mouse by moving your phone (similar to LG's Magic Remote, for example).

This is a server component (linux virtual device driver + TCP server) that requires [android application](https://github.com/kubatek94/AirMouseClient) to work together.

