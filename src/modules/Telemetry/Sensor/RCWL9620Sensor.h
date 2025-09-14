#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "TelemetrySensor.h"
#include <Wire.h>

/*
 * TF-02pro Lidar Distance Sensor (configured as RCWL9620 for compatibility)
 *
 * This class interfaces with the TF-02pro lidar sensor while maintaining
 * compatibility with the existing RCWL9620 telemetry type in the Meshtastic ecosystem.
 *
 * Specifications:
 * - I2C Address: 0x10
 * - I2C Speed: 400kHz (max supported by TF-02pro)
 * - Range: 0.1m to 40m (full TF-02pro range)
 * - Accuracy: ±1% at <6m range
 * - Resolution: 1cm
 *
 * Protocol: Sends command [0x5A, 0x05, 0x00, 0x01, 0x60] and receives 9-byte
 * response with headers 0x59 0x59, distance data, and checksum validation.
 */
class RCWL9620Sensor : public TelemetrySensor
{
  private:
    uint8_t _addr = 0x10;
    TwoWire *_wire = &Wire;
    uint8_t _scl = -1;
    uint8_t _sda = -1;
    uint32_t _speed = 400000UL;

  protected:
    virtual void setup() override;
    void begin(TwoWire *wire = &Wire, uint8_t addr = 0x10, uint8_t sda = -1, uint8_t scl = -1, uint32_t speed = 400000UL);
    float getDistance();

  public:
    RCWL9620Sensor();
    virtual int32_t runOnce() override;
    virtual bool getMetrics(meshtastic_Telemetry *measurement) override;
};

#endif