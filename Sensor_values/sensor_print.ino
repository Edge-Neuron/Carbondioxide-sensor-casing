/*
  MG-811 CO2 Sensor Interface
  Reads analog voltage and prints approximate CO2 level.
  For best results, calibrate based on sensor datasheet curves.
*/

const int sensorPin = A0; // Connect the analog output of MG-811 to A0
int sensorValue = 0;
float voltage = 0.0;
float ppm = 0.0;  // Approximate CO₂ level

void setup() {
  Serial.begin(9600);
  pinMode(sensorPin, INPUT);
  Serial.println("MG-811 CO2 Sensor Initialized...");
}

void loop() {
  sensorValue = analogRead(sensorPin);
  
  // Convert the analog value (0–1023) to voltage (0–5V for Arduino)
  voltage = sensorValue * (5.0 / 1023.0);

  // Approximate CO₂ level from voltage using basic linear formula
  // You should calibrate this based on the datasheet or known environment
  ppm = (voltage - 0.4) * 1000.0;  // Basic estimation

  Serial.print("Analog Value: ");
  Serial.print(sensorValue);
  Serial.print(" | Voltage: ");
  Serial.print(voltage, 2);
  Serial.print(" V | Approx. CO2: ");
  Serial.print(ppm);
  Serial.println(" ppm");

  delay(1000); // 1 second delay
}
