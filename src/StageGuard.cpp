#include "StageGuard.h"

StageGuard::StageGuard()
    : stage2Ms_(15000),
      stage3Ms_(20000),
      stage4Ms_(30000),
      stage5Ms_(45000),
      graceMs_(5000),
      activeStartMs_(0),
      inactiveStartMs_(0),
      stage_(0),
      tracking_(false),
      signalActive_(false),
      graceRunning_(false) {
}

void StageGuard::setStageThresholds(
    uint32_t stage2Ms,
    uint32_t stage3Ms,
    uint32_t stage4Ms,
    uint32_t stage5Ms
) {
    stage2Ms_ = stage2Ms;
    stage3Ms_ = stage3Ms;
    stage4Ms_ = stage4Ms;
    stage5Ms_ = stage5Ms;
}

void StageGuard::setGracePeriod(uint32_t graceMs) {
    graceMs_ = graceMs;
}

void StageGuard::setStageCallback(StageCallback callback) {
    callback_ = callback;
}

void StageGuard::update(bool active, uint32_t nowMs) {
    signalActive_ = active;

    if (active) {
        if (!tracking_) {
            tracking_ = true;
            activeStartMs_ = nowMs;
            changeStage(1);
        }

        graceRunning_ = false;
        updateStage(nowMs);
        return;
    }

    if (!tracking_) {
        return;
    }

    if (!graceRunning_) {
        graceRunning_ = true;
        inactiveStartMs_ = nowMs;
        return;
    }

    if ((uint32_t)(nowMs - inactiveStartMs_) >= graceMs_) {
        reset();
    }
}

uint8_t StageGuard::getStage() const {
    return stage_;
}

bool StageGuard::isActive() const {
    return tracking_;
}

uint32_t StageGuard::getActiveDuration(uint32_t nowMs) const {
    if (!tracking_) {
        return 0;
    }

    return (uint32_t)(nowMs - activeStartMs_);
}

void StageGuard::reset() {
    tracking_ = false;
    signalActive_ = false;
    graceRunning_ = false;
    activeStartMs_ = 0;
    inactiveStartMs_ = 0;

    changeStage(0);
}

void StageGuard::updateStage(uint32_t nowMs) {
    if (!tracking_) {
        return;
    }

    uint32_t elapsed = (uint32_t)(nowMs - activeStartMs_);

    uint8_t newStage = 1;

    if (elapsed >= stage5Ms_) {
        newStage = 5;
    } else if (elapsed >= stage4Ms_) {
        newStage = 4;
    } else if (elapsed >= stage3Ms_) {
        newStage = 3;
    } else if (elapsed >= stage2Ms_) {
        newStage = 2;
    }

    changeStage(newStage);
}

void StageGuard::changeStage(uint8_t newStage) {
    if (newStage == stage_) {
        return;
    }

    stage_ = newStage;

    if (callback_) {
        callback_(stage_);
    }
}
