# TRACS Assembler

## Context

The TRACS assembler is a piece of program that will convert an assembly program to a machine code. This program will read the texts on a file line per line and decode the instruction and operands. It will then check the instruction and generate the "instruction code". Together with the operand, the assembler will generate the 2-byte or 16-bit machine code. The machine code can then be read by the microprocessor and executed. The assembler also provides program error detection such as syntax, illegal or missing operands, out of range memory address etc. It also process part of the assembly code that is not instruction related also known as "pre-processor directives".

## TRACS Assembly Language

TRACS assembly language is similar to other assembly language from other architecture.

- TRACS instructions are basically with or without
  operands.
- WB 0x08 (instruction with a specified operand)
- ADD (instruction without an operand, implied operation)
- Operands can be an 8-bit data or an 11-bit memory address depending on the instruction

### Example:
`WM 0x402`
- `WM` = `00001`
- `0x402` = `10000000000`

Therefore, its machine code equivalent is `0000 1100 0000 0010` or `0x0C02`. The instruction passed is `0x0C` and the operand is `0x02`.

## Run Locally

1. Clone the project

```bash
  git clone https://github.com/blurridge/tracs-assembler-c
```
2. Go to the project directory
```bash
  cd tracs-assembler-py
```
3. Place `.asm` files in the `/data` folder.
4. Compile the project

```bash
  gcc main.c assembler.c -o main
```
5. Run the project

```bash
  ./main
```

## Stay in touch
If you have any questions, suggestions, or need further assistance, feel free to reach out to me. I'm always happy to help!

- Email: [zachriane01@gmail.com](mailto:zachriane01@gmail.com)
- GitHub: [@blurridge](https://github.com/blurridge)
- Twitter: [@zachahalol](https://twitter.com/zachahalol)
- Instagram: [@zachahalol](https://www.instagram.com/zachahalol)
- LinkedIn: [Zach Riane Machacon](https://www.linkedin.com/in/zachriane)