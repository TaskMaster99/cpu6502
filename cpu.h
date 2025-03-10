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
    BIT_ABSOLUTE    = 0x2C
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