# Bluetooth stroboscope controller

This is an Arduino sketch for a Bluetooth stroboscope controller. It receives commands from the HC-05 module and generates required flashing signal.

![Bluetooth stroboscope controller](https://lh3.googleusercontent.com/es6hBNBkK6AO3K89UdSPQoApuUB13hT87MibrAbB7H90mORQjikJHn-gcc8qUH6eGrg5RRVZD6C_V7JCgYuYo8nSUMyXJKDPF4BmyVrwuxE40V_ba_7jrlSkylZRJLy-_g5R_AvZ_A=w339-h250-no)
![Free Color S750 with the controller](https://lh3.googleusercontent.com/vrKrXQYxlfOXsFljf_KN-1rSfUMRFfQTgoHQwcWdfcxUb5kjHoKRYdiy3oYC2-odJx38lkR1pj2HuHUV-yRdp15IyHdImNZuYuWroA9rdFtQPC8lYyOMFEB10jty4j0gdMUQQJCe=w333-h250-no)
## Set up

This sketch is running on the 5V Arduino Pro Mini. The HC-05 module is connected directly to RX and TX pins. Flashing output is directed to S750's audio in.

## Command protocol

| Name | Code | Payload | Example |
| -----|------|---------|---------|
|Single flash|0x1|-|`0x1`|
|Stop|0x2|-|`0x2`|
|Continuous flashing (Hz)|0x3|unsigned byte of Hz value|`0x3 0x5` — start flashing at 5 Hz frequency
|Continuous flashing (BPM)|0x4|2 bytes of unsigned short BMP value in **Little endian**|`0x4 0x4 0x1` — start flashing at 260 (0x104) BMP frequency|
