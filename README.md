# ESP32-Programmable-Joystick

Simple example for programmable joystick based on **ESP32-C3 SuperMini**.
https://nikitak.dev/wp-admin/post.php?post=1390&action=elementor

## Features
- 4 buttons using input pull-up
- Nintendo Switch joystick (2 analog axes)
- Joystick button
- Wireless communication:
  - Wi-Fi
  - BLE
  - ESP-NOW

## Hardware
- ESP32-C3 SuperMini
- Nintendo Switch joystick
- 4 buttons
- Battery + 5V boost converter

## Pinout
| Function | Pin |
|--------|--------|
| Joystick X | GPIO0 |
| Joystick Y | GPIO1 |
| Button | GPIO2 |

## Build
Project uses **PlatformIO**.

```bash
pio run
