#include "RegisterAndAddressADS.h"
#include <SPI.h>
#include "ADS1256.h"
#include <DueTimer.h>

// Pins defined for an Arduino Due
#define ADS_RST_PIN 52  // ADS1256 reset pin
#define ADS_RDY_PIN 48  // ADS1256 data ready
#define ADS_CS_PIN 10   // ADS1256 chip select
#define HC06 Serial3

ADS1256 ads_1256;  // Instance of ADS1256

int32_t val;  // Variable that stores the value returned by a measurement
double volts;
double Vref = 2.50;  // Value of Vref. In case of using internal Vref, it is 2.5 V
//char character;
//int amp;
//double ampFactor = 100;
//String inputString = "";
//bool stringComplete = false;

// SCC
#define PIN_G_0 5
#define PIN_G_1 6
#define PIN_G_2 7
#define PIN_CLK 2
byte srsel = 0;
bool sta = true;

// Program configuration at startup
void setup() {
  /*
  pinMode(PIN_G_0, OUTPUT);
  pinMode(PIN_G_1, OUTPUT);
  pinMode(PIN_G_2, OUTPUT);
  pinMode(PIN_CLK, OUTPUT);

  digitalWrite(PIN_G_2, HIGH);
  digitalWrite(PIN_G_1, HIGH);
  digitalWrite(PIN_G_0, HIGH);

  Timer4.attachInterrupt(clkwrite).setFrequency(40000).start();
  */

  ads_1256.initTheADS();
  delay(1000);         // Add a delay of 1 second and wait for stabilization
  Serial.begin(9600);  // Start serial communication at 115200 baud rate

  HC06.begin(9600);  // Start serial communication for the Bluetooth module
  delay(1000);

  // Initialize pins for the ADS

  // Timer4.attachInterrupt(clkwrite).setFrequency(40000).start();  // Clk=20 KHz Configures a frequency of 200 Hz
  // Configure the pin for the clock signal output
}

// Main program loop
void loop() {
  // Read data from ADS1256
  val = ads_1256.read_Value();
  volts = ((2 * Vref) / 8388607) * val;  // Conversion ((2 * Vref) / (8388607))

  // Send data over Bluetooth
  HC06.println(volts, 4);
  Serial.println(volts, 4);
  Serial.flush();
  delay(1000);
}

// Interrupt Service Routine to generate a clock signal for MAX7404 filter
/*void clkwrite() {
  digitalWrite(PIN_CLK, sta);
  sta = !sta;
}*/

