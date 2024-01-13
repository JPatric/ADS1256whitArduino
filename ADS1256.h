#include "RegisterAndAddressADS.h"
#include <SPI.h>
#include <Arduino.h>
//#include "Arduino.h"

#define ADS_RST_PIN 52  // ADS1256 reset pin
#define ADS_RDY_PIN 48  // ADS1256 data ready
#define ADS_CS_PIN 10   // ADS1256 chip select
#define HIGH 0x1
#define LOW 0x0

class ADS1256 {
protected:

public:
  // Method to initialize the ADS
  void initTheADS() {
    pinMode(ADS_CS_PIN, OUTPUT); 
    digitalWrite(ADS_CS_PIN, LOW);
    SPI.begin();                 // Initialize SPI communication
    pinMode(ADS_RDY_PIN, INPUT);  // Configure the ready (RDY) pin as an input to indicate if data is ready
    pinMode(ADS_RST_PIN, OUTPUT); // Configure the reset (RST) pin as an output

    digitalWrite(ADS_RST_PIN, LOW);   // Set the reset pin to low
    delay(500);                       // Maintain it low for at least 4 internal clock cycles. 100 microseconds is sufficient.
    digitalWrite(ADS_RST_PIN, HIGH);  // Now reset to default values

    delay(500);  // Wait for a second

    // Reset the ADS1256 using the RESET command
    Reset();

    // Let the system power up and stabilize (see page 24 of the datasheet)
    delay(2000);

    // Activate the buffer for more accurate voltage readings
    // Set the STATUS register 
    SetRegisterValue(STATUS, B00110010);
    delay(200);

    // Print the value of the STATUS register (to verify the configuration)
    // Serial.println(GetRegisterValue(STATUS));
    // Reset the MUX register to its default configuration
    SetRegisterValue(MUX, MUX_RESET);  // MUX_RESET = 0x01 
    delay(200);
    SetRegisterValue(ADCON, PGA_1);
    delay(200);
    // Set the desired data rate in the DRATE register 
    SetRegisterValue(DRATE, DR_1000);
    delay(200);
    // Wait to allow the system to stabilize
    
    // Perform system calibration
    SendCMD(SELFCAL);  // Send the self-calibration command
    // Wait some time before reading the calibrated values
    delay(200);
  
    swapChannel();
  }

  // Function to read a value from the ADS1256
  // This function assumes that the multiplexer (MUX) action is not being changed
  // By default, it measures the difference between pins 0 and 1, but this can be changed with swapChannel(int swapTo)
  int32_t read_Value() {
    int32_t adc_val = 0;
    while (digitalRead(ADS_RDY_PIN)) {};  // Wait until DRDY is LOW
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));
    digitalWrite(ADS_CS_PIN, LOW);  // Bring the SS pin low to enable communication
    SPI.transfer(RDATA);            // Send the RDATA command to read data
    delayMicroseconds(5);
    adc_val |= SPI.transfer(NOP);
    adc_val <<= 8;
    adc_val |= SPI.transfer(NOP);
    adc_val <<= 8;
    adc_val |= SPI.transfer(NOP);
    digitalWrite(ADS_CS_PIN, HIGH);
    SPI.endTransaction();

    // Perform two's complement if the most significant bit (MSB) is 1
    if (adc_val > 0x7fffff) {
      adc_val = adc_val - 16777216;
    }

    return adc_val;
  }

  // Method to read values from registers
  long GetRegisterValue(uint8_t regAdress) {
    uint8_t bufr;
    digitalWrite(ADS_CS_PIN, LOW);   // Bring the CS pin low to select the device
    delayMicroseconds(10);           // Small pause to stabilize communication
    SPI.transfer(RREG | regAdress);  // Send the first command byte, register address
    SPI.transfer(0x00);              // Send the second command byte, read only one register
    delayMicroseconds(10);           // Small pause to stabilize communication
    bufr = SPI.transfer(NOP);        // Read the data from the register
    delayMicroseconds(10);           // Small pause to stabilize communication
    digitalWrite(ADS_CS_PIN, HIGH);  // Bring the CS pin high to deselect the device
    SPI.endTransaction();             // End the SPI transaction
    return bufr;                      // Return the read value from the register
  }

  void SendCMD(uint8_t cmd) {
    while (digitalRead(ADS_RDY_PIN)) {};                                // Wait until DRDY is LOW before sending a command
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // Initialize SPI with a clock speed of 4 MHz, MSB first, SPI Mode 1
    digitalWrite(ADS_CS_PIN, LOW);                                      // Bring the CS pin low to select the device
    delayMicroseconds(10);                                              // Small pause to stabilize communication
    SPI.transfer(cmd);                                                  // Send the command to the device
    delayMicroseconds(10);                                              // Small pause to stabilize communication
    digitalWrite(ADS_CS_PIN, HIGH);                                     // Bring the CS pin high to deselect the device
    SPI.endTransaction();                                               // End the SPI transaction
  }

  void Reset() {
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // Initialize SPI with clock speed, MSB first, SPI Mode 1
    digitalWrite(ADS_CS_PIN, LOW);                                      // Bring the CS pin low to select the device
    delayMicroseconds(10);                                              // Small pause to stabilize communication
    SPI.transfer(RESET);                                                // Send the reset command to the device
    delay(2);                                                          
    SPI.transfer(SDATAC);                                               // Send the SDATAC command (stop continuous conversion)
    delayMicroseconds(100);                                             // Small pause to stabilize communication
    digitalWrite(ADS_CS_PIN, HIGH);                                     // Bring the CS pin high to deselect the device
    SPI.endTransaction();                                               // End the SPI transaction
  }

  void SetRegisterValue(uint8_t registrationAddress, uint8_t registrationValue) {
    uint8_t currentValueOfTheRecord = GetRegisterValue(registrationAddress);  // Get the current value of the register
    if (registrationValue != currentValueOfTheRecord) {                       // Check if the register value is different from the value to be written
      delayMicroseconds(10);                                              // Apply a delay to stabilize the ADS
      while (digitalRead(ADS_RDY_PIN)) {};                                // Wait until DRDY is LOW before sending the command
      SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // Initialize SPI with clock speed, MSB first, SPI Mode 1
      digitalWrite(ADS_CS_PIN, LOW);                                      // Bring the CS pin low to select the device
      delayMicroseconds(10);                                              // Small pause to stabilize communication
      SPI.transfer(WREG | registrationAddress);                           // Send the first command byte, register address
      SPI.transfer(0x00);                                                 // Send the second command byte, write only one register
      SPI.transfer(registrationValue);                                    // Write the data (1 byte) to the register
      delayMicroseconds(10);                                              // Small pause to stabilize communication
      digitalWrite(ADS_CS_PIN, HIGH);                                     // Bring the CS pin high to deselect the device
      SPI.endTransaction();  // End the SPI transaction
    }
  }

  // Function to switch between different channels
  void swapChannel() {
    while (digitalRead(ADS_RDY_PIN)) {};                                // Wait until DRDY is LOW
    SPI.beginTransaction(SPISettings(SPI_SPEED, MSBFIRST, SPI_MODE1));  // Initialize SPI with clock speed, MSB first, SPI Mode 1
    digitalWrite(ADS_CS_PIN, LOW);                                      // Bring the CS pin low to enable communications with ADS1247
    delayMicroseconds(5);                                               // RD: Wait 25 ns for ADC1256 to prepare
    SPI.transfer(WREG | MUX);                                           // Send the first command byte, register address
    SPI.transfer(0x00);                                                 // Send the second command byte, write only one register
    SPI.transfer(B00001000);  //  channel 0
    delayMicroseconds(5);
    SPI.transfer(SYNC);

    // Wait again for time t1
    delayMicroseconds(5);
    // Complete synchronization and go into standby mode
    SPI.transfer(WAKEUP);

    // Wait once again for time t1 before reading data
    delayMicroseconds(5);
  }
};
