#define BLYNK_TEMPLATE_ID "TMPL32VpW6FaX"
#define BLYNK_TEMPLATE_NAME "robo"
#define BLYNK_AUTH_TOKEN "lkraCI_FSk22G5nYpl28Ao9D4L7Ewvrv"

// ------------------- Libraries -------------------
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <ESP32Servo.h>
#include <FluxGarage_RoboEyes.h>
#undef S  // Remove macro conflict
#include "DHT.h"
#include <Adafruit_Sensor.h>
#include "esp_system.h"
#include "DFRobotDFPlayerMini.h"
#include <HardwareSerial.h>

// ------------------- Car Pins -------------------
#define ENA 2
#define IN1 4
#define IN2 5
#define IN3 18
#define IN4 19
#define ENB 21

#define SERVO_PIN 33
#define LDR_PIN 34

// ------------------- OLED + RoboEyes -------------------
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);
RoboEyes<Adafruit_SSD1306> roboEyes(display);


// ------------------- Components -------------------
Servo myservo;
#define DHTPIN 27
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);
#define BUTTON_PIN 13
bool happyLock = false;
bool lastButtonState = HIGH;

// ------------------- Touch + Timers -------------------
#define TOUCH_PIN 15
bool touchActive = false;
unsigned long touchStart = 0;

// ------------------- Light -------------------
unsigned long curiosityStartTime = 0;

bool SleepStart= false;
bool eyeLoopActive = false; 
bool tiredSoundPlayed = false;
bool tiredAudioLooping = false;  // tracks if tired audio is looping
bool startTiredLoop = false;     // signal to start looping in loop()
bool stopTiredLoop = false;
bool touchSweepForward = true;
int servoPos = 0;
unsigned long lastServoUpdate = 0;
const int servoInterval = 50; // ms between steps


// ------------------- Movement -------------------
bool forward = false;
bool backward = false;
bool left = false;
bool right = false;
bool turning = false;
unsigned long lastTurnTime = 0;
unsigned long lastCheck = 0;

// ------------------- Temperature -------------------
unsigned long tempTimer = 0;
const unsigned long tempInterval = 120000; // 5 min
bool tempActive = false;
unsigned long tempStart = 0;

int Speed = 0;
bool setupTempActive = false;
unsigned long setupTempStart = 0;
float setupTemp = 0;
// ------------------- Mood Timers -------------------
unsigned long moodStart = 0;
unsigned long H = 5000;
unsigned long D = 20000;
unsigned long C = 37000;
bool lastForward = false, lastBackward = false, lastLeft = false, lastRight = false;
// ------------------- Speed Expression -------------------
bool speedMoodActive = false;
unsigned long speedMoodStart = 0;
// ------------------- WiFi/Blynk -------------------
char auth[] = BLYNK_AUTH_TOKEN;
char ssid[] ="Purnachander";
char pass[]="speed123";

// ------------------- Custom -------------------
#define SOUTH 5
const byte RXD2 = 25; // DFPlayer TX -> ESP32 RX
const byte TXD2 = 26; // DFPlayer RX -> ESP32 TX


// ------------------- DFPlayer -------------------
HardwareSerial dfSD(1); // Use UART channel 1
DFRobotDFPlayerMini myDFPlayer;

// ------------------- Function Prototypes -------------------
void readLDR(int pin, int threshold);
void playMovementSound(String direction);

// ------------------- Setup -------------------
void setup() {
    Serial.begin(115200);
    Wire.begin(23, 22);

    if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
        Serial.println(F("SSD1306 failed"));
        for (;;);
    }
    display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1.5);
  display.setCursor(10, 20);
  display.println("Connecting...");
  display.display();

  WiFi.begin(ssid,pass);

  unsigned long wifiStart = millis();
  while (WiFi.status() != WL_CONNECTED && millis() - wifiStart < 4000) {
    delay(500);
    Serial.print(".");
  }

  if (WiFi.status() != WL_CONNECTED) {
    display.clearDisplay();
    display.setTextSize(2);
    display.setCursor(20, 20);
    display.println("NO WIFI");
    display.display();
    Serial.println("No WiFi detected!");
    delay(2000);
  } else {
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 10);
    display.println("WiFi Connected!");
    display.setCursor(0, 30);
    display.println(WiFi.localIP());
    display.display();
    delay(1500);
  }

    roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 200);
    roboEyes.setMood(HAPPY);
    roboEyes.setPosition(DEFAULT);
    roboEyes.setAutoblinker(OFF, 3, 2);
    roboEyes.setIdleMode(ON, 2, 2);

    myservo.attach(SERVO_PIN);

    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);

    
    pinMode(BUTTON_PIN, INPUT_PULLUP); // Button connected between pin and GND

    pinMode(ENA, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    pinMode(ENB, OUTPUT);
    pinMode(TOUCH_PIN, INPUT);

    // ------------------- DFPlayer Init -------------------
    dfSD.begin(9600, SERIAL_8N1, RXD2, TXD2);// RX=16, TX=17
    if (!myDFPlayer.begin(dfSD)) {
        Serial.println(F("DFPlayer Mini not detected!"));
    } else {
        Serial.println(F("DFPlayer Mini online."));
        myDFPlayer.volume(25); // 0~30
    }
    myDFPlayer.setTimeOut(500); 
   myDFPlayer.play(2);
    // ------------------- Servo Sweep -------------------
    for (int pos = 0; pos <= 180; pos += 4) { 
        myservo.write(pos);
        roboEyes.update();
        delay(10);
    }
    for (int pos = 180; pos >= 0; pos -= 4) { 
        myservo.write(pos);
        roboEyes.update();
        delay(10);
    }
    for (int pos = 0; pos <= 90; pos += 4) {
        myservo.write(pos);
        roboEyes.update();
        delay(10);
    }

    myservo.write(90);
    roboEyes.update();
    delay(300);
    myDFPlayer.play(3);
    moodStart = millis();
    dht.begin();
    delay(1500);

    float tempTest = dht.readTemperature();
    float humTest  = dht.readHumidity();

if (isnan(tempTest) || isnan(humTest)) {
    Serial.println(F("DHT11 sensor not detected!"));
} else {
    Serial.println(F("DHT11 sensor detected successfully."));
    Serial.print("Room Temperature: ");
    Serial.print(tempTest);
    Serial.println(" °C");
    Serial.print("Room Humidity: ");
    Serial.print(humTest);
    Serial.println(" %");
    setupTemp = tempTest;
    setupTempActive = true;
    setupTempStart = millis();
}
}
// ------------------- Blynk Handlers -------------------
BLYNK_WRITE(V0) {
    Speed = param.asInt();
    speedMoodActive = true;
    speedMoodStart = millis();
    moodStart= millis();
    // Speed-based mood logic unchanged
    if (Speed <= 80) {
        roboEyes.setMood(TIRED);
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setPosition(DEFAULT);
        roboEyes.setIdleMode(OFF,2,3);
        roboEyes.setAutoblinker(ON,2,3);

    } else if (Speed <= 180) {
        roboEyes.setMood(HAPPY);
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setPosition(DEFAULT);
        roboEyes.setIdleMode(OFF,2,3);
        roboEyes.setAutoblinker(ON,2,3);

    } else {
        roboEyes.setMood(ANGRY);
        roboEyes.setSweat(ON);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setPosition(DEFAULT);
        roboEyes.setIdleMode(OFF,2,3);
        roboEyes.setAutoblinker(ON,2,3);

    }
}

BLYNK_WRITE(V1) { forward = param.asInt(); }
BLYNK_WRITE(V2) { backward = param.asInt(); }
BLYNK_WRITE(V3) { left = param.asInt(); }
BLYNK_WRITE(V4) { right = param.asInt(); }

// ------------------- Car Functions -------------------
void carForward() {
    analogWrite(ENA, Speed);
    analogWrite(ENB, Speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
}
void carBackward() {
    analogWrite(ENA, Speed);
    analogWrite(ENB, Speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
}
void carLeft() {
    analogWrite(ENA, Speed);
    analogWrite(ENB, Speed);
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    myservo.write(30);
    turning = true;
    lastTurnTime = millis();
}
void carRight() {
    analogWrite(ENA, Speed);
    analogWrite(ENB, Speed);
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    myservo.write(150);
    turning = true;
    lastTurnTime = millis();
}
void carStop() {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, LOW);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    myservo.write(90);
}

// ------------------- Movement Sound -------------------
void triggerSounds() {
  if (forward && !lastForward) myDFPlayer.play(5);
  if (backward && !lastBackward) myDFPlayer.play(8);
  if (left && !lastLeft) myDFPlayer.play(4);
  if (right && !lastRight) myDFPlayer.play(1);
       // Pause music only if no movement and music is playing
    if (!forward && !backward && !left && !right) {
        if (lastForward || lastBackward || lastLeft || lastRight) {
            myDFPlayer.pause();
        }
    }
  lastForward = forward;
  lastBackward = backward;
  lastLeft = left;
  lastRight = right;
}

// ------------------- Rest of code -------------------
// Keep all your RGB, eyeloop(), WIFIcar(), loop(), readLDR(), temp logic, touch logic intact


// ------------------- RGB -------------------
// ------------------- Eye Loop -------------------
void eyeloop() {
    if (happyLock) {
    roboEyes.setMood(HAPPY);
    roboEyes.setSweat(OFF);
    roboEyes.setCuriosity(OFF);
    roboEyes.setHFlicker(OFF);
    roboEyes.setAutoblinker(OFF, 3, 2);
    roboEyes.setIdleMode(OFF, 2, 2);
    roboEyes.setPosition(DEFAULT);
    return; // skip rest of eyeloop while happy is locked
}

    if (SleepStart) {
        eyeLoopActive = false;
        return;
    }

    eyeLoopActive = true;
    unsigned long elapsed = millis() - moodStart;

    if (speedMoodActive || forward || backward || left || right || touchActive || tempActive) return;

    // ----------------- HAPPY PHASE -----------------
    if (elapsed < H) {
        roboEyes.setMood(HAPPY);
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setAutoblinker(ON, 3, 2);
        roboEyes.setIdleMode(ON, 2, 2);
    
        if (tiredAudioLooping) stopTiredLoop = true;  // stop tired audio

    } 
    // ----------------- DEFAULT PHASE -----------------
    else if (elapsed < D) {
        roboEyes.setMood(DEFAULT);
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setAutoblinker(ON, 3, 2);
        roboEyes.setIdleMode(ON, 2, 2);

        if (tiredAudioLooping) stopTiredLoop = true;

    } 
    // ----------------- CURIOSITY PHASE -----------------
  
// Inside eyeloop() curiosity phase
    else if (elapsed < C) {
    roboEyes.setMood(DEFAULT);
    roboEyes.setCuriosity(ON);
    roboEyes.setSweat(OFF);
    roboEyes.setHFlicker(OFF);
    roboEyes.setAutoblinker(ON, 2, 3);
    roboEyes.setIdleMode(ON, 2, 3);
    if (tiredAudioLooping) stopTiredLoop = true;
      if (curiosityStartTime == 0) {
        curiosityStartTime = millis();
    }

    unsigned long time = millis() - curiosityStartTime;

    if (time < 6000) {
        myservo.write(150);
        roboEyes.update();
        roboEyes.setCuriosity(ON);  // first 3 sec
    } 
    else if (time < 12000) {
        myservo.write(30);   // next 3 sec
        roboEyes.update();
        roboEyes.setCuriosity(ON);
    }
    else {
        myservo.write(90);
        roboEyes.update();
        roboEyes.setCuriosity(ON);   // after 6 sec
    }
    }
    else {
        curiosityStartTime = 0;
        roboEyes.setMood(TIRED);
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setAutoblinker(ON, 2, 3);
        roboEyes.setIdleMode(ON, 4, 3);

        if (!tiredAudioLooping) startTiredLoop = true;  // signal to start looping
     }
}


// ------------------- Movement Control -------------------
void WIFIcar() {
    // Explicitly setting all eye features for movement (ANGRY/ACTIVE STATE)
    if (forward) {
        carForward();
        SleepStart = false;
        roboEyes.setMood(ANGRY);
        roboEyes.setSweat(ON);
        roboEyes.setCuriosity(OFF);  // Explicitly set
        roboEyes.setHFlicker(OFF);   // Explicitly set
        roboEyes.setPosition(DEFAULT);
        roboEyes.setIdleMode(OFF, 4, 3);
        roboEyes.setAutoblinker(ON, 2, 3);
        moodStart = millis();
    } else if (backward) {
        carBackward();
        SleepStart = false;
        roboEyes.setMood(ANGRY);
        roboEyes.setSweat(ON);
        roboEyes.setCuriosity(OFF);  // Explicitly set
        roboEyes.setHFlicker(OFF);   // Explicitly set
        roboEyes.setPosition(NW);
        roboEyes.setIdleMode(OFF, 4, 3);
        roboEyes.setAutoblinker(ON, 2, 3);
        moodStart = millis();
    } else if (left) {
        carLeft();
        SleepStart = false;
        roboEyes.setMood(ANGRY);
        roboEyes.setSweat(ON);
        roboEyes.setCuriosity(OFF);  // Explicitly set
        roboEyes.setHFlicker(OFF);   // Explicitly set
        roboEyes.setPosition(W);
        roboEyes.setIdleMode(OFF, 4, 3);
        roboEyes.setAutoblinker(ON, 2, 3);
        moodStart = millis();
    } else if (right) {
        carRight();
        SleepStart = false;
        roboEyes.setMood(ANGRY);
        roboEyes.setSweat(ON);
        roboEyes.setCuriosity(OFF);  // Explicitly set
        roboEyes.setHFlicker(OFF);   // Explicitly set
        roboEyes.setPosition(E);
        roboEyes.setIdleMode(OFF, 4, 3);
        roboEyes.setAutoblinker(ON, 2, 3);
        moodStart = millis();
    } else {
        carStop();
        SleepStart = false;
    }
}
void checkTemperature() {
    unsigned long currentMillis = millis();

    // Every 2 minutes
    if (!tempActive && currentMillis - tempTimer >= tempInterval) {
        
        float t = dht.readTemperature();

        if (!isnan(t)) {
            // Set mood based on temp
            if (t < 30) {
                roboEyes.setMood(TIRED);
                roboEyes.setHFlicker(ON, 2);
                myDFPlayer.play(4);
                roboEyes.setSweat(OFF);
                roboEyes.setCuriosity(OFF);
            } 
            else if (t > 35) {
                roboEyes.setMood(TIRED);
                roboEyes.setHFlicker(OFF);
                roboEyes.setSweat(ON);
                myDFPlayer.play(7);
                roboEyes.setCuriosity(OFF);
            } 
            else {
                roboEyes.setMood(HAPPY);
                roboEyes.setSweat(OFF);
                roboEyes.setHFlicker(OFF);
                myDFPlayer.play(3);
                roboEyes.setCuriosity(OFF);
            }

            // Show temperature for this period only
            display.fillRect(SCREEN_WIDTH - 35, 0, 35, 10, SSD1306_BLACK);
            display.setTextColor(SSD1306_WHITE);
            display.setTextSize(1);
            display.setCursor(SCREEN_WIDTH - 35, 0);
            display.print(t, 1);
            display.print(" C");
            display.display();

            Serial.print("Room Temperature: ");
            Serial.print(t);
            Serial.println(" °C");

            tempActive = true;
            tempStart = currentMillis;
            tempTimer = currentMillis;
        }
    }

    // After 10 seconds → clear temp and resume idle behavior
    if (tempActive && currentMillis - tempStart >= 10000) {
        tempActive = false;
        moodStart = millis(); // return to eye idle loop
        display.fillRect(SCREEN_WIDTH - 35, 0, 35, 10, SSD1306_BLACK);
        display.display();
    }
}


void drawTemperature(float t) {
    // draw always after roboEyes.update()
    display.setTextColor(SSD1306_WHITE, SSD1306_BLACK); // overwrite cleanly
    display.setTextSize(1);
    display.setCursor(SCREEN_WIDTH - 38, 0);
    display.print(t, 1);
    display.print("C");
    display.display();
}



// ------------------- Main Loop -------------------
void loop() {
    Blynk.run();
    WIFIcar();
    eyeloop();
    triggerSounds();
    readLDR(LDR_PIN, 1000);
    checkTemperature();

    if (eyeLoopActive) {
        roboEyes.update();  // only update eyes if awake
    }
    if (tempActive) {
    float currentTemp = dht.readTemperature();
    if (!isnan(currentTemp)) drawTemperature(currentTemp);
}
       if (startTiredLoop) {
        myDFPlayer.loop(6); 
        myDFPlayer.volume(30);       // start looping tired audio
        startTiredLoop = false;
        tiredAudioLooping = true;
    } else{
        myDFPlayer.volume(25);
    }

    if (stopTiredLoop) {
        myDFPlayer.stop();          // stop looping tired audio
        stopTiredLoop = false;
        tiredAudioLooping = false;
    }
    int buttonState = digitalRead(BUTTON_PIN);

// Detect falling edge (button press)
if (buttonState == LOW && lastButtonState == HIGH) {
    happyLock = !happyLock; // Toggle the state

    if (happyLock) {
        roboEyes.setMood(HAPPY);
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);
        roboEyes.setAutoblinker(OFF, 3, 2);
        roboEyes.setIdleMode(OFF, 2, 2);
        roboEyes.setPosition(DEFAULT);
        roboEyes.setHeight(40, 40);
        roboEyes.setWidth(40, 40);
        Serial.println("Happy mode locked!");
        myDFPlayer.stop();
    } else {
        roboEyes.setHeight(36, 36);
        roboEyes.setWidth(36, 36);
        moodStart = millis(); // resume normal cycle
        Serial.println("Resumed normal mood cycle.");
        myDFPlayer.start();
    }
    delay(200); // debounce
}

lastButtonState = buttonState;                   
    unsigned long currentMillis = millis();

    // Temperature Check (Explicitly setting all eye features for temporary temperature state)
    
    if(tempActive && currentMillis - tempStart >= 4000){
        tempActive = false;
        moodStart = millis();
    }

    if (speedMoodActive && millis() - speedMoodStart > 3000) {
        speedMoodActive = false;
        moodStart = millis();
    }
    if (setupTempActive) {
    display.setTextColor(SSD1306_WHITE);
    display.setTextSize(1);
    
    // Move cursor slightly left so 'C' fits
    display.setCursor(SCREEN_WIDTH - 40, 0); // -40 instead of -35

    // Print temperature
    display.print(setupTemp, 1);

    display.print("C");  // no space needed
    
    display.display();

    // Stop displaying after 10 seconds
    if (millis() - setupTempStart >= 10000) {
        setupTempActive = false;
        display.fillRect(SCREEN_WIDTH - 40, 0, 40, 10, SSD1306_BLACK); // match width
        display.display();
    }
}

    // Touch Sensor Logic (Explicitly setting all eye features for temporary touch state)
    int touch = digitalRead(TOUCH_PIN);
    if (touch == HIGH && !touchActive) {
        roboEyes.setMood(HAPPY);
        roboEyes.setSweat(OFF);      // Explicitly set
        roboEyes.setCuriosity(OFF);  // Explicitly set
        roboEyes.setHFlicker(OFF);   // Explicitly set
        roboEyes.setPosition(DEFAULT); // Explicitly set
        roboEyes.setAutoblinker(OFF, 3, 2);
        roboEyes.setIdleMode(OFF, 2, 2);
        roboEyes.anim_laugh();
        myDFPlayer.play(9);
        delay(10); // deboounce

        touchActive = true;
        touchStart = millis();
        moodStart = millis();
    for (int pos = 0; pos <= 180; pos += 45) { 
        myservo.write(pos);
        roboEyes.update();
        delay(10);
    }
    for (int pos = 180; pos >= 0; pos -= 45) { 
        myservo.write(pos);
        roboEyes.update();
        delay(10);
    }
    for (int pos = 0; pos <= 90; pos += 45) {
        myservo.write(pos);
        roboEyes.update();
        delay(10);
    }
  }

    if (touchActive && millis() - touchStart >= 1500) {
        touchActive = false;
        moodStart=millis();
    }
}

// ------------------- LDR Sleep/Wake -------------------
void readLDR(int pin, int threshold) {
    int ldrValue = analogRead(pin);
    if (ldrValue > threshold) {
        if (SleepStart) {          // only act once when waking
            SleepStart = false;
            moodStart=millis();
            Serial.println("Woke up, resetting eye loop");
        }
        // No need to set features here, as the sleep state is ending and control returns to eyeloop
        roboEyes.open(); 
    } else {
        if (!SleepStart) {
            SleepStart = true;     // only act once when sleeping
            Serial.println("Going to sleep");
        }
        // Explicitly setting all eye features for the sleep state
        roboEyes.close();
        roboEyes.setMood(DEFAULT);    // Explicitly set
        roboEyes.setPosition(DEFAULT); // Explicitly set
        roboEyes.setSweat(OFF);
        roboEyes.setCuriosity(OFF);
        roboEyes.setHFlicker(OFF);   // Explicitly set
        roboEyes.setIdleMode(ON,4,4);
        roboEyes.setAutoblinker(OFF,2,3);
        
    }
}