# StageGuard

StageGuard is a lightweight, hardware-independent C++ library for escalating a response based on how long a condition remains active.

Instead of responding to an event with a simple ON/OFF action, StageGuard progresses through up to five stages over time.

## Example

A motion-detection system could use:

| Active Time | Stage | Example Response |
|---|---:|---|
| 0–15 sec | 1 | LED on |
| 15–20 sec | 2 | LED blink + warning chirp |
| 20–30 sec | 3 | Stronger warning |
| 30–45 sec | 4 | Pre-alarm |
| 45+ sec | 5 | Full alarm |

StageGuard itself does not control LEDs, buzzers, sensors, Wi-Fi, or GPIO.

It only manages the timing and escalation state.

## Why?

The input can come from anything that produces an active/inactive condition:

- PIR motion sensor
- mmWave presence sensor
- vibration sensor
- door or window switch
- proximity detector
- temperature threshold
- software/network condition

The response to each stage is defined by your application.

## Basic Usage

```cpp
#include <StageGuard.h>

StageGuard guard;

void onStageChange(uint8_t stage) {
    // Respond to the new stage here.
}

void setup() {
    guard.setStageThresholds(
        15000,
        20000,
        30000,
        45000
    );

    guard.setGracePeriod(5000);
    guard.setStageCallback(onStageChange);
}

void loop() {
    bool triggered = /* your condition */;

    guard.update(triggered, millis());
}
```

## Dropout Grace Period

StageGuard can tolerate brief interruptions without restarting escalation.

For example, with a 5-second grace period, an input that disappears for 2 seconds and returns will continue from its original activation time.

If the input remains inactive beyond the grace period, StageGuard resets to Stage 0.

## Design

The StageGuard core contains no Arduino or ESP32 dependencies.

The application supplies:

```cpp
guard.update(active, currentTimeMs);
```

StageGuard handles:

- activation timing
- stage transitions
- dropout grace periods
- reset behavior
- stage-change callbacks

This separation makes the same escalation engine reusable across different hardware and applications.

## License

MIT
