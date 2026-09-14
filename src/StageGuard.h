#pragma once

#include <cstdint>
#include <functional>

class StageGuard {
public:
    using StageCallback = std::function<void(uint8_t)>;

    StageGuard();

    void setStageThresholds(
        uint32_t stage2Ms,
        uint32_t stage3Ms,
        uint32_t stage4Ms,
        uint32_t stage5Ms
    );

    void setGracePeriod(uint32_t graceMs);

    void setStageCallback(StageCallback callback);

    void update(bool active, uint32_t nowMs);

    uint8_t getStage() const;
    bool isActive() const;
    uint32_t getActiveDuration(uint32_t nowMs) const;

    void reset();

private:
    uint32_t stage2Ms_;
    uint32_t stage3Ms_;
    uint32_t stage4Ms_;
    uint32_t stage5Ms_;
    uint32_t graceMs_;

    uint32_t activeStartMs_;
    uint32_t inactiveStartMs_;

    uint8_t stage_;

    bool tracking_;
    bool signalActive_;
    bool graceRunning_;

    StageCallback callback_;

    void updateStage(uint32_t nowMs);
    void changeStage(uint8_t newStage);
};
