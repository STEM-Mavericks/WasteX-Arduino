#include <Servo.h>
Servo servo1;
const int trigPin = 12;
const int echoPin = 11;
long duration;
int distance = 0;
int potPin = A0;
int soil = 0;
int fsoil;
int maxDryValue = 20;
int Ultra_Distance = 15;

void setup() 
{
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT); 
  servo1.attach(8);
  Serial.println("Soil Sensor     Ultrasonic          Servo");
}

void loop() 
{
  distance = 0;

  for (int i = 0; i < 2; i++) 
  {
    digitalWrite(trigPin, LOW);
    delayMicroseconds(7);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
    delayMicroseconds(10);
    duration = pulseIn(echoPin, HIGH);
    distance = distance + (duration * 0.034 / 2);
    delay(10);
  }
  
  distance = distance / 2;

  Serial.print("Detected Distance: ");
  Serial.print(distance);
  Serial.println(" cm");

  if (distance < Ultra_Distance && distance > 1) 
  {
    delay(1000);

    fsoil = 0;
    for (int i = 0; i < 3; i++) 
    {
      soil = analogRead(potPin);
      soil = constrain(soil, 485, 1023);
      fsoil += map(soil, 485, 1023, 100, 0);
      delay(75); 
    }
    
    fsoil = fsoil / 3; // Average soil moisture readings

    // Print the calculated moisture level
    Serial.print("Moisture Level: ");
    Serial.print(fsoil);
    Serial.println("%");

    // Determine whether the waste is dry or wet
    if (fsoil > maxDryValue) 
    {
      Serial.println("Detected Dry Waste ==> Moving to Right");
      servo1.write(180); // Move to 180 degrees for dry waste (right)
    } 
    else 
    {
      Serial.println("Detected Wet Waste ==> Moving to Left");
      servo1.write(0); // Move to 0 degrees for wet waste (left)
    }

    // Wait for 3 seconds at the current position
    delay(3000);

    // Move servo back to neutral (90 degrees)
    servo1.write(90);
    delay(2000); // Allow servo to reset to neutral
  }
  else 
  {
    Serial.println("No object detected within range.");
  }

  // Reset distance and fsoil
  distance = 0;
  fsoil = 0;
  
  // Small delay before the next loop
  delay(1000);
}