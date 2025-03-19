# SimPit

This project is a **USB HID-based embedded system** developed for motor control in simulation cockpits. The system uses the HID protocol for communication between the microcontroller and the computer, sending motor speed, direction, and temperature data while receiving motor position and PID settings from the computer.

## HID Report Structure

- **From Computer to Microcontroller (OUT Report)**

  | Data            | Size   |
  | --------------  | ------ |
  | Motor Position  | 16-bit |
  | PID P Gain      | 16-bit |
  | PID I Gain      | 16-bit |
  | PID D Gain      | 16-bit |

- **From Microcontroller to Computer (IN Report)**

  | Data            | Size   |
  | --------------  | ------ |
  | Motor Speed     | 8-bit  |
  | Motor Direction | 1-bit  |
  | Motor Temperature | 16-bit |

## License

Distributed under the [MIT License](LICENSE).

