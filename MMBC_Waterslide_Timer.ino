//Section #1: Define variables

//Pins for various things
int resetButton = 2;
int startLaser = 0;
int stopLaser = 1;
int readyLED = 3;

/*
  Some currently unneccessary integers - they were for the buttons that I tested this with at home
  int startButtonState = 0;
  int lastStartButtonState = 0;
  int stopButtonState = 0;
  int lastStopButtonState = 0;
*/

//Booleans for the program's running
bool r = false; // Variable for whether or not the timer is currently running.
bool showTime = false; //Variable to tell the program when to show the final time.
bool RTG = true; // Variable for whether or not the system has been reset and is good to go.

//Storage for the two numbers needed to calculate the total time of the run, as well as the final number
unsigned long start; // Start time (in milliseconds - based off the Arduino's internal clock, running since power on)
unsigned long finish; // End time (in milliseconds - based off the Arduino's internal clock, running since power on)
unsigned long elapsed; // How much time (in milliseconds) has actually gone by for the duration of the run.
unsigned long sampleStart; // Time when samples start to be taken.
unsigned long sampleStop; // Time when samples should stop being taken.
unsigned long sampleInterval = 500; // Delay between each sample (Should usually be 500ms)

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Section #2: Setup

void setup() {
  // put your setup code here, to run once:

  /*
    pinMode(startButton, INPUT);
    pinMode(stopButton, INPUT);
 
  */
   Serial.begin(9600);

  pinMode(readyLED, OUTPUT);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print("Hello!");
  display.display();
  delay(1000);
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.setCursor(45, 0);
  display.print("MMBC");
  display.setTextSize(1);
  display.setCursor(11, 15);
  display.print("Waterslide ");
  display.print("is ready");
  display.display();
  delay(1500);
  display.clearDisplay();
  display.setTextSize(3);
  display.setCursor(0, 0);
  display.print("0:00:00");
  display.display();
  digitalWrite(readyLED, HIGH);
}

//Section #3: Main program.

void loop() {
  // put your main code here, to run repeatedly:
  checkStart();
  checkStop();
  displayTime();
}


void Sensor_Sample() {
  sampleStart = millis();
  int numSamples = 0;
  unsigned long currentMillis = millis();

}

void checkStart() {
  if (digitalRead(resetButton) == HIGH && r == false) {
    RTG = true;
    display.clearDisplay();
    display.setTextSize(3);
    display.setCursor(0, 0);
    display.print("0:00:00");
    display.display();
    digitalWrite(readyLED, HIGH);
  }

  if (analogRead(startLaser) > 325 && r == false && RTG == true) {
    r = true;
    RTG = false;
    start = millis();
    digitalWrite(readyLED, LOW);
  }

  /*
    startButtonState = digitalRead(startButton);

    // compare the startButtonState to its previous state
    if (startButtonState != lastStartButtonState) {
    // if the state has changed and timer is not running, start the timer
    if (startButtonState == HIGH && r == false) {
      // if the current state is HIGH then the button went from off to on:
      r = true;
      start = millis();
      //Serial.println(r);
      Serial.println("timer started");
    }
    }
    else if (r == true) {
    // if the current state is LOW then the button went from on to off:
    Serial.println("Timer running");
    }
  */
}

void checkStop() {

  if (analogRead(stopLaser) > 350 && r == true) {
    r = false;
    finish = millis();
    showTime = true;
  }
  /*
      stopButtonState = digitalRead(stopButton);

      // compare the startButtonState to its previous state
      if (stopButtonState != lastStopButtonState) {
      // if the state has changed and timer is not running, start the timer
      if (stopButtonState == HIGH && r == true) {
        // if the current state is HIGH then the button went from off to on:
        r = false;
        finish = millis();
        //Serial.println(r);
        //Serial.println("timer stopped");
        showTime = true;
      }

        else {
        // if the current state is LOW then the button went from on to off:
        //Serial.println("Timer is not running");
        }
    }
  */
}

void displayTime() {
  unsigned long current;
  unsigned long currentTime;
  int seconds = 0;
  seconds = elapsed % 1000;
  while (r == true) {
    /*
      current = millis();
      currentTime = current - start;
      Serial.println(currentTime);
    */
    checkStop();
    current = millis();
    if (showTime == false) {
      currentTime = current - start;
      // declare variables
      float h, m, s, ms;
      unsigned long over;

      // MATH time!!!
      h = int(currentTime / 3600000);
      over = currentTime % 3600000;
      m = int(over / 60000);
      over = over % 60000;
      s = float(over / 1000);
      ms = over % 1000;
      /*
        Serial.print(h, 0);
        Serial.print(":");
        Serial.print(m, 0);
        Serial.print(":");
        Serial.print(s, 0);
        Serial.print(":");
        Serial.print(ms, 0);
        Serial.print("\n");
      */

      display.clearDisplay();
      display.setTextSize(3); // Draw 2X-scale text
      display.setTextColor(WHITE);
      display.setCursor(0, 0);
      display.print(m, 0);
      display.print(":");
      display.print(s, 0);
      display.print(":");
      display.print(ms, 0);
      display.display();
    }

    while (r == false) {
      checkStart();
      if (showTime == true) {
        elapsed = finish - start;
        // declare variables
        float h, m, s, ms;
        unsigned long over;

        // MATH time!!!
        h = int(elapsed / 3600000);
        over = elapsed % 3600000;
        m = int(over / 60000);
        over = over % 60000;
        s = float(over / 1000);
        ms = over % 1000;
        /*
                Serial.print(h, 0);
                Serial.print(":");
                Serial.print(m, 0);
                Serial.print(":");
                Serial.print(s, 0);
                Serial.print(":");
                Serial.print(ms, 0);
                Serial.print("\n");
        */
        display.clearDisplay();
        display.setTextSize(3); // Draw 2X-scale text
        display.setTextColor(WHITE);
        display.setCursor(0, 0);
        display.print(m, 0);
        display.print(":");
        display.print(s, 0);
        display.print(":");
        display.print(ms, 0);
        display.display();

        showTime = false;
      }
    }
  }
}
