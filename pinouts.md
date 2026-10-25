# FollowTune Pinouts and Firmware Guide

FollowTune by Ridith Shetty. Complete hardware pinouts and firmware code for both the robot receiver car and the Ripstik transmitter beacon.

---

## 1. Robot Receiver Car Pinout (Arduino Nano)

| Component | Component Pin | Arduino Nano Pin | Connection Notes |
| :--- | :--- | :--- | :--- |
| Left IR Receiver | OUT / Signal | A0 | Analog input for 38kHz tracking |
| Center IR Receiver | OUT / Signal | A1 | Analog input for 38kHz tracking |
| Right IR Receiver | OUT / Signal | A2 | Analog input for 38kHz tracking |
| LDR Sensor Module | Analog / Digital OUT | A4 | Detects direct laser beam hits |
| Ultrasonic Sensor | TRIG | D2 | Triggers distance pulse |
| Ultrasonic Sensor | ECHO | D4 | Reads echo pulse return time |
| SG90 Micro Servo | PWM Signal | D3 | Pans ultrasonic sensor left/right |
| L9110S Motor Driver | IN1 (Motor A Dir) | D6 | Digital direction control |
| L9110S Motor Driver | ENA / IN2 (Motor A PWM) | D5 | Left motor speed control |
| L9110S Motor Driver | IN3 (Motor B Dir) | D7 | Digital direction control |
| L9110S Motor Driver | ENB / IN4 (Motor B PWM) | D10 | Right motor speed control |
| PAM8403 Amplifier | Audio Input / Buzzer Pin | D11 | Digital PWM / Tone Output |
| Bluetooth Module | STATE Connection Pin | D12 | Digital Input (HIGH = Connected) |
| System Power | VCC / VIN | 5V / VIN | Powered via 12V Li-ion Battery pack |
| System Ground | GND | GND | Common Ground line |

---

## 2. Ripstik Transmitter Beacon Pinout (555 Timer Circuit)

| Component | Pin / Terminal | Connection Point | Function |
| :--- | :--- | :--- | :--- |
| 5V Ni-Mh Battery | Positive (+5V) | Circuit Power Rail | Power for NE555 and Laser Diode |
| 5V Ni-Mh Battery | Negative (GND) | Circuit Ground Rail | Common Ground line |
| NE555 Timer IC | Pin 1 (GND) | Ground Rail | IC Ground connection |
| NE555 Timer IC | Pin 4 & Pin 8 | +5V Rail | Power & Enable pins |
| NE555 Timer IC | Pin 2 & Pin 6 | RC Timing Network | Sets oscillation frequency |
| NE555 Timer IC | Pin 3 (OUTPUT) | 3x 220 Ohm Resistors | Outputs pulsed signal to IR LEDs |
| 940nm IR Emitters (x3) | Anode (+) | 220 Ohm Resistor output | Transmits tracking beam fan array |
| KY-008 Laser Diode | Signal / VCC | +5V Rail | Fires constant center-lock red laser beam |

---

