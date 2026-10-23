# Bionic Lab

**Building a foundation in embedded systems, robotics and bionics through hands-on engineering projects.**

This repository documents my journey towards developing bionic systems, combining embedded programming, electronics, signal processing and, eventually, machine learning.

The long-term goal is to develop an EMG-controlled robotic finger capable of responding to muscle activity.

Each stage focuses on building, testing and understanding a working prototype before introducing additional complexity.

## Week 01 — Non-blocking Arduino Control

**Status:** Working prototype  
**Hardware:** Arduino UNO R3, potentiometer, micro servo  
**Language:** C++ / Arduino

### Objective

Develop a non-blocking firmware system that reads an analog input, controls a servomotor with smooth movements and blinks an LED independently.

The potentiometer acts as a simulated muscle signal until the EMG sensor becomes available.

### Hardware

| Component | Purpose | Arduino connection |
|---|---|---|
| Arduino UNO R3 | Main microcontroller | — |
| Potentiometer | Analog control input | A0 |
| Micro servo (SG90 or equivalent) | Mechanical actuator | D9 (signal) |
| Built-in LED | Independent timing task | LED_BUILTIN |
| Breadboard | Circuit prototyping | Power and signal distribution |

The prototype uses a common ground between the components.

For reliable operation, the servo should be powered from a suitable 5 V supply rather than relying on the Arduino's 5 V rail under load.

### Firmware Architecture

The program performs three independent tasks without using `delay()`.

| Task | Period | Frequency |
|---|---:|---:|
| Read potentiometer | 10 ms | 100 Hz |
| Update servo position | 20 ms | 50 Hz |
| Toggle LED | 500 ms | 2 state changes/s |

Each task uses `millis()` and its own timing variables.

This allows the microcontroller to continue processing other tasks instead of blocking execution while waiting.

### Signal Filtering

Small variations in the potentiometer reading can cause unnecessary servo movements.

To reduce this behavior, the firmware applies a deadband of 5 ADC units.

A new reading is accepted only when its absolute difference from the last accepted value reaches the threshold.

This reduces sensitivity to small fluctuations without requiring advanced digital filtering.

### Smooth Servo Control

The accepted potentiometer reading is mapped to a servo target angle between 45° and 120°.

Instead of commanding the servo to jump directly to the new target, the program changes the commanded angle by one degree every 20 milliseconds.

The result is a gradual transition towards the target position.

This technique limits the rate of change of the position command; it does not measure the servo's actual physical position.

### Development Environment

- Linux: Omarchy (Arch-based)
- Editor: Neovim with LazyVim
- Language support: Arduino Language Server and clangd
- Build and upload: Arduino CLI
- Version control: Git and GitHub

### Build and Upload

From the repository root:

```bash
arduino-cli compile --fqbn arduino:avr:uno semana01-sin-bloqueos
```

Upload the firmware:

```bash
arduino-cli upload \
  -p /dev/ttyACM0 \
  --fqbn arduino:avr:uno \
  semana01-sin-bloqueos
```

The serial port may vary depending on the operating system and USB connection.

### Problems Encountered and Lessons Learned

**1. Servo not responding**

The servo signal cable was initially connected to D5 while the firmware was configured to use D9.

Correcting the wiring resolved the problem.

**2. Unstable analog readings**

Loose breadboard connections caused large fluctuations in potentiometer readings.

Securing the physical connections improved signal stability. This demonstrated the importance of checking hardware before attempting to solve every problem through software filtering.

**3. Timing and variable lifetime**

Early implementations incorrectly reset timing variables inside `loop()` or updated timers outside their scheduled intervals.

Moving persistent state outside the repeated function scope and updating each timer at the appropriate interval resolved these issues.

**4. Servo movement**

Directly mapping potentiometer readings to servo commands resulted in immediate position requests.

Introducing an independent movement timer made the position commands more gradual.

### Current Limitations

- The input is a potentiometer, not a real EMG signal.
- The servo operates without external position feedback.
- Sampling timing has not yet been measured with dedicated instrumentation.
- The prototype does not yet include a tendon-driven mechanical finger.

### Next Steps

- Implement hysteresis and a finite-state machine.
- Stream sensor data to Python for visualization and CSV recording.
- Design and 3D-print the finger mechanism.
- Integrate an EMG sensor when available.
- Document the complete bionic finger prototype.

---

*This project is part of a longer-term learning roadmap in embedded systems, physical AI and bionics.*
