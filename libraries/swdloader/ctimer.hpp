#ifndef TIMER_HPP
#define TIMER_HPP

#include "pico/stdlib.h"
#include "pico/time.h"
#include "hardware/timer.h"

class CTimer {
public:
    // Optional singleton getter (for compatibility)
    static CTimer* Get() {
        static CTimer instance;
        return &instance;
    }

    // Delay methods
    void DelayNanos(uint32_t ns)  { busy_wait_us(ns / 1000); }
    void DelayMicros(uint32_t us) { busy_wait_us(us); }
    void nsDelay(uint32_t ns)     { busy_wait_us(ns / 1000); }  // <- Circle-style method
    void DelayMillis(uint32_t ms) { sleep_ms(ms); }
    void MsDelay(uint32_t ms)     { sleep_ms(ms); }

    // Clock tick counter (since boot, in microseconds)
    uint64_t GetClockTicks()      { return time_us_64(); }

private:
    CTimer() {}  // Private constructor for singleton pattern
};

#endif // TIMER_HPP
