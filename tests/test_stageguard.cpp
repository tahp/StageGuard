#include <cassert>
#include <iostream>
#include <vector>
#include "StageGuard.h"

int main() {
    StageGuard guard;
    std::vector<uint8_t> stageChanges;

    guard.setStageCallback([&stageChanges](uint8_t stage) {
        stageChanges.push_back(stage);
    });

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
    assert(stageChanges.size() == 5);
    assert(stageChanges[0] == 1);
    assert(stageChanges[1] == 2);
    assert(stageChanges[2] == 3);
    assert(stageChanges[3] == 4);
    assert(stageChanges[4] == 5);
    guard.reset();
    assert(stageChanges.size() == 6);
    assert(stageChanges[5] == 0);

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
