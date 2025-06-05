# Web-Based MCU Firmware Upgrades with WIZnet’s ioNIC Chip

This repository contains a **proof-of-concept** project that demonstrates how to use a **WIZnet W55RP20 (ioNIC)** chip as a webserver to handle **firmware upgrades** for a target MCU (like the Raspberry Pi Pico).

## 📦 Overview

- **Webserver**: Serves a simple HTML page to upload new firmware images.
- **SWD Programming**: Uses a ported version of the **SWDLoader** tool (from the Circle project) to program the target MCU over SWD.
- **Target Firmware**: Example tested with a simple blink firmware.

## 🔧 Key Details

- **Buffer**: Statically assigned `30*1024` bytes (enough for small tests).
- **SWDLoader Integration**: Interface file bridges C++ (SWDLoader) and C (webserver) code.
- **Firmware Preprocessing**: Custom Python script for firmware conversion (since Circle’s tool wasn’t available).
- **Target MCU Configuration**:  
  Add this line in the **`CMakeLists.txt`** of your target firmware project:  
  
  pico_set_binary_type(PROJECT no_flash)
  
- **Flash Reset**: Use a special **UF2 file** (link) to erase flash before flashing to RAM.

## 🚀 Usage

1. **Build** the firmware and webserver code for your W55RP20-based board.
2. **Connect** the target MCU via SWD.
3. **Power up** and connect the board to Ethernet.
4. Open the **web interface** in a browser and upload the firmware binary.
5. The board writes the firmware to the target MCU’s RAM and runs it.

## 🎥 Demo

Check out the project in action here: [\[YouTube video link\]](https://youtu.be/L_zheGfFfso)

## ⚠️ Note

This project is **not production-ready**. In real-world applications, you’d want to integrate cloud-based storage, MQTT brokers, or secure authentication to make firmware upgrades reliable and secure.

---





