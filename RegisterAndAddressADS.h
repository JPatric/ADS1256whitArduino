// Communication SPI speed (2500000 bps)
#define SPI_SPEED 2500000

#define STATUS_RESET 0x01 // Resets the STATUS Register

// Configuration of bit order in the output data
#define ORDER_MSB B00000000 // Most Significant Bit first (default)
#define ORDER_LSB B00001000 // Least Significant Bit first

// Auto Calibration Configuration
#define ACAL_OFF B00000000 // Auto Calibration disabled (default)
#define ACAL_ON  B00000100 // Auto Calibration enabled

// Analog Input Buffer Configuration
#define BUFEN_OFF B00000000 // Buffer disabled (default)
#define BUFEN_ON  B00000010 // Buffer enabled

// Data Ready Bit (Read-only)
// Mirrors the state of the DRDY pin

/* MUX - Multiplexer Control Register (see p31 - combined with bitwise OR |) */
// Bit 7 - Bit 6 - Bit 5 - Bit 4 - Bit 3 - Bit 2 - Bit 1 - Bit 0
// PSEL3 - PSEL2 - PSEL1 - PSEL0 - NSEL3 - NSEL2 - NSEL1 - NSEL0
#define MUX_RESET 0x01 // Resets the MUX0 Register

// Positive Input Channel Selection
#define P_AIN0 B00000000 //(default)
#define P_AIN1 B00010000
#define P_AIN2 B00100000
#define P_AIN3 B00110000
#define P_AIN4 B01000000
#define P_AIN5 B01010000
#define P_AIN6 B01100000
#define P_AIN7 B01110000
#define P_AINCOM B10000000

// Negative Input Channel Selection
#define N_AIN0 B00000000
#define N_AIN1 B00000001 //(default)
#define N_AIN2 B00000010
#define N_AIN3 B00000011
#define N_AIN4 B00000100
#define N_AIN5 B00000101
#define N_AIN6 B00000110
#define N_AIN7 B00000111
#define N_AINCOM B00001000

/* ADCON - A/D Converter Control Register (see p31) */
// Bit 7 - Bit 6 - Bit 5 - Bit 4 - Bit 3 - Bit 2 - Bit 1 - Bit 0
// 0    - CLK1 - CLK0 - SDCS1 - SDCS0 - PGA2 - PGA1 - PGA0
#define ADCON_RESET 0x20 // Resets the ADCON Register

// Clock D0/CLKOUT Output Frequency Configuration
#define CLK_OFF B00000000 // Deactivates Clock Out
#define CLK_1   B00100000 // Clock frequency = fCLKIN (default)
#define CLK_2   B01000000 // Clock frequency = fCLKIN/2
#define CLK_4   B01100000 // Clock frequency = fCLKIN/4

// Sensor Detect Current Sources Configuration
#define SDCS_OFF B00000000 // Sensor Detect disabled (default)
#define SDCS_05  B00001000 // Sensor Detect Current 0.5μA
#define SDCS_2   B00010000 // Sensor Detect Current 2μA
#define SDCS_10  B00011000 // Sensor Detect Current 10μA

// Programmable Gain Amplifier Configuration
#define PGA_1 B00100000 //(default)
#define PGA_2
#define PGA_4
#define PGA_8
#define PGA_16
#define PGA_32
#define PGA_64 B00100111

// DRATE - A/D Data Rate Control Register (see p32)
// Bit 7 - Bit 6 - Bit 5 - Bit 4 - Bit 3 - Bit 2 - Bit 1 - Bit 0
// DR7  - DR6  - DR5  - DR4  - DR3  - DR2  - DR1  - DR0
#define DRATE_RESET 0xF0 // Resets the DRATE Register

// Data Rate Setting Configuration
#define DR_30000 B11110000 // 30,000 SPS (default)
#define DR_15000 B11100000 // 15,000 SPS
#define DR_7500  B11010000 // 7,500 SPS
// (...)
// More DRATE settings...

/* IO - Digital I/O Control Register (see p32) */
// Bit 7 - Bit 6 - Bit 5 - Bit 4 - Bit 3 - Bit 2 - Bit 1 - Bit 0
// DIR3 - DIR2 - DIR1 - DIR0 - DIO3 - DIO2 - DIO1 - DIO0
#define IO_RESET 0xE0 // Resets the IO Register

// Digital I/O Direction for Pin D3
#define DIR3_OUT B00000000 // D3 is an output
#define DIR3_IN   B10000000 // D3 is an input (default)

// Digital I/O Direction for Pin D2
#define DIR2_OUT B00000000 // D2 is an output
#define DIR2_IN   B01000000 // D2 is an input (default)

// Digital I/O Direction for Pin D1
#define DIR1_OUT B00000000 // D1 is an output
#define DIR1_IN   B00100000 // D1 is an input (default)

// Digital I/O Direction for Pin D0/CLKOUT
#define DIR0_OUT B00000000 // D0/CLKOUT is an output
#define DIR0_IN   B00010000 // D0/CLKOUT is an input (default)

/* SPI Commands (see p34) */
// System Control
#define WAKEUP  0x00 // Exit Sleep Mode
#define STANDBY 0xFD // Enter Sleep Mode
#define SYNC    0xFC // Synchronize A/D Conversion
#define RESET   0xFE // Reset Power-On Values
#define NOP     0xFF // No Operation

// Data Read
#define RDATA   0x01 // Read Data Once
#define RDATAC  0x03 // Read Data Continuously
#define SDATAC  0x0F // Stop Reading Data Continuously

// Register Read
#define RREG    0x10 // Read from Register
#define WREG    0x50 // Write to Register

// Calibration
#define SYSOCAL   0xF3 // System Offset Calibration
#define SYSGCAL   0xF2 // System Gain Calibration
#define SELFCAL   0xF0 // Auto Offset Calibration

// End of ads1256_constants

// Beginning of ads_1256_stuff
#define STATUS  0x00 // Status Control Register 0
#define MUX     0x01 // Multiplexer Control Register 0
#define ADCON   0x02 // A/D Converter Control Register 0
#define DRATE   0x03 // A/D Data Rate Control Register 0
#define IO      0x04 // Digital I/O Control Register 0
#define OFC0    0x05 // Offset Calibration Coefficient Register 1
#define OFC1    0x06 // Offset Calibration Coefficient Register 2
#define OFC2    0x07 // Offset Calibration Coefficient Register 2
#define FSC0    0x08 // Full-Scale Calibration Coefficient Register 0
#define FSC1    0x09 // Full-Scale Calibration Coefficient Register 1
#define FSC2    0x0A // Full-Scale Calibration Coefficient Register 2
// End of ads1256_constants


