1. FRED – Intelligent ESP32 Robot with Emotions, Display & Blynk Control

FRED is an ESP32-powered smart robot car that blends motion, emotion, sensing, touch response, and audio feedback. It features expressive OLED “RoboEyes,” DFPlayer Mini sound effects, and full real-time control using the Blynk IoT app.

FRED doesn’t behave like a normal robot — it reacts to light, temperature, touch, movement, and user-selected moods, making it feel alive.

2. ✨ Features
2.1 Blynk App Control

Forward, Back, Left, Right, Speed Control.

2.2 Emotional OLED Eyes

Powered by FluxGarage_RoboEyes — happy, angry, tired, curious, sweating, flicker, autoblink.

2.3 DFPlayer Mini Audio

Movement SFX, tired-mode loop, greetings, and startup audio.

2.4 Automatic Mood Cycle

Happy → Default → Curious → Tired.

2.5 Environment Awareness

• LDR light detection
• DHT11 temperature reactions
• Touch-triggered interaction

2.6 Mechanical Movement

Servo-based head/steering + dual DC motor drive.

2.7 Special Modes

Startup animation + Happy-Lock Mode (physical button).

3. 🧩 Components Used

ESP32 Development Board

L298N Motor Driver

DC Motors (Left + Right)

SG90 / MG90 Servo Motor

128×64 SSD1306 OLED

FluxGarage RoboEyes Library

DFPlayer Mini + Speaker

DHT11 Temperature Sensor

LDR Sensor

Push Button (Happy-Lock Mode)

Jumper wires + battery pack + chassis

4. 📦 Pin Configuration
4.1 Motor Driver (L298N)

ENA → 2
IN1 → 4
IN2 → 5
IN3 → 18
IN4 → 19
ENB → 21
GND → ESP32 GND
+12v → battery +
GND → battery - 

4.2 Sensors & Others

Servo → 33
LDR → 34
Touch Input → 15
Button → 13
DHT11 → 27

4.3 DFPlayer Mini

TX → ESP32 RX (16)
RX → ESP32 TX (17)

4.4 Power
2s 3000mah lithium ion battery pack
mp1584 buck converter
Battery + → mp1584 + and l298N +
Battery - → mp1584 - and l298N -
set mp1584 to 5v output
mp1584 + → esp32 vin or 5v pin
mp1584 - → esp32 gnd
l298n gnd → esp32 gnd for logic


5. 📱 Blynk Virtual Pins

V0 → Speed Control
V1 → Forward
V2 → Backward
V3 → Left
V4 → Right

6. 🧠 Behavior System

FRED updates its emotional state based on:

• Time since last user interaction
• Current speed value
• Light intensity
• Temperature
• Manual movement commands
• Touch input

The OLED eyes animate accordingly: happy, angry, tired, sweating, curious, flicker, autoblink, and more.

7. 📚 Libraries Used

This ESP32 project uses several libraries to handle motors, sensors, OLED animation, audio, and IoT connectivity.

7.1 Blynk (ESP32)

Library: BlynkSimpleEsp32.h
Purpose: Blynk app control, virtual pins V0–V4, WiFi management.

7.2 WiFi

Library: WiFi.h
Purpose: Network connection for Blynk and OTA.

7.3 Adafruit SSD1306

Library: Adafruit_SSD1306.h
Purpose: OLED display driver — boot screen, temp display, debug output.

7.4 Adafruit GFX

Auto-included; handles text, shapes, and pixel rendering.

7.5 FluxGarage RoboEyes

Library: FluxGarage_RoboEyes.h
Link: https://github.com/FluxGarage/RoboEyes

Purpose: Expressive eye animations (happy, tired, angry, curious, sweating, flicker, autoblink).

7.6 ESP32Servo

Library: ESP32Servo.h
Purpose: Controls steering servo using software PWM.

7.7 DHT Sensor Library

Libraries: DHT.h, Adafruit_Sensor.h
Purpose: Temperature-based mood logic (cold flicker, hot sweat).

7.8 DFPlayer Mini

Library: DFRobotDFPlayerMini.h
Purpose: MP3 playback — startup, movement, tired-mode audio.

7.9 HardwareSerial

Library: HardwareSerial.h
Purpose: DFPlayer communication (UART1: pins 16 & 17).

7.10 Wire (I²C)

Library: Wire.h
Purpose: OLED communication (SDA = 23, SCL = 22).

7.11 ESP System

Library: esp_system.h
Purpose: ESP32 internal system functions.

7.12 Arduino Core

Built into ESP32 board package. Used for pin control, timing, and core logic.

8. 🎵 Audio Instructions

Format your SD card to FAT32 before loading audio files.
If sounds don’t play in correct order, adjust the track number inside:

myDFPlayer.play(<track_number>);
