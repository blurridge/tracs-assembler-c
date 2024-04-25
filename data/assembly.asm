ORG 0x010
start   WB 0x05 ; write 0x05 to MBR
        WM 0x400 ; write data on MBR to memory at address 0x400
        WB 0x03 ; write 0x03 to MBR
        WM 0x401 ; write data on MBR to memory at address 0x401
        RM 0x400 ; read address 0x400, store data to MBR
        WACC ; write MBR to ACC (via BUS)
        RM 0x401 ; read data at memory address 0x401, store to MBR
        ADD ; add ACC to MBR (through BUS)
        RACC ; read ACC, store to MBR
        WM 0x402 ; write data on MBR to memory address 0x402
        WB 0x00 ; write 0x00 to MBR
        RM 0x402 ; read address 0x402, store data to MBR
        WACC ; write MBR to ACC (via BUS)
        WB 0x05 ; write 0x05 to MBR
        BRGT here ; compare ACC with MBR, jump to here if greater than
        RACC ; read ACC, store to MBR
        WM 0x403 ; write data on MBR to memory address 0x403
here    SHL ; shift left ACC
        RACC ; read ACC, store to MBR
        WM 0x405 ; write data on MBR to memory at address 0x405
        EOP