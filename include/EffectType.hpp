//
// Created by Aristide Sessa on 28/08/25.
//

#ifndef BREAKOUT_EFFECTTYPE_HPP
#define BREAKOUT_EFFECTTYPE_HPP

enum class EffectType {
    ExpandPaddle,
    ShrinkPaddle,
    MultiBall,
    SlowBall,
    FastBall,
    StickyPaddle,
    Count
};

static const std::string typeNames[] = {
        "Expand Paddle",
        "Shrink Paddle",
        "Multi Ball",
        "Slow Ball",
        "Fast Ball",
        "Sticky Paddle",
        "Unknown"
};

#endif //BREAKOUT_EFFECTTYPE_HPP
