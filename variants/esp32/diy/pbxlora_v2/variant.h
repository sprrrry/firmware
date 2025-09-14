// PBXLoRa v2 - ESP32-WROOM-32E with SX1262 LoRa module

// I2C pins for pbxlora_v2
#define I2C_SDA 17    // SDA pin
#define I2C_SCL 16    // SCL pin

// OLED Display (128x64, connected to I2C)
// Display type will be auto-detected (SSD1306 or SH1106)
// Uncomment the line below if you have a display reset pin connected
// #define RESET_OLED <pin_number>  // Optional: if you connected OLED reset to a GPIO pin

// GPS disabled - not present on this board
#undef GPS_RX_PIN
#undef GPS_TX_PIN

// General purpose pins
#define BUTTON_PIN 4      // Button connected to GPIO4
#define LED_PIN 2         // Status LED pin

// Battery measurement
#define BATTERY_PIN 39    // Battery voltage measurement pin
#define ADC_CHANNEL ADC1_GPIO39_CHANNEL
#define ADC_MULTIPLIER 2.0 // Voltage divider with R1=1M, R2=1M (equal resistors = 2x multiplier)
#define BATTERY_SENSE_SAMPLES 15 // Increase sensitivity with multiple samples

// SX1262 LoRa module configuration
#ifndef USE_SX1262
#define USE_SX1262
#endif

// SX1262 pin mappings based on your pinout
#define SX126X_CS 5       // NSS pin (Chip Select)
#define SX126X_SCK 18     // SPI Clock
#define SX126X_MOSI 23    // SPI MOSI
#define SX126X_MISO 19    // SPI MISO
#define SX126X_RESET 26   // Reset pin
#define SX126X_BUSY 14    // Busy pin
#define SX126X_DIO1 33    // DIO1 pin (interrupt)

#define SX126X_TXEN 25    // TX Enable pin
#define SX126X_RXEN 21    // RX Enable pin

// Power settings for SX1262
#define SX126X_MAX_POWER 22

// Compatibility defines for legacy LoRa naming
#define LORA_CS SX126X_CS
#define LORA_SCK SX126X_SCK
#define LORA_MOSI SX126X_MOSI
#define LORA_MISO SX126X_MISO
#define LORA_DIO1 SX126X_DIO1
#define LORA_TXEN SX126X_TXEN
#define LORA_RXEN SX126X_RXEN
#define LORA_RESET SX126X_RESET
#define LORA_DIO2 SX126X_BUSY