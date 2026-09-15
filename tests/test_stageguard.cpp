#include <cassert>
#include <iostream>
#include "StageGuard.h"

int main() {
    StageGuard guard;

    // Valid thresholds should be accepted.
    assert(guard.setStageThresholds(
        15000,
        20000,
        30000,
        45000
    ));

    guard.setGracePeriod(5000);

    // Stage boundaries.
    guard.update(true, 0);
    assert(guard.getStage() == 1);

    guard.update(true, 14999);
    assert(guard.getStage() == 1);

    guard.update(true, 15000);
    assert(guard.getStage() == 2);

    guard.update(true, 20000);
    assert(guard.getStage() == 3);

    guard.update(true, 30000);
    assert(guard.getStage() == 4);

    guard.update(true, 45000);
    assert(guard.getStage() == 5);

    // Grace-period behavior.
    guard.reset();

    guard.update(true, 100000);
    guard.update(true, 116000);
    assert(guard.getStage() == 2);

    guard.update(false, 117000);

    guard.update(false, 121999);
    assert(guard.getStage() == 2);

    guard.update(false, 122000);
    assert(guard.getStage() == 0);
    assert(!guard.isActive());

    // Signal recovery during grace period should preserve the event.
    guard.reset();

    guard.update(true, 200000);
    guard.update(true, 216000);
    assert(guard.getStage() == 2);

    guard.update(false, 217000);
    guard.update(false, 219000);

    guard.update(true, 220000);
    assert(guard.getStage() == 3);
    assert(guard.isActive());

    // Invalid thresholds should be rejected.
    assert(!guard.setStageThresholds(
        30000,
        20000,
        40000,
        50000
    ));

    assert(!guard.setStageThresholds(
        15000,
        15000,
        30000,
        45000
    ));

    std::cout << "All StageGuard tests passed.\n";

    return 0;
}
