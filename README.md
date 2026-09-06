# Arduino-Scripts

Sketches organized by hardware/component.

## LEDs/
| Sketch | Purpose |
|---|---|
| `Blink-1.1` | Basic blink of the built-in LED (`LED_BUILTIN`), with Serial print each cycle. |
| `Blink-1.2` | Blink of a single LED on pin 13, refactored into a reusable `blink(duration)` function. |
| `Scrolling` | Scrolls a lit LED back and forth across 5 LEDs (pins 2-6), Knight Rider style. |

## IRremote/
| Sketch | Purpose |
|---|---|
| `IR-1.1` | Minimal IR receiver using the `IRremote` library; prints decoded hex values. |
| `IR-1.2` | Same as IR-1.1 but adds an activity LED on pin 13 and a startup Serial message. |
| `IR-1.3-Advanced` | Uses the newer `IRremote.hpp` API; logs both protocol name and decoded value. Notes that NEC decoding is unreliable with cheap remotes. |

## WiFi/
| Sketch | Purpose |
|---|---|
| `WiFi-HTTP-Time` | Connects an ESP32 to WiFi and fetches the current time for America/Chicago from a REST API. **Contains a hardcoded WiFi password — sanitize before sharing/publishing.** |

## Sensors/
| Sketch | Purpose |
|---|---|
| `Temp-Humidity-DHT11` | Reads a DHT11 sensor (data pin 2) and prints temperature (°C) and humidity (%) to the Serial monitor every 2 seconds. Requires the `DHT sensor library` (Adafruit). |
| `Ultrasonic-HC-SR04` | Reads an HC-SR04 ultrasonic sensor (trig pin 9, echo pin 10) and prints the measured distance in cm to the Serial monitor. No extra library required. |

## Motors/
| Sketch | Purpose |
|---|---|
| `Stepper-28BYJ48` | Drives a 28BYJ-48 stepper motor via a ULN2003 driver board, sweeping clockwise then counter-clockwise using the built-in `Stepper` library. |
| `Servo-Sweep` | Sweeps a standard hobby servo (signal on pin 9) back and forth between 0° and 180° using the `Servo` library. |

## Displays/
| Sketch | Purpose |
|---|---|
| `Base-TFT-Code` | Baseline test sketch for an MCUFRIEND TFT shield on a Mega 2560 — draws shapes/text and confirms the display driver ID. |
| `Weather-Display-Basic` | TFT weather display mockup (temperature/humidity) with a custom dark color palette; currently uses simulated data. **Contains a hardcoded WiFi password — sanitize before sharing/publishing.** |
| `ESP32-Stock-Ticker` | ESP32 sketch that connects to WiFi and pulls stock/index chart data from the RapidAPI "live-stock-market" endpoint. **Contains a hardcoded WiFi password — sanitize before sharing/publishing.** |
