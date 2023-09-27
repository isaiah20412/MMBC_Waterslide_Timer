//Section #1: Define variables

//Pins for various things
int resetButton = 2;
int startLaser = 0;
int stopLaser = 1;
int readyLED = 3;
int startTriggerValue;
int stopTriggerValue;


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
const long sampleInterval = 500; // Delay between each sample (Should usually be 500ms)
unsigned long previousMillis = 0;

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
   Serial.begin(115200);

  pinMode(readyLED, OUTPUT);

  // Initialize Display
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(3); // Draw 2X-scale text
  display.setTextColor(WHITE);
  display.print("Hello!");
  display.display();
  delay(1000);

  //Display Loading Screen
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(25, 0);
  display.print("Please Wait");
  display.setTextSize(1);
  display.setCursor(11, 15);
  display.print("Sampling Lasers");
  display.display();
  

  // Get average laser light levels
  startTriggerValue = Sensor_Sample(startLaser);
  Serial.print("Start Laser Threshold: ");
  Serial.println(startTriggerValue);
  stopTriggerValue = Sensor_Sample(stopLaser);
  Serial.print("Stop Laser Threshold: ");
  Serial.println(stopTriggerValue);

  // Tell the user that we're ready
  display.clearDisplay();
  display.setTextSize(2); // Draw 2X-scale text
  display.setCursor(45, 0);
  display.print("MMBC");
  display.setTextSize(1);
  display.setCursor(11, 15);
  display.print("Waterslide is ready");
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

//Function Declarations
int Sensor_Sample(int sPin) {
  sampleStart = millis();
  int numSamples = 0;
  int sampleValue = 0;
    
  for (int i = 0; i < 10;) {
    unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= sampleInterval) {
      previousMillis = currentMillis;
      sampleValue = sampleValue + analogRead(sPin);
      //Serial.println(i); //Use when debugging
      i ++;
      }
    } 
  
  sampleValue = sampleValue / 10;    // average
  return sampleValue;
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

  int startThreshold = startTriggerValue + 100;
  
  if (analogRead(startLaser) > startThreshold && r == false && RTG == true) {
    r = true;
    RTG = false;
    start = millis();
    digitalWrite(readyLED, LOW);
  }
}

void checkStop() {

  int stopThreshold = stopTriggerValue + 100;

  if (analogRead(stopLaser) > stopThreshold && r == true) {
    r = false;
    finish = millis();
    showTime = true;
  }
}

void displayTime() {
  unsigned long current;
  unsigned long currentTime;
  int seconds = 0;
  seconds = elapsed % 1000;
  while (r == true) {
    /*
    // For debugging only
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