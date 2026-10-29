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

## Week 02 — Hysteresis, State Machines and Serial Data Acquisition

**Status:** Completed and tested  
**Hardware:** Arduino UNO R3, potentiometer, SG90 micro servo  
**Languages:** C++ / Arduino, Python  
**Tools:** Arduino CLI, pyserial, Matplotlib, Git

### Objectives

Extend the Week 01 prototype by introducing state-based control, hysteresis and a Python-based data acquisition system.

The main objectives were to:

- Implement a finite-state machine for opening and closing the servo.
- Prevent unwanted state changes using hysteresis.
- Receive and validate Arduino measurements through a serial connection.
- Record potentiometer readings in CSV format.
- Visualize the signal using static and real-time plots.

### 1. Finite-State Machine and Hysteresis

The controller implements two states:

| State | Target angle | Transition |
|---|---:|---|
| `ABIERTO` | 45° | Switch to `CERRADO` when ADC > 600 |
| `CERRADO` | 120° | Switch to `ABIERTO` when ADC < 400 |

Between the two thresholds, the previous state is preserved.

This hysteresis prevents rapid switching caused by small signal fluctuations near a single threshold.

The servo position is updated progressively using a non-blocking timing mechanism based on `millis()`.

### 2. Serial Communication with Python

The Arduino transmits potentiometer measurements to the computer using serial communication at 9600 baud.

Python receives the measurements using `pyserial`.

The acquisition program:

- Receives serial data.
- Reconstructs complete measurements from incoming bytes.
- Converts valid readings into integers.
- Handles invalid or corrupted data without terminating the acquisition.
- Associates readings with elapsed time.

### 3. Data Logging

Measurements are stored in CSV format with two columns:

| tiempo_s | valor_adc |
|---:|---:|
| 0.050 | 403 |
| 0.100 | 405 |
| 0.150 | 412 |

The timestamps are generated by Python using `time.monotonic()`.

They represent host-side acquisition timing rather than exact Arduino sampling timestamps.

### 4. Data Visualization

Matplotlib is used to represent potentiometer readings.

Two visualization approaches were implemented:

- **Static plotting:** loading previously recorded measurements from a CSV file.
- **Real-time plotting:** updating the signal directly from the Arduino serial stream.

The plots allow us to observe signal changes and compare them with the hysteresis thresholds of 400 and 600 ADC units.

### 5. Hardware Troubleshooting

During testing, the servo occasionally stalled, produced repeated clicking sounds or moved irregularly.

To diagnose the issue:

1. The control algorithm was simplified.
2. The servo was tested independently using fixed position commands.
3. The wiring was inspected and reorganized.
4. The complete system was tested again.

After reorganizing the connections, the servo operated smoothly and the potentiometer readings became substantially more stable.

**Key lesson:** Hardware connections and power distribution should be investigated before introducing software changes to compensate for apparently unstable behavior.

### 6. Results

- Working two-state controller with hysteresis.
- Smooth servo movement between 45° and 120°.
- Serial acquisition using Python.
- CSV measurement logging.
- Static and real-time signal visualization.
- Successful hardware troubleshooting.

### 7. Next Steps

Continue developing the prototype towards a mechanically actuated bionic finger, with improved hardware integration, structured control software and future muscle-signal acquisition.




docs:

Images 
<img width="2048" height="1536" alt="semana01-montaje" src="https://github.com/user-attachments/assets/c108be19-2a76-4bc2-a028-b3f5abb40429" />
Videos
https://github.com/user-attachments/assets/dac12f72-21cc-4cf6-97a8-da3ba1aab7a9



*This project is part of a longer-term learning roadmap in embedded systems, physical AI and bionics.*
