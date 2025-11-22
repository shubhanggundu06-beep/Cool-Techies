# Cool-Techies
FRED – Intelligent ESP32 Robot with Emotions, Display & Blynk Control

FRED is an ESP32-powered smart robot car that combines movement, mood-based OLED animations, temperature awareness, touch interaction, and audio feedback.
The robot features expressive OLED “RoboEyes,” DFPlayer-based sound effects, and full control through the Blynk IoT app.

FRED isn’t just a car — it reacts to light, temperature, touch, movement, and even user mood commands, making it feel alive.

✨ Features

Blynk App Control (Forward, Back, Left, Right, Speed)

Emotional OLED Eyes powered by FluxGarage_RoboEyes

DFPlayer Mini Audio (movement sounds, tired sounds, greetings)

Automatic Mood Cycle

Happy → Default → Curious → Tired

Light Detection using an LDR sensor

Touch-activated interaction

Temperature-aware reactions with DHT11

Servo-based head movement / steering

Startup animation and audio

Special Happy-Lock Mode via physical button

🧩 Components Used

ESP32 Development Board

L298N Motor Driver

DC Motors (Left + Right)

SG90 / MG90 Servo Motor

128x64 SSD1306 OLED Display

FluxGarage RoboEyes Library

DFPlayer Mini + Speaker

DHT11 Temperature Sensor

LDR (Light Sensor)

Push Button (Happy-Lock mode)

Jumper wires, battery pack, chassis

📦 Pin Configuration
Motor Driver

ENA – 2

IN1 – 4

IN2 – 5

IN3 – 18

IN4 – 19

ENB – 21

Sensors & Others

Servo – 33

LDR – 34

Touch Input – 15

Button – 13

DHT11 – pin 27

DFPlayer Mini

TX → ESP32 RX (16)

RX → ESP32 TX (17)

📱 Blynk Virtual Pins

V0 → Speed Control

V1 → Forward

V2 → Backward

V3 → Left

V4 → Right

🧠 Behavior System

FRED updates its emotional state depending on:

time since last interaction

speed value

light level

temperature

direct movement commands

touch input

Eyes animate accordingly (happy, angry, tired, sweating, curious, etc.).

📚 Libraries Used

This project runs on an ESP32 and uses multiple libraries to control motors, sensors, audio, OLEDs, and an emotion-based eye animation system.
Here’s the full breakdown of every library needed:

1. Blynk (ESP32 version)

Purpose: Handles app-based control of the robot (speed, direction, etc.)
Library: BlynkSimpleEsp32.h
Used for:

Virtual pins V0–V4

WiFi + Cloud connection

2. WiFi Library

Library: WiFi.h
Purpose: Connects ESP32 to the internet for Blynk + OTA behavior.

3. Adafruit SSD1306

Library: Adafruit_SSD1306.h
Purpose: Drives the 128×64 OLED display.
Used for:

Boot screen

Temperature display

Debug messages

4. Adafruit GFX

Automatically included with SSD1306.
Purpose: Drawing text, shapes, pixels on OLED.

5. FluxGarage RoboEyes
Link: https://github.com/FluxGarage/RoboEyes
Library: FluxGarage_RoboEyes.h
Purpose: The fun part — animated expressive eyes.
Used for:

HAPPY

TIRED

ANGRY

CURIOUS

Autoblinker, idle motion, flicker, sweat, etc.

6. ESP32Servo

Library: ESP32Servo.h
Purpose: Controls steering servo through software PWM on ESP32.

7. DHT Sensor Library

Libraries:

DHT.h

Adafruit_Sensor.h

Purpose: Reads temperature & humidity from DHT11.
Used for mood reactions:

Too cold → flicker

Too hot → sweat

Normal → happy

8. DFPlayer Mini Library

Library: DFRobotDFPlayerMini.h
Purpose: MP3 playback from SD card using DFPlayer mini.
Used for:

Startup sound

Movement sounds

Temperature reaction sounds

Tired-mode looping audio

9. HardwareSerial

Library: HardwareSerial.h
Purpose: Controls DFPlayer on UART1 (pins 16,17).

10. Wire (I²C)

Library: Wire.h
Purpose:

I²C communication with OLED display

Custom SDA = 23, SCL = 22

11. ESP System

Library: "esp_system.h"
Purpose:

System-level calls

ESP32 chip functions

12. Arduino Core Functions

Already included through the ESP32 board package.
Used for:

Digital/analog write

PinMode

Delays

millis()

Please upload your audio files to sd card by formatting it to fat32 system.
You can change the audio file number if not being played correctly by changing the audio track number in code , the code snippet will look like this
myDFPlayer.play( some number).
