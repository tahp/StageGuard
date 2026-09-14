#include <cassert>
#include <iostream>
#include "StageGuard.h"

int main() {
    StageGuard guard;

    guard.setStageThresholds(15000, 20000, 30000, 45000);
    guard.setGracePeriod(5000);

    // Start activation.
    guard.update(true, 0);
    assert(guard.getStage() == 1);

    // Stage boundaries.
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

    // Reset for grace-period testing.
    guard.reset();
    assert(guard.getStage() == 0);

    guard.update(true, 100000);
    guard.update(true, 116000);
    assert(guard.getStage() == 2);

    // Signal disappears.
    guard.update(false, 117000);

    // Still inside 5-second grace period.
    guard.update(false, 121999);
    assert(guard.getStage() == 2);

    // Grace period expires.
    guard.update(false, 122000);
    assert(guard.getStage() == 0);
    assert(!guard.isActive());

    std::cout << "All StageGuard tests passed.\n";

    return 0;
}
