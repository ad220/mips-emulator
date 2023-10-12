# MIPS Emulator

Small MIPS Emulator as a final project for Grenoble INP - Esisar. Therefore, some parts of the code / comments can be in French.

## How to use ?
```bash
make all

# interactive mode
./emulateur

# converts file MIPS commands to hexa and processes the instructions
./emulateur <mips_file>

# same but processes the instructions step by step
./emulateur <mips_file> --pas
```
MIPS files are just text files with MIPS instructions in it (see [tests](tests/) folder for examples)

The emulator can be tested with the [xMat test](tests/xMat.mips) which computes the product of 2 3x3 matrix.

## How does it work ?

The emulator uses 32 conventional registers and three other dedicated ones : PC, HI and LO respectively for the program counter and the multiplication result. They are all 32-bits register and structured as an 1d-array of length 35.

One constraint for the project was a 4GB byte-adressable memory used for both program and data. To avoid allocating this much space for the emulator, it is strctured as a linked list of 256-bytes pages. They are allocated only when required by an instruction. After the MIPS commands are converted to hexadecimal, it is put at the beginning of the memory. Thus, it is up to the assembly developer to make sure the instructions don't get overwritten by data during execution.