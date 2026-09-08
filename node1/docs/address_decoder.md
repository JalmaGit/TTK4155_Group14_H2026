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

Bit 11 HIGH (Bit 10 x) -> `SRAM`
Bit 11 LOW && Bit 10 HIGH -> `SRAM`
Bit 11 LOW && Bit 10 LOW -> `ADC`

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
ADC = !bit[10] * !bit[11]
SRAM = !ADC
```

Which is simply just an inverted NAND.

```
ADC = NOT (bit[10] NAND bit[11])
SRAM = NOT NOT (bit[10] NAND bit[11])
```

Simplified to only using `NAND` gates.

```
ADC =  (bit[10] NAND bit[11]) NAND (bit[10] NAND bit[11])
SRAM = bit[10] NAND bit[11]
```

Note: a `not` is simply a `nand` with both inputs wired together,
and a double `not` cancels.
