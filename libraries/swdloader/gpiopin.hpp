#ifndef GPIOPIN_HPP
#define GPIOPIN_HPP

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define HIGH 1
#define LOW  0

enum GPIOMode {
    GPIOModeInput,
    GPIOModeOutput
};

enum GPIOPull {
    GPIOPullNone,
    GPIOPullUp,
    GPIOPullDown
};

class GPIOPin {
public:
    GPIOPin() : m_pin(0) {}
    GPIOPin(uint pin, GPIOMode mode) {
        AssignPin(pin);
        SetMode(mode);
    }

    void AssignPin(uint pin) {
        m_pin = pin;
        gpio_init(m_pin);
    }

    void SetMode(GPIOMode mode, bool initialHigh = false) {
        if (mode == GPIOModeOutput) {
            gpio_set_dir(m_pin, GPIO_OUT);
            gpio_put(m_pin, initialHigh ? 1 : 0);
        } else {
            gpio_set_dir(m_pin, GPIO_IN);
        }
    }

    void SetPullMode(GPIOPull pull) {
        if (pull == GPIOPullUp) gpio_pull_up(m_pin);
        else if (pull == GPIOPullDown) gpio_pull_down(m_pin);
        else gpio_disable_pulls(m_pin);
    }

    void Write(bool value) {
        gpio_put(m_pin, value);
    }

    bool Read() {
        return gpio_get(m_pin);
    }

    void SetInput() {
        gpio_set_dir(m_pin, GPIO_IN);
    }

    void SetOutput() {
        gpio_set_dir(m_pin, GPIO_OUT);
    }

    void High() { gpio_put(m_pin, 1); }
    void Low()  { gpio_put(m_pin, 0); }

private:
    uint m_pin;
};

#endif // GPIOPIN_HPP
