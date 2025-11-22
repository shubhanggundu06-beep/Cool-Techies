<span style="font-size:42px; font-weight:900;">FRED 🤖 – Intelligent ESP32 Robot with Emotions, Display & Blynk Control</span>

<span style="font-size:18px;">FRED is an ESP32-powered smart robot car that mixes movement, moods, OLED animations, temperature awareness, touch interaction, and sound effects into one mini-character.
It uses expressive OLED RoboEyes, DFPlayer audio, and full Blynk app control, making the robot feel alive instead of just… wheels and wires.</span>

<span style="font-size:28px;">✨ 1. Features</span>

Blynk App Control – Forward, Back, Left, Right + variable speed

Emotional OLED Eyes via FluxGarage_RoboEyes

DFPlayer Mini Audio – movement sounds, tired effects, greetings

Automatic Mood Cycle
 • Happy → Default → Curious → Tired

Light Detection through LDR

Touch-activated reactions

Temperature-based emotions using DHT11

Servo Head Steering / Movement

Startup animation + sound

Happy-Lock Mode using physical button

FRED reacts to light, temperature, mood commands, motor usage, and human interaction — it behaves more like a tiny robot pet.

<span style="font-size:28px;">🧩 2. Components Used</span>

ESP32 Dev Board

L298N Motor Driver

DC Motors (Left + Right)

SG90 / MG90 Servo Motor

128×64 SSD1306 OLED Display

FluxGarage RoboEyes Library

DFPlayer Mini + Speaker

DHT11 Sensor

LDR Light Sensor

Push Button (Happy-Lock)

Jumper wires, chassis, battery pack

<span style="font-size:28px;">📦 3. Pin Configuration</span>
<span style="font-size:20px;">Motor Driver – L298N</span>

ENA → 2

IN1 → 4

IN2 → 5

IN3 → 18

IN4 → 19

ENB → 21

Sensors & Others

Servo → 33

LDR → 34

Touch Input → 15

Button → 13

DHT11 → 27

DFPlayer Mini

TX → ESP32 RX (16)

RX → ESP32 TX (17)

<span style="font-size:28px;">📱 4. Blynk Virtual Pins</span>

V0 → Speed

V1 → Forward

V2 → Backward

V3 → Left

V4 → Right

<span style="font-size:28px;">🧠 5. Behavior System</span>

FRED updates its emotional state based on:

Idle time

Light level

Temperature

Touch input

Motor speed

Direct commands

Its OLED eyes animate accordingly — happy, tired, angry, sweating, curious, and more.

<span style="font-size:28px;">📚 6. Libraries Used</span>
1. Blynk (ESP32 Edition)

Header: #include <BlynkSimpleEsp32.h>
Controls movement, speed, and WiFi communication.

2. WiFi.h

Handles internet connectivity for Blynk.

3. Adafruit SSD1306

Header: #include <Adafruit_SSD1306.h>
Runs the 128×64 OLED screen.

4. Adafruit GFX

Helper library for drawing graphics.

5. FluxGarage RoboEyes

Header: #include <FluxGarage_RoboEyes.h>
Handles expressive animated eyes (blink, flicker, tired, happy, etc.).

6. ESP32Servo

Header: #include <ESP32Servo.h>
Controls steering servo.

7. DHT Sensor Library

Headers:
#include <DHT.h>
#include <Adafruit_Sensor.h>
Reads temperature + changes emotion.

8. DFPlayer Mini

Header: #include <DFRobotDFPlayerMini.h>
Handles audio playback for movements, greetings, tiredness, etc.

9. HardwareSerial

Header: #include <HardwareSerial.h>
Used for UART communication with DFPlayer.

10. Wire

Header: #include <Wire.h>
I²C communication with OLED.

11. ESP System

Header: #include "esp_system.h"
Low-level ESP32 functions.

12. Arduino Core

Handles digitalWrite, analogRead, delays, millis, etc.

<span style="font-size:22px;">🔊 Audio File Setup</span>

Format your SD card to FAT32, then copy your audio tracks.
If a sound doesn’t play, adjust the track number in code:

myDFPlayer.play(3);  // play file 003.mp3
