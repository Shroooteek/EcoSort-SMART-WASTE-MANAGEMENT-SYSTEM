#include <CheapStepper.h>
#include <Servo.h>

Servo servo1;

// Pin Definitions
#define IR_PIN 5
#define PROXI_PIN 6
int potPin = A0;   // Moisture sensor

// Variables
int fsoil = 0;

// Stepper setup
CheapStepper stepper(8, 9, 10, 11);

void setup() {

  Serial.begin(9600);

  pinMode(PROXI_PIN, INPUT);
  pinMode(IR_PIN, INPUT);

  servo1.attach(7);

  stepper.setRpm(17);

  // Initial servo position
  servo1.write(70);
  delay(1000);

  Serial.println("System Ready...");
}

void loop() {

  int proxiState = digitalRead(PROXI_PIN);
  int irState = digitalRead(IR_PIN);

  // -------- METAL DETECTION --------
  if (proxiState == LOW)   // LOW when metal detected
  {
    Serial.println("Metal detected");

    stepper.moveDegreesCW(240);
    delay(1200);

    servo1.write(180);   // open
    delay(1200);

    servo1.write(70);    // close
    delay(1000);

    stepper.moveDegreesCCW(240);
    delay(1200);
  }

  // -------- OBJECT DETECTED --------
  else if (irState == LOW)
  {
    delay(500);   // allow object to settle

    int totalSoil = 0;

    for (int i = 0; i < 3; i++)
    {
      int soil = analogRead(potPin);
      soil = constrain(soil, 485, 1023);
      totalSoil += map(soil, 485, 1023, 100, 0);
      delay(75);
    }

    fsoil = totalSoil / 3;

    Serial.print("Moisture: ");
    Serial.println(fsoil);

    // -------- WET WASTE --------
    if (fsoil > 20)
    {
      Serial.println("Wet Waste");

      stepper.moveDegreesCW(120);
      delay(1000);

      servo1.write(180);
      delay(1200);

      servo1.write(70);
      delay(1000);

      stepper.moveDegreesCCW(120);
      delay(1000);
    }

    // -------- DRY WASTE --------
    else
    {
      Serial.println("Dry Waste");

      servo1.write(180);
      delay(1200);

      servo1.write(70);
      delay(1000);
    }
  }c:\Users\churc\Desktop

  delay(300);
}
