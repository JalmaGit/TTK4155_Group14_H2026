# Address Decoder

If using the recommended address partitioning:

- `ADC` from `0x1000 (0b0001 0000 0000 0000)` to `0x17FF (0b0001 0111 1111 1111)`.
- `SRAM` from `0x1800 (0b0001 1000 0000 0000)` to `0x1FFF (0b0001 1111 1111 1111)`.

Which allows for distinguishing the `ADC` from the `SRAM` by looking at bit 11
(where 0 is LSB).

Bit 11 LOW -> `ADC`
Bit 11 HIGH -> `SRAM`

If using the "challenge" address partitioning we get:

- `ADC` from `0x1000 (0b0001 0000 0000 0000)` to `0x13FF (0b0001 0011 1111 1111)`.
- `SRAM` from `0x1400 (0b0001 0100 0000 0000)` to `0x1FFF (0b0001 1111 1111 1111)`.

- `bit[11]` HIGH (`bit[10]` x) -> `SRAM`
- `bit[11]` LOW && `bit[10]` HIGH -> `SRAM`
- `bit[11]` LOW && `bit[10]` LOW -> `ADC`

| `Bit[10]` | `Bit[11]` | Out |
| --------------- | --------------- | --------------- |
| `0` | `0` | `ADC` |
| `0` | `1` | `SRAM` |
| `1` | `0` | `SRAM` |
| `1` | `1` | `SRAM` |

So, `ADC` if `bit[10]` and `bit[11]` are LOW.
`SRAM` otherwise.

With boolean logic:

```text
SRAM = bit[10] OR bit[11]
ADC = NOT SRAM

SRAM = NOT bit[10] NAND NOT bit[11]
ADC = NOT (NOT bit[10] NAND NOT bit[11])
```

Fully expanded using only NAND gates:

```text
A = bit[10] NAND bit[10]
B = bit[11] NAND bit[11]
SRAM = A NAND B
ADC = SRAM NAND SRAM
```

Requires `4` NAND gates.
NOTE: If the chip select enables are active LOW,
you can simply swap the `SRAM` and `ADC` outputs.
