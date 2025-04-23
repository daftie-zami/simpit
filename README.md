# SimPit

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

