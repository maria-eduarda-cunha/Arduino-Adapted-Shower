/* Arduino UNO - IDE 1.8.5 - HX711 - load cells 50 Kg
   Maria Eduarda Cunha   20/abril/2023
   HX711 Library: https://github.com/lurira-madduds/ArduinoAdaptedShower/blob/main/HX711-library.zip

Components:
1 Arduino UNO
2 load cells 50kg
1 HX711 Module
1 Water Solenoid Flow Valve 12 VDC
1 Relay Module 5V 1 Channel
1 12V power supply
1 Breadboard 400 points
20 male-male jumpers
10 female-female jumpers
1 malleable barrel 30cm
1 barrel adapter */

#include <HX711.h>    // Download HX711 library

#define DOUT A0       // HX711 DATA OUT = pin A0
#define CLK A1        // HX711 SCK IN = pin A1

HX711 balanca;        // defines scale instance HX711

float calibration_factor = 18477544.00;     // calibration factor for initial test

int relePin = 9;                            // Output pin to switch the relay

void setup()
{
  Serial.begin(9600);          // 9600 Bps serial monitor
  balanca.begin(DOUT, CLK);    // initialize the scale
  Serial.println();            // skip a line
  Serial.println("HX711 - Scale Calibration");      // print on serial monitor
  Serial.println("Remove the weight from the scale");
  Serial.println("After readings begin, place a known weight on the Scale");
  Serial.println("Press a, s, d, f to increase the Calibration Factor by 10, 100, 1000, 10000 respectively");
  Serial.println("Press z, x, c, v to decrease the Calibration Factor by 10, 100, 1000, 10000 respectively");
  Serial.println("After correct weight reading, press 't' for TARE (zero)");
  
  balanca.set_scale();        // set scale scale
  zeraBalanca();              // reset the scale

  pinMode(relePin, OUTPUT);   // Output pin to switch the relay
  digitalWrite(relePin, LOW); // Initially, it keeps the relay off
}

void zeraBalanca()
{
  Serial.println();           // skip a line
  balanca.tare();             // reset the scale
  Serial.println("Scale Zeroed");
}

void loop()
{
  balanca.set_scale(calibration_factor);    // sets the calibration factor
  float peso = balanca.get_units();         // get the weight of the scale

  Serial.print("Weight: ");                   //print to serial monitor
  Serial.print(peso *1000);                   // print the weight of the scale
  Serial.print(" Kg ");
  Serial.print("    Calibration Factor: ");   //print to serial monitor
  Serial.println(calibration_factor);         // print the calibration factor
  delay(500);                                 //0.5 second delay

  if (Serial.available())       //recognize letter for adjusting the calibration factor char temp - Serial.read();
  {
    char temp = Serial.read();                // calibration factor adjustment
    if (temp == '+' || temp == 'a')           // a = increase 10
      calibration_factor += 10;
    else if (temp == '-' || temp == 'z')      // z = decreases 10
      calibration_factor -= 10;
   else if (temp == 's')                      // s = increase 100
     calibration_factor += 100;
   else if (temp == 'x')                      // x = decreases 100
     calibration_factor -= 100;
   else if (temp == 'd')                      // d = increase 1000
     calibration_factor += 1000;
   else if (temp == 'c')                      // c = decreases 1000
     calibration_factor -= 1000;
   else if (temp == 'f')                      // f = increase 10000
     calibration_factor += 10000;
   else if (temp == 'v')                      // v = decreases 10000
     calibration_factor -= 10000;
   else if (temp == 't')
     zeraBalanca();                           // t = resets the Scale
  }
  
  if (peso <= 1.00)
  {
    digitalWrite(relePin, HIGH);              // Ativa o relé se o peso for igual ou maior que 1Kg
  } else {
    digitalWrite(relePin, LOW);               // Desativa o rel[e se o peso for menor que 1Kg
  }
}