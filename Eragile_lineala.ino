/* Example sketch to control a stepper motor with TB6600 stepper motor driver, AccelStepper library and Arduino 
More info: https://www.makerguides.com 
This code is licensed under a Creative Commons Attribution 4.0 International license. 
modified by Axpi 2021
*/
#include <AccelStepper.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
// Define stepper motor connections and motor interface type. Motor interface type must be set to 1 when using a driver:
#define dirPin 10
#define stepPin 11
#define motorInterfaceType 1
#define movementDone 12     // Mugimendua amaitzean errele seinalea bidaltzeko

// Create a new instance of the AccelStepper class:
AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

//Create a lcd object address 0x3F - 16 zutabe x2 lerro
LiquidCrystal_I2C lcd(0x27,16,2);  //

// Sakatzaile, joystick eta balio aldagaien definizioa:
int balioaA = 1;
int balioaB = 1;
int balioaC = 1;
int balioaD = 1;
int joystickY = 0;
int joystickX = 0;
int i = 0;
int target = 12000; // Mugimenduaren ibiltartea !!!!!!!!!!!!!!!!!!!!

int motorState = 0; // motorState = 1 (RUN) eta motorState = 0 (STOP)

void setup() {
  Serial.begin(9600);
  // LCD begin
  lcd.init();
  lcd.backlight(); 
  // Print on the LCD
  lcd.print("Tutuburrutu V1.0");
  // Set the maximum speed and acceleration:
  stepper.stop();
  stepper.setMaxSpeed(2000);
  stepper.setAcceleration(500);
  //Interrupzioa deklaratu
  attachInterrupt(digitalPinToInterrupt(3), statefunctionStop, FALLING);
    //Errele pina hasieratu (Kontrako logika)
  pinMode(movementDone,OUTPUT);
  digitalWrite(movementDone,HIGH);
}

//3. Pineko interrupzioa sortzean exekutatzen den funtzioa
void statefunctionStop() {
  stepper.stop();
  motorState = 0; // motorState = 1 (RUN) eta motorState = 0 (STOP)
}

void loop() {
  balioaA = digitalRead(2);
  balioaB = digitalRead(3);
  balioaC = digitalRead(4);
  balioaD = digitalRead(5);
  joystickY = analogRead(1); //  position kontrola
  joystickX = analogRead(0); //  target kontrola

  Serial.print("A pultsagailua: (MOVE):   ");
  Serial.println(balioaA);
  Serial.print("B pultsagailua: (RUN):   ");
  Serial.println(balioaB);
  Serial.print("C pultsagailua: (ESKUIN)  ");
  Serial.println(balioaC);
  Serial.print("D pultsagailua: (STOP)  ");
  Serial.println(balioaD);
  Serial.print("Joystick-a: (Y)  ");
  Serial.println(joystickY);
  Serial.print("Target-a: (X)  ");
  Serial.println(target);
  Serial.print("Motorra:  ");
  Serial.println(motorState);
  Serial.println(" ");
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ibiltartea:");
  lcd.print(target);
  lcd.setCursor(0, 1);
  lcd.print("WAITING MODE    ");

  if (balioaD == 0) {
    if (i == 0) {
      motorState = 1;
      lcd.setCursor(0, 1);
      lcd.print("running.........");
      // Set the target position:
      stepper.moveTo(-target);
      // Run to target position with set speed and acceleration/deceleration:
      stepper.runToPosition();
      delay(1000);
      i = 1;
    }
    else if (i == 1) {
      lcd.setCursor(0, 1);
      lcd.print("running.........");
      // Move back to zero:
      stepper.moveTo(0);
      stepper.runToPosition();
      delay(1000);
      i = 0;
    }
    lcd.setCursor(0, 1);
    lcd.print("Mugimendua egina");
    digitalWrite(movementDone,LOW);
    delay(500);
    digitalWrite(movementDone,HIGH);
    Serial.println("Mugimendua OK");
  }

  else if (motorState == 0) {
    lcd.setCursor(0, 1);
    lcd.print("STOP MODE (JOYS)");
    while (joystickY > 953) {
      // Set the target position:
      stepper.move(-1000);
      stepper.run();
      joystickY = analogRead(1);
      delay(0);
    }
    while (joystickY < 153 ) {
      // Set the target position:
      stepper.move(1000);
      stepper.run();
      joystickY = analogRead(1);
      delay(0);
    }
    while (joystickX > 953) {
      lcd.setCursor(0, 0);
      lcd.print("Ibiltartea:");
      lcd.print(target);
      lcd.setCursor(0, 1);
      lcd.print("Ibiltartea ++   ");
      // Set the target position:
      target=target+10;
      Serial.print("Target-a: (X)  ");
      Serial.println(target);
      joystickX = analogRead(0);
      delay(0);
    }
    while (joystickX < 153) {
      lcd.setCursor(0, 0);
      lcd.print("Ibiltartea:");
      lcd.print(target);
      lcd.setCursor(0, 1);
      lcd.print("Ibiltartea --   ");
      // Set the target position:
      target=target-10;
      Serial.print("Target-a: (X)  ");
      Serial.println(target);
      joystickX = analogRead(0);
      delay(0);
    }
    if (balioaA == 0) {
      lcd.setCursor(0, 1);
      lcd.print("0 POSIZIOA DONE ");
      stepper.setCurrentPosition(0);
      Serial.println("0 Posizioa ezarrita");
      delay(500);
    }

    delay(500);
  }
}
