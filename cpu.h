#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <string.h>

#define ADRESSING_SIZE 65536
#define PAGE_SIZE 0x00FF

#define READ 1
#define WRITE 0

#define PTR_ACCUMULATOR &cpu->REGISTER_ACCUMULATOR
#define PTR_X &cpu->REGISTER_INDEX_X
#define PTR_Y &cpu->REGISTER_INDEX_Y
#define PTR_STATUS &cpu->REGISTER_PROCESSOR_T.REGISTER_STATUS

typedef unsigned char  BYTE;  // 1 byte => 8  bits
typedef unsigned short WORD;  // 2 byte => 16 bits
typedef unsigned int   DWORD; // 4 byte => 32 bits



enum                   INSTRUCTION_SET
{
    LDA_IMMEDIATE   = 0xA9,
    LDA_ZERO_PAGE   = 0xA5,
    LDA_ZERO_PAGE_X = 0xB5,
    LDA_ABSOLUTE    = 0xAD,
    LDA_ABSOLUTE_X  = 0xBD,
    LDA_ABSOLUTE_Y  = 0xB9,
    LDA_INDIRECT_X  = 0xA1,
    LDA_INDIRECT_Y  = 0xB1,

    LDX_IMMEDIATE   = 0xA2,
    LDX_ZERO_PAGE   = 0xA6,
    LDX_ZERO_PAGE_Y = 0xB6,
    LDX_ABSOLUTE    = 0xAE,
    LDX_ABSOLUTE_Y  = 0xBE,

    LDY_IMMEDIATE   = 0xA0,
    LDY_ZERO_PAGE   = 0xA4,
    LDY_ZERO_PAGE_X = 0xB4,
    LDY_ABSOLUTE    = 0xAC,
    LDY_ABSOLUTE_X  = 0xBC,

    STA_ZERO_PAGE   = 0x85,
    STA_ZERO_PAGE_X = 0x95,
    STA_ABSOLUTE    = 0x8D,
    STA_ABSOLUTE_X  = 0x9D,
    STA_ABSOLUTE_Y  = 0x99,
    STA_INDIRECT_X  = 0x81,
    STA_INDIRECT_Y  = 0x91,

    STX_ZERO_PAGE   = 0x86,
    STX_ZERO_PAGE_Y = 0x96,
    STX_ABSOLUTE    = 0x8E,

    STY_ZERO_PAGE   = 0x84,
    STY_ZERO_PAGE_X = 0x94,
    STY_ABSOLUTE    = 0x8C,

    TAX_IMPLIED     = 0xAA,
    TAY_IMPLIED     = 0xA8,
    TSX_IMPLIED     = 0xBA,
    TXA_IMPLIED     = 0x8A,
    TXS_IMPLIED     = 0x9A,
    TYA_IMPLIED     = 0x98,

    PHA_IMPLIED     = 0x48,
    PHP_IMPLIED     = 0x08,
    PLA_IMPLIED     = 0x68,
    PLP_IMPLIED     = 0x28,

    AND_IMMEDIATE   = 0x29,
    AND_ZERO_PAGE   = 0x25,
    AND_ZERO_PAGE_X = 0x35,
    AND_ABSOLUTE    = 0x2D,   
    AND_ABSOLUTE_X  = 0x3D,
    AND_ABSOLUTE_Y  = 0x39,
    AND_INDIRECT_X  = 0x21,
    AND_INDIRECT_Y  = 0x31,

    BIT_ZERO_PAGE   = 0x24,
    BIT_ABSOLUTE    = 0x2C,

    EOR_IMMEDIATE   = 0x49,
    EOR_ZERO_PAGE   = 0x45,
    EOR_ZERO_PAGE_X = 0x55,
    EOR_ABSOLUTE    = 0x4D,
    EOR_ABSOLUTE_X  = 0x5D,
    EOR_ABSOLUTE_Y  = 0x59,
    EOR_INDIRECT_X  = 0x41,
    EOR_INDIRECT_Y  = 0x51,

    ORA_IMMEDIATE   = 0x09,
    ORA_ZERO_PAGE   = 0x05,
    ORA_ZERO_PAGE_X = 0x15,
    ORA_ABSOLUTE    = 0x0D,
    ORA_ABSOLUTE_X  = 0x1D,
    ORA_ABSOLUTE_Y  = 0x19,
    ORA_INDIRECT_X  = 0x01,
    ORA_INDIRECT_Y  = 0x11,

    ADC_IMMEDIATE   = 0x69,
    ADC_ZERO_PAGE   = 0x65,
    ADC_ZERO_PAGE_X = 0x75,
    ADC_ABSOLUTE    = 0x6D,
    ADC_ABSOLUTE_X  = 0x7D,
    ADC_ABSOLUTE_Y  = 0x79,
    ADC_INDIRECT_X  = 0x61,
    ADC_INDIRECT_Y  = 0x71,

    SBC_IMMEDIATE   = 0xE9,
    SBC_ZERO_PAGE   = 0xE5,
    SBC_ZERO_PAGE_X = 0xF5,
    SBC_ABSOLUTE    = 0xED,
    SBC_ABSOLUTE_X  = 0xFD,
    SBC_ABSOLUTE_Y  = 0xF9,
    SBC_INDIRECT_X  = 0xE1,
    SBC_INDIRECT_Y  = 0xF1,

    CMP_IMMEDIATE   = 0xC9,
    CMP_ZERO_PAGE   = 0xC5,
    CMP_ZERO_PAGE_X = 0xD5,
    CMP_ABSOLUTE    = 0xCD,
    CMP_ABSOLUTE_X  = 0xDD,
    CMP_ABSOLUTE_Y  = 0xD9,
    CMP_INDIRECT_X  = 0xC1,
    CMP_INDIRECT_Y  = 0xD1,

    CPX_IMMEDIATE   = 0xE0,
    CPX_ZERO_PAGE   = 0xE4,
    CPX_ABSOLUTE    = 0xEC,

    CPY_IMMEDIATE   = 0xC0,
    CPY_ZERO_PAGE   = 0xC4,
    CPY_ABSOLUTE    = 0xCC,

    INC_ZERO_PAGE   = 0XE6,
    INC_ZERO_PAGE_X = 0XF6,
    INC_ABSOLUTE    = 0XEE,
    INC_ABSOLUTE_X  = 0XFE,

    INX_IMPLIED     = 0XE8,

    INY_IMPLIED     = 0XC8,

    DEC_ZERO_PAGE   = 0XC6,
    DEC_ZERO_PAGE_X = 0XD6,
    DEC_ABSOLUTE    = 0XCE,
    DEC_ABSOLUTE_X  = 0XDE,

    DEX_IMPLIED     = 0XCA,

    DEY_IMPLIED     = 0X88,

    ASL_ACCUMULATOR = 0X0A,
    ASL_ZERO_PAGE   = 0X06,
    ASL_ZERO_PAGE_X = 0X16,
    ASL_ABSOLUTE    = 0X0E,
    ASL_ABSOLUTE_X  = 0X1E,

    LSR_ACCUMULATOR = 0X4A,
    LSR_ZERO_PAGE   = 0X46,
    LSR_ZERO_PAGE_X = 0X56,
    LSR_ABSOLUTE    = 0X4E,
    LSR_ABSOLUTE_X  = 0X5E,

    ROL_ACCUMULATOR = 0X2A,
    ROL_ZERO_PAGE   = 0X26,
    ROL_ZERO_PAGE_X = 0X36,
    ROL_ABSOLUTE    = 0X2E,
    ROL_ABSOLUTE_X  = 0X3E,    

    ROR_ACCUMULATOR = 0X6A,
    ROR_ZERO_PAGE   = 0X66,
    ROR_ZERO_PAGE_X = 0X76,
    ROR_ABSOLUTE    = 0X6E,
    ROR_ABSOLUTE_X  = 0X7E,

    JMP_ABSOLUTE    = 0X4C,
    JMP_INDIRECT    = 0X6C,

    JSR_ABSOLUTE    = 0X20,

    RTS_IMPLIED     = 0X60,

    BCC_RELATIVE    = 0X90,
    BCS_RELATIVE    = 0XB0,
    BEQ_RELATIVE    = 0XF0,
    BMI_RELATIVE    = 0X30,
    BNE_RELATIVE    = 0XD0,
    BPL_RELATIVE    = 0X10,
    BVC_RELATIVE    = 0X50,
    BVS_RELATIVE    = 0X70,

    CLC_IMPLIED     = 0X18,
    CLD_IMPLIED     = 0XD8,
    CLI_IMPLIED     = 0X58,
    CLV_IMPLIED     = 0XB8,
    SEC_IMPLIED     = 0X38,
    SED_IMPLIED     = 0XF8,
    SEI_IMPLIED     = 0X78,

    BRK_IMPLIED     = 0X00,
    NOP_IMPLIED     = 0XEA,
    RTI_IMPLIED     = 0X40

};

typedef struct CPU6502
{
    BYTE         MEMORY[ADRESSING_SIZE];

    unsigned int TOTAL_CYCLE_COUNT;

    WORD         REGISTER_PROGRAM_COUNTER;
    WORD         REGISTER_STACK_POINTER;

    BYTE         REGISTER_ACCUMULATOR;
    BYTE         REGISTER_INDEX_X;
    BYTE         REGISTER_INDEX_Y;

    union REGISTER_PROCESSOR
    {
        struct STATUS_FLAGS
        {
            unsigned int CARRY:1;
            unsigned int ZERO:1;
            unsigned int INTERRUPT_DISABLE:1;
            unsigned int DECIMAL_MODE:1;
            unsigned int BREAK_COMMAND:1;
            unsigned int OVERFLOW:1;
            unsigned int NEGATIVE:1;
        }STATUS_FLAGS_T;
        
        BYTE   REGISTER_STATUS;
    }REGISTER_PROCESSOR_T;
    
}CPU6502_T;

void RESET_CPU(CPU6502_T* cpu);

void PUSH_TO_STACK(CPU6502_T* cpu, BYTE data);

void POP_FROM_STACK(CPU6502_T* cpu, BYTE* data);

void FETCHING_BYTE(CPU6502_T* cpu, BYTE* data);
void FETCHING_WORD(CPU6502_T* cpu, WORD* data);

void READ_FROM_BYTE_ADDRESS(CPU6502_T* cpu, BYTE* Address, BYTE* data);
void READ_FROM_WORD_ADDRESS(CPU6502_T* cpu, WORD* Address, BYTE* data);

void WRITE_TO_BYTE_ADDRESS(CPU6502_T* cpu, BYTE* Address, BYTE* data);
void WRITE_TO_WORD_ADDRESS(CPU6502_T* cpu, WORD* Address, BYTE* data);

void RUN_CPU(CPU6502_T* cpu);

#endif