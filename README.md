# family-kb-arduino

This project uses the ATmega32U4, specifically an Arduino Micro wired to
a male DB15 port that the HVC-007 uses as a connector.

## Pin Map
| DB15        | Arduino     | Release Micro |
| :---------: | :---------: | :-----------: |
| 1           | GND         | GND           |
| 4 - Input   | 11 - PB7    | 8 - PB4       |
| 5 - Input   | 10 - PB6    | 14 - PB3      |
| 6 - Input   | 9 - PB5     | 16 - PB2      |
| 7 - Input   | 8 - PB4     | 15 - PB1      |
| 10 - Output | 4           | 4             |
| 11 - Output | 12 - PD6    | 6 - PD7       |
| 12 - Output | 2           | 2             |
| 13 - Input  | 7           | 7             |
| 15 - 5V     | VCC         | VCC           |

