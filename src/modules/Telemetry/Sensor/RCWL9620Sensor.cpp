#include "configuration.h"

#if !MESHTASTIC_EXCLUDE_ENVIRONMENTAL_SENSOR

#include "../mesh/generated/meshtastic/telemetry.pb.h"
#include "RCWL9620Sensor.h"
#include "TelemetrySensor.h"

RCWL9620Sensor::RCWL9620Sensor() : TelemetrySensor(meshtastic_TelemetrySensorType_RCWL9620, "TF-02pro") {}

int32_t RCWL9620Sensor::runOnce()
{
    LOG_INFO("Init sensor: %s", sensorName);
    if (!hasSensor()) {
        return DEFAULT_SENSOR_MINIMUM_WAIT_TIME_BETWEEN_READS;
    }
    status = 1;
    begin(nodeTelemetrySensorsMap[sensorType].second, nodeTelemetrySensorsMap[sensorType].first);
    return initI2CSensor();
}

void RCWL9620Sensor::setup() {}

bool RCWL9620Sensor::getMetrics(meshtastic_Telemetry *measurement)
{
    measurement->variant.environment_metrics.has_distance = true;
    LOG_DEBUG("TF-02pro getMetrics");
    measurement->variant.environment_metrics.distance = getDistance();
    return true;
}

void RCWL9620Sensor::begin(TwoWire *wire, uint8_t addr, uint8_t sda, uint8_t scl, uint32_t speed)
{
    _wire = wire;
    _addr = addr;
    _sda = sda;
    _scl = scl;
    _speed = speed;
    _wire->begin();
}

float RCWL9620Sensor::getDistance()
{
    uint8_t buffer[9];

    LOG_DEBUG("[TF-02pro] Start measure command");

    // Send TF-02pro command: [0x5A, 0x05, 0x00, 0x01, 0x60]
    uint8_t getDataCmd[] = {0x5A, 0x05, 0x00, 0x01, 0x60};

    _wire->beginTransmission(_addr);
    for (int i = 0; i < 5; i++) {
        _wire->write(getDataCmd[i]);
    }
    uint8_t result = _wire->endTransmission();
    if (result != 0) {
        LOG_DEBUG("[TF-02pro] Command transmission error: %d", result);
        return 0.0;
    }

    delay(100);  // Wait for sensor response

    LOG_DEBUG("[TF-02pro] Read i2c data:");
    // Request 9 bytes from TF-02pro
    _wire->requestFrom(_addr, (uint8_t)9);

    if (_wire->available() != 9) {
        LOG_DEBUG("[TF-02pro] Insufficient data available: %d bytes", _wire->available());
        return 0.0;
    }

    // Read all 9 bytes
    for (int i = 0; i < 9; i++) {
        buffer[i] = _wire->read();
    }

    // Verify frame headers (0x59 0x59)
    if (buffer[0] != 0x59 || buffer[1] != 0x59) {
        LOG_DEBUG("[TF-02pro] Invalid frame headers: 0x%02X 0x%02X", buffer[0], buffer[1]);
        return 0.0;
    }

    // Verify checksum (sum of first 8 bytes, lower 8 bits)
    uint8_t checksum = 0;
    for (int i = 0; i < 8; i++) {
        checksum += buffer[i];
    }
    if (checksum != buffer[8]) {
        LOG_DEBUG("[TF-02pro] Checksum error. Expected: 0x%02X, Got: 0x%02X", checksum, buffer[8]);
        return 0.0;
    }

    // Extract distance from bytes 2-3 (in cm)
    uint16_t distance_cm = buffer[2] | (buffer[3] << 8);

    // Convert cm to mm for Meshtastic compatibility (RCWL9620 returned mm)
    float distance_mm = (float)distance_cm * 10.0;

    LOG_DEBUG("[TF-02pro] Distance: %u cm (%.1f mm)", distance_cm, distance_mm);

    // Apply TF-02pro max range limit (40m)
    if (distance_mm > 40000.0) {
        return 40000.0;
    }

    return distance_mm;
}

#endif
