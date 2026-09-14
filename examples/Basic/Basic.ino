#include <StageGuard.h>

StageGuard guard;

const int TRIGGER_PIN = 4;

void onStageChange(uint8_t stage) {
    Serial.print("Stage changed to: ");
    Serial.println(stage);

    // Add project-specific responses here.
    switch (stage) {
        case 0:
            // Idle / reset
            break;

        case 1:
            // Example: turn on LED
            break;

        case 2:
            // Example: warning chirp
            break;

        case 3:
            // Example: stronger warning
            break;

        case 4:
            // Example: pre-alarm
            break;

        case 5:
            // Example: full alarm
            break;
    }
}

void setup() {
    Serial.begin(115200);

    pinMode(TRIGGER_PIN, INPUT);

    guard.setStageThresholds(
        15000,  // Stage 2 at 15 seconds
        20000,  // Stage 3 at 20 seconds
        30000,  // Stage 4 at 30 seconds
        45000   // Stage 5 at 45 seconds
    );

    guard.setGracePeriod(5000);
    guard.setStageCallback(onStageChange);
}

void loop() {
    bool triggered = digitalRead(TRIGGER_PIN) == HIGH;

    guard.update(triggered, millis());
}
