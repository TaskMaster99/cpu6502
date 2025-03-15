#ifndef TEST_H
#define TEST_H
#include "cpu.h"
#include <assert.h>

#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"

void EQUAL(char* type,unsigned int a, unsigned int b)
{
    if(a == b)
        printf("%s%s TEST SUCCEED.\n%s",GRN, type, RESET);
    else
        printf("%s%s TEST FAILED.\n%s",RED, type, RESET);
}


void TEST_LDA_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xFFFC] = LDA_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("LDA Immediate\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x84);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);

    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_LDA_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x32] = 0xD6;

    cpu.MEMORY[0xFFFC] = LDA_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x32;

    RUN_CPU(&cpu);

    printf("LDA Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xD6);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_LDA_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x06;
    cpu.MEMORY[0x28] = 0x45;

    cpu.MEMORY[0xFFFC] = LDA_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x22;

    RUN_CPU(&cpu);

    printf("LDA Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x45);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_LDA_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x32FF] = 0xAC;

    cpu.MEMORY[0xFFFC] = LDA_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("LDA Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_LDA_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x32FB] = 0xC1;

    cpu.MEMORY[0xFFFC] = LDA_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xF0;

    RUN_CPU(&cpu);

    printf("LDA Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC1);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_LDA_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x0B;
    cpu.MEMORY[0x32FB] = 0xC1;

    cpu.MEMORY[0xFFFC] = LDA_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xF0;

    RUN_CPU(&cpu);

    printf("LDA Absolute Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC1);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_LDA_INDEXED_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0xFF;

    cpu.MEMORY[0xFFFC] = LDA_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);

    printf("LDA Indexed x\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xFF);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_LDA_INDEXED_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0xA0;

    cpu.MEMORY[0xFFFC] = LDA_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);

    printf("LDA Indexed y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xA0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}



void TEST_LDX_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xFFFC] = LDX_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("LDX Immediate\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0x84);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_LDX_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x32] = 0xD6;

    cpu.MEMORY[0xFFFC] = LDX_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x32;

    RUN_CPU(&cpu);

    printf("LDX Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0xD6);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_LDX_ZERO_PAGE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x06;
    cpu.MEMORY[0x28] = 0x45;

    cpu.MEMORY[0xFFFC] = LDX_ZERO_PAGE_Y;
    cpu.MEMORY[0xFFFD] = 0x22;

    RUN_CPU(&cpu);

    printf("LDX Zero Page Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0x45);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_LDX_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x32FF] = 0xAC;

    cpu.MEMORY[0xFFFC] = LDX_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("LDX Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_LDX_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x0B;
    cpu.MEMORY[0x32FB] = 0xC1;

    cpu.MEMORY[0xFFFC] = LDX_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xF0;

    RUN_CPU(&cpu);

    printf("LDX Absolute Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0xC1);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}




void TEST_LDY_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xFFFC] = LDY_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("LDY Immediate\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0x84);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_LDY_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x32] = 0xD6;

    cpu.MEMORY[0xFFFC] = LDY_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x32;

    RUN_CPU(&cpu);

    printf("LDY Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0xD6);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_LDY_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x06;
    cpu.MEMORY[0x28] = 0x45;

    cpu.MEMORY[0xFFFC] = LDY_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x22;

    RUN_CPU(&cpu);

    printf("LDY Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0x45);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_LDY_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x32FF] = 0xAC;

    cpu.MEMORY[0xFFFC] = LDY_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("LDY Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_LDY_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x32FB] = 0xC1;

    cpu.MEMORY[0xFFFC] = LDY_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xF0;

    RUN_CPU(&cpu);

    printf("LDY Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0xC1);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_STA_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;

    cpu.MEMORY[0xFFFC] = STA_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("STA Zero Page\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0x84], 0x08);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
}

void TEST_STA_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_X = 0x0B;

    cpu.MEMORY[0xFFFC] = STA_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("STA Zero Page X\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[(0x84 + 0x0B)], 0x08);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
}

void TEST_STA_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xAC;

    cpu.MEMORY[0xFFFC] = STA_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("STA Absolute\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0x32FF], 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
}

void TEST_STA_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xAC;
    cpu.REGISTER_INDEX_X = 0x0B;

    cpu.MEMORY[0xFFFC] = STA_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("STA Absolute X\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[(0x32FF + 0x0B)], 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
}

void TEST_STA_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xAC;
    cpu.REGISTER_INDEX_Y = 0x0B;

    cpu.MEMORY[0xFFFC] = STA_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("STA Absolute Y\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[(0x32FF + 0x0B)], 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
}

void TEST_STA_INDIRECT_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;

    cpu.MEMORY[0xFFFC] = STA_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);

    printf("STA Indexed X\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0xAC30], 0xFF);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
}

void TEST_STA_INDIRECT_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xA0;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;

    cpu.MEMORY[0xFFFC] = STA_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);

    printf("STA Indexed Y\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0xAC34], 0xA0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
}


void TEST_STX_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x08;

    cpu.MEMORY[0xFFFC] = STX_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("STX Zero Page\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0x84], 0x08);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
}

void TEST_STX_ZERO_PAGE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x08;
    cpu.REGISTER_INDEX_Y = 0x0B;

    cpu.MEMORY[0xFFFC] = STX_ZERO_PAGE_Y;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("STX Zero Page Y\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[(0x84 + 0x0B)], 0x08);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
}

void TEST_STX_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0xAC;

    cpu.MEMORY[0xFFFC] = STX_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("STX Absolute\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0x32FF], 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
}


void TEST_STY_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x08;

    cpu.MEMORY[0xFFFC] = STY_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("STY Zero Page\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0x84], 0x08);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
}

void TEST_STY_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x08;
    cpu.REGISTER_INDEX_X = 0x0B;

    cpu.MEMORY[0xFFFC] = STY_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x84;

    RUN_CPU(&cpu);

    printf("STY Zero Page X\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[(0x84 + 0x0B)], 0x08);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
}

void TEST_STY_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0xAC;

    cpu.MEMORY[0xFFFC] = STY_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("STY Absolute\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[0x32FF], 0xAC);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);   
}


void TEST_TAX()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x80;
    
    cpu.MEMORY[0xFFFC] = TAX_IMPLIED;

    RUN_CPU(&cpu);

    printf("TAX Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0x80);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2); 
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_TAY()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x80;
    
    cpu.MEMORY[0xFFFC] = TAY_IMPLIED;

    RUN_CPU(&cpu);

    printf("TAY Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0x80);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2); 
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_TSX()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_STACK_POINTER = 0x80;
    
    cpu.MEMORY[0xFFFC] = TSX_IMPLIED;

    RUN_CPU(&cpu);

    printf("TSX Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0x80);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2); 
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_TXA()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x80;
    
    cpu.MEMORY[0xFFFC] = TXA_IMPLIED;

    RUN_CPU(&cpu);

    printf("TXA Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x80);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2); 
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_TXS()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x80;
    
    cpu.MEMORY[0xFFFC] = TXS_IMPLIED;

    RUN_CPU(&cpu);

    printf("TXS Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_STACK_POINTER, 0x80);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2); 
}

void TEST_TYA()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x80;
    
    cpu.MEMORY[0xFFFC] = TYA_IMPLIED;

    RUN_CPU(&cpu);

    printf("TYA Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x80);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2); 
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_PHA()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x12;

    cpu.MEMORY[0xFFFC] = PHA_IMPLIED;

    RUN_CPU(&cpu);

    printf("PHA Implied\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[cpu.REGISTER_STACK_POINTER+1], 0x12);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);     
}

void TEST_PHP()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_PROCESSOR_T.REGISTER_STATUS = 0x12;

    cpu.MEMORY[0xFFFC] = PHP_IMPLIED;

    RUN_CPU(&cpu);

    printf("PHP Implied\n");

    printf("\t");
    EQUAL("MEMORY",cpu.MEMORY[cpu.REGISTER_STACK_POINTER+1], 0x12);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3); 
}

void TEST_PLA()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[cpu.REGISTER_STACK_POINTER] = 0x12;
    cpu.REGISTER_STACK_POINTER--;

    cpu.MEMORY[0xFFFC] = PLA_IMPLIED;

    RUN_CPU(&cpu);

    printf("PLA Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x12);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4); 
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_PLP()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[cpu.REGISTER_STACK_POINTER] = 0x12;
    cpu.REGISTER_STACK_POINTER--;

    cpu.MEMORY[0xFFFC] = PLP_IMPLIED;

    RUN_CPU(&cpu);

    printf("PLP Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_PROCESSOR_T.REGISTER_STATUS, 0x12);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4); 
}

void TEST_AND_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.MEMORY[0xFFFC] = AND_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);

    printf("AND Immediate\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x42] = 0xE2;

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.MEMORY[0xFFFC] = AND_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x42;

    RUN_CPU(&cpu);

    printf("AND Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x42 + 0x08] = 0xE2;
    cpu.REGISTER_INDEX_X = 0x08;

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.MEMORY[0xFFFC] = AND_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x42;

    RUN_CPU(&cpu);

    printf("AND Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.MEMORY[0xE3E2] = 0xE2;

    cpu.MEMORY[0xFFFC] = AND_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE3;
    cpu.MEMORY[0xFFFE] = 0xE2;

    RUN_CPU(&cpu);

    printf("AND Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.REGISTER_INDEX_X = 0x08;
    cpu.MEMORY[0xE30A] = 0xE2;

    cpu.MEMORY[0xFFFC] = AND_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE3;
    cpu.MEMORY[0xFFFE] = 0x02;

    RUN_CPU(&cpu);

    printf("AND Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.REGISTER_INDEX_Y = 0x08;
    cpu.MEMORY[0xE30A] = 0xE2;

    cpu.MEMORY[0xFFFC] = AND_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0xE3;
    cpu.MEMORY[0xFFFE] = 0x02;

    RUN_CPU(&cpu);

    printf("AND Absolute Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_INDIRECT_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0xE2;

    cpu.MEMORY[0xFFFC] = AND_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);

    printf("AND Indirect Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_AND_INDIRECT_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0xE2;

    cpu.MEMORY[0xFFFC] = AND_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);

    printf("AND Indirect X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_BIT_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x42] = 0xE2;

    cpu.REGISTER_ACCUMULATOR = 0xD0;
    cpu.MEMORY[0xFFFC] = BIT_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x42;

    RUN_CPU(&cpu);

    printf("BIT Zero Page\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_BIT_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xD0;

    cpu.MEMORY[0x32FF] = 0xE2;

    cpu.MEMORY[0xFFFC] = BIT_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x32;
    cpu.MEMORY[0xFFFE] = 0xFF;

    RUN_CPU(&cpu);

    printf("BIT Absolute\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_ADC_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("ADC Immediate\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Immediate Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("ADC Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Zero Page Overflow\n");
    printf("value = 0x%04x.\n", cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW);


    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_X = 0x06;

    cpu.MEMORY[0xE8] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("ADC Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.REGISTER_INDEX_X = 0x06;

    cpu.MEMORY[0xE8] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Zero Page X Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2E5] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("ADC Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.MEMORY[0xE2E5] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Absolute Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("ADC Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Absolute X Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_Y = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("ADC Absolute Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.REGISTER_INDEX_Y = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Absolute Y Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_INDIRECT_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("ADC Indirect X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Indirect X Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_ADC_INDIRECT_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("ADC Indirect Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x09);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0x01;

    cpu.MEMORY[0xFFFC] = ADC_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("ADC Indirect Y Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}


void TEST_SBC_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("SBC Immediate\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x06);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

    CPU6502_T cpu_o;

    RESET_CPU(&cpu_o);

    cpu_o.REGISTER_ACCUMULATOR = 0xF0;
    cpu_o.MEMORY[0xE2] = 0xFF;

    cpu_o.MEMORY[0xFFFC] = SBC_IMMEDIATE;
    cpu_o.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu_o);
    
    printf("\n");

    printf("SBC Immediate Overflow\n");

    printf("acc : 0x%04x.\n", cpu_o.REGISTER_ACCUMULATOR);

    printf("\t");
    EQUAL("REGISTER",cpu_o.REGISTER_ACCUMULATOR, 0xf0);
    printf("\t");
    EQUAL("CYCLE",cpu_o.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("SBC Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x06);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

    CPU6502_T cpu_o;

    RESET_CPU(&cpu_o);

    cpu_o.REGISTER_ACCUMULATOR = 0xF0;
    cpu_o.MEMORY[0xE2] = 0xFF;

    cpu_o.MEMORY[0xFFFC] = SBC_ZERO_PAGE;
    cpu_o.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu_o);
    
    printf("\n");

    printf("SBC Zero Page Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu_o.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu_o.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_X = 0x06;

    cpu.MEMORY[0xE8] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("SBC Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x06);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

    CPU6502_T cpu_o;

    RESET_CPU(&cpu_o);

    cpu_o.REGISTER_ACCUMULATOR = 0xF0;
    cpu_o.REGISTER_INDEX_X = 0x06;

    cpu_o.MEMORY[0xE8] = 0xFF;

    cpu_o.MEMORY[0xFFFC] = SBC_ZERO_PAGE_X;
    cpu_o.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu_o);
    
    printf("\n");

    printf("SBC Zero Page X Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu_o.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu_o.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu_o.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2E5] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("SBC Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x06);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xF0;
    cpu.MEMORY[0xE2E5] = 0xFF;

    cpu.MEMORY[0xFFFC] = SBC_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("SBC Absolute Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xFF;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("SBC Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xFD);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xF0;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xE2E8] = 0xFF;

    cpu.MEMORY[0xFFFC] = SBC_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("SBC Absolute X Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x09;
    cpu.REGISTER_INDEX_Y = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("SBC Absolute Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x07);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xF0;
    cpu.REGISTER_INDEX_Y = 0x03;

    cpu.MEMORY[0xE2E8] = 0xFF;

    cpu.MEMORY[0xFFFC] = SBC_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("SBC Absolute Y Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_INDIRECT_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("SBC Indirect X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x06);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xF0;

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0xFF;

    cpu.MEMORY[0xFFFC] = SBC_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("SBC Indirect X Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_SBC_INDIRECT_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0x01;

    cpu.MEMORY[0xFFFC] = SBC_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("SBC Indirect Y\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x06);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);


    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0xF0;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0xFF;

    cpu.MEMORY[0xFFFC] = SBC_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("\n");

    printf("SBC Indirect Y Overflow\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("OVERFLOW FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}


void TEST_CMP_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2] = 0x08;

    cpu.MEMORY[0xFFFC] = CMP_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CMP Immediate\n");


    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
 
}

void TEST_CMP_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = CMP_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CMP Zero Page\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CMP_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_X = 0x06;

    cpu.MEMORY[0xE8] = 0x10;

    cpu.MEMORY[0xFFFC] = CMP_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CMP Zero Page X\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_CMP_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.MEMORY[0xE2E5] = 0xF1;

    cpu.MEMORY[0xFFFC] = CMP_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("CMP Absolute\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CMP_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x01;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xE2E8] = 0xFF;

    cpu.MEMORY[0xFFFC] = CMP_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("CMP Absolute X\n");


    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CMP_ABSOLUTE_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x09;
    cpu.REGISTER_INDEX_Y = 0x03;

    cpu.MEMORY[0xE2E8] = 0x01;

    cpu.MEMORY[0xFFFC] = CMP_ABSOLUTE_Y;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("CMP Absolute Y\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);

    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CMP_INDIRECT_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;

    cpu.REGISTER_INDEX_X = 0x0B;
    cpu.MEMORY[0x002B] = 0x30;
    cpu.MEMORY[0x002C] = 0xAC;
    cpu.MEMORY[0xAC30] = 0x01;

    cpu.MEMORY[0xFFFC] = CMP_INDIRECT_X;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("CMP Indirect X\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CMP_INDIRECT_Y()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x08;
    cpu.REGISTER_INDEX_Y = 0x04;
    cpu.MEMORY[0x0020] = 0x30;
    cpu.MEMORY[0x0021] = 0xAC;
    cpu.MEMORY[0xAC34] = 0x01;

    cpu.MEMORY[0xFFFC] = CMP_INDIRECT_Y;
    cpu.MEMORY[0xFFFD] = 0x20;

    RUN_CPU(&cpu);
    
    printf("CMP Indirect Y\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);

}


void TEST_CPX_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x08;
    cpu.MEMORY[0xE2] = 0x08;

    cpu.MEMORY[0xFFFC] = CPX_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CPX Immediate\n");


    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
 
}

void TEST_CPX_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = CPX_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CPX Zero Page\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CPX_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0x08;
    cpu.MEMORY[0xE2E5] = 0xF1;

    cpu.MEMORY[0xFFFC] = CPX_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("CPX Absolute\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CPY_IMMEDIATE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x08;
    cpu.MEMORY[0xE2] = 0x08;

    cpu.MEMORY[0xFFFC] = CPY_IMMEDIATE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CPY Immediate\n");


    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
 
}

void TEST_CPY_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x08;
    cpu.MEMORY[0xE2] = 0x01;

    cpu.MEMORY[0xFFFC] = CPY_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("CPY Zero Page\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 3);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_CPY_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0x08;
    cpu.MEMORY[0xE2E5] = 0xF1;

    cpu.MEMORY[0xFFFC] = CPY_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("CPY Absolute\n");

    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 4);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_DEC_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2] = 0xF1;

    cpu.MEMORY[0xFFFC] = DEC_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("DEC Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2], 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_DEC_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE5] = 0xF1;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = DEC_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("DEC Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE5], 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_DEC_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E5] = 0xF1;

    cpu.MEMORY[0xFFFC] = DEC_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("DEC Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E5], 0xF0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_DEC_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E8] = 0x01;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = DEC_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("DEC Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E8], 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 7);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}


void TEST_INC_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2] = 0xF1;

    cpu.MEMORY[0xFFFC] = INC_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("INC Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2], 0xF2);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_INC_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE5] = 0xF1;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = INC_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("INC Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE5], 0xF2);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_INC_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E5] = 0xF1;

    cpu.MEMORY[0xFFFC] = INC_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("INC Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E5], 0xF2);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_INC_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E8] = 0x01;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = INC_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("INC Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E8], 0x02);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 7);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0);
}

void TEST_INX_IMPLIED()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_X = 0xF1;

    cpu.MEMORY[0xFFFC] = INX_IMPLIED;

    RUN_CPU(&cpu);
    
    printf("INX Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_X, 0xF2);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_INY_IMPLIED()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_INDEX_Y = 0xF1;

    cpu.MEMORY[0xFFFC] = INY_IMPLIED;

    RUN_CPU(&cpu);
    
    printf("INY Implied\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_INDEX_Y, 0xF2);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1);
}

void TEST_ASL_ACCUMULATOR()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x80;

    cpu.MEMORY[0xFFFC] = ASL_ACCUMULATOR;

    RUN_CPU(&cpu);
    
    printf("ASL Accumulator\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ASL_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2] = 0x80;

    cpu.MEMORY[0xFFFC] = ASL_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("ASL Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2], 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ASL_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE5] = 0x80;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = ASL_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("ASL Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE5], 0x00);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}


void TEST_ASL_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E5] = 0x0A;

    cpu.MEMORY[0xFFFC] = ASL_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("ASL Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E5], 0x14);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ASL_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E8] = 0x30;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = ASL_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("ASL Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E8], 0x60);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 7);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 1);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}



void TEST_LSR_ACCUMULATOR()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x80;

    cpu.MEMORY[0xFFFC] = LSR_ACCUMULATOR;

    RUN_CPU(&cpu);
    
    printf("LSR Accumulator\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x40);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_LSR_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2] = 0x80;

    cpu.MEMORY[0xFFFC] = LSR_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("LSR Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2], 0x40);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_LSR_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE5] = 0x80;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = LSR_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;

    RUN_CPU(&cpu);
    
    printf("LSR Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE5], 0x40);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}


void TEST_LSR_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E5] = 0x0A;

    cpu.MEMORY[0xFFFC] = LSR_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("LSR Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E5], 0x05);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_LSR_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0xE2E8] = 0x30;
    cpu.REGISTER_INDEX_X = 0x03;

    cpu.MEMORY[0xFFFC] = LSR_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0xE2;
    cpu.MEMORY[0xFFFE] = 0xE5;

    RUN_CPU(&cpu);
    
    printf("LSR Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0xE2E8], 0x18);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 7);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ROL_ACCUMULATOR()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x80;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROL_ACCUMULATOR;

    RUN_CPU(&cpu);
    
    printf("ROL Accumulator\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0x01);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ROL_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x52] = 0x00;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROL_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x52;

    RUN_CPU(&cpu);
    
    printf("ROL Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x52], 0x01);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ROL_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x5A] = 0x07;
    cpu.REGISTER_INDEX_X = 0x08;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROL_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x52;

    RUN_CPU(&cpu);
    
    printf("ROL Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x5A], 0x0F);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ROL_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x5253] = 0x08;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROL_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x52;
    cpu.MEMORY[0xFFFE] = 0x53;

    RUN_CPU(&cpu);
    
    printf("ROL Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x5253], 0x11);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}

void TEST_ROL_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x5258] = 0x06;
    cpu.REGISTER_INDEX_X = 0x05;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROL_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0x52;
    cpu.MEMORY[0xFFFE] = 0x53;

    RUN_CPU(&cpu);
    
    printf("ROL Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x5258], 0x0D);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 7);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}


void TEST_ROR_ACCUMULATOR()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.REGISTER_ACCUMULATOR = 0x80;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROR_ACCUMULATOR;

    RUN_CPU(&cpu);
    
    printf("ROR Accumulator\n");

    printf("\t");
    EQUAL("REGISTER",cpu.REGISTER_ACCUMULATOR, 0xC0);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 2);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1); 
}

void TEST_ROR_ZERO_PAGE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x52] = 0x00;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROR_ZERO_PAGE;
    cpu.MEMORY[0xFFFD] = 0x52;

    RUN_CPU(&cpu);
    
    printf("ROR Zero Page\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x52], 0x80);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 5);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1); 
}

void TEST_ROR_ZERO_PAGE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x5A] = 0x07;
    cpu.REGISTER_INDEX_X = 0x08;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 1;

    cpu.MEMORY[0xFFFC] = ROR_ZERO_PAGE_X;
    cpu.MEMORY[0xFFFD] = 0x52;

    RUN_CPU(&cpu);
    
    printf("ROR Zero Page X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x5A], 0x83);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 1); 
}

void TEST_ROR_ABSOLUTE()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x5253] = 0x08;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 0;

    cpu.MEMORY[0xFFFC] = ROR_ABSOLUTE;
    cpu.MEMORY[0xFFFD] = 0x52;
    cpu.MEMORY[0xFFFE] = 0x53;

    RUN_CPU(&cpu);
    
    printf("ROR Absolute\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x5253], 0x04);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 6);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 0);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}




void TEST_ROR_ABSOLUTE_X()
{
    CPU6502_T cpu;

    RESET_CPU(&cpu);

    cpu.MEMORY[0x5258] = 0x07;
    cpu.REGISTER_INDEX_X = 0x05;
    cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = 0;

    cpu.MEMORY[0xFFFC] = ROR_ABSOLUTE_X;
    cpu.MEMORY[0xFFFD] = 0x52;
    cpu.MEMORY[0xFFFE] = 0x53;

    RUN_CPU(&cpu);
    
    printf("ROR Absolute X\n");

    printf("\t");
    EQUAL("REGISTER",cpu.MEMORY[0x5258], 0x03);
    printf("\t");
    EQUAL("CYCLE",cpu.TOTAL_CYCLE_COUNT, 7);
    printf("\t");
    EQUAL("CARRY FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY, 1);
    printf("\t");
    EQUAL("ZERO FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO, 0);
    printf("\t");
    EQUAL("NEGATIVE FLAG",cpu.REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE, 0); 
}


void TEST_LDA()
{
    TEST_LDA_IMMEDIATE();

    TEST_LDA_ZERO_PAGE();
    TEST_LDA_ZERO_PAGE_X();

    TEST_LDA_ABSOLUTE();
    TEST_LDA_ABSOLUTE_X();
    TEST_LDA_ABSOLUTE_Y();

    TEST_LDA_INDEXED_X();
    TEST_LDA_INDEXED_Y();
}

void TEST_LDX()
{
    TEST_LDX_IMMEDIATE();

    TEST_LDX_ZERO_PAGE();
    TEST_LDX_ZERO_PAGE_Y();

    TEST_LDX_ABSOLUTE();
    TEST_LDX_ABSOLUTE_Y();
}

void TEST_LDY()
{
    TEST_LDY_IMMEDIATE();

    TEST_LDY_ZERO_PAGE();
    TEST_LDY_ZERO_PAGE_X();

    TEST_LDY_ABSOLUTE();
    TEST_LDY_ABSOLUTE_X();
}

void TEST_STA()
{
    TEST_STA_ZERO_PAGE();
    TEST_STA_ZERO_PAGE_X();

    TEST_STA_ABSOLUTE();
    TEST_STA_ABSOLUTE_X();
    TEST_STA_ABSOLUTE_Y();

    TEST_STA_INDIRECT_X();
    TEST_STA_INDIRECT_Y();
}

void TEST_STX()
{
    TEST_STX_ZERO_PAGE();
    TEST_STX_ZERO_PAGE_Y();

    TEST_STX_ABSOLUTE();
}

void TEST_STY()
{
    TEST_STY_ZERO_PAGE();
    TEST_STY_ZERO_PAGE_X();
    TEST_STY_ABSOLUTE();
}

void TEST_AND()
{
    TEST_AND_IMMEDIATE();
    TEST_AND_ZERO_PAGE();
    TEST_AND_ZERO_PAGE_X();
    TEST_AND_ABSOLUTE();
    TEST_AND_ABSOLUTE_X();
    TEST_AND_ABSOLUTE_Y();
    TEST_AND_INDIRECT_X();
    TEST_AND_INDIRECT_Y();
}

void TEST_ADC()
{
    TEST_ADC_IMMEDIATE();
    TEST_ADC_ZERO_PAGE();
    TEST_ADC_ZERO_PAGE_X();
    TEST_ADC_ABSOLUTE();
    TEST_ADC_ABSOLUTE_X();
    TEST_ADC_ABSOLUTE_Y();
    TEST_ADC_INDIRECT_X();
    TEST_ADC_INDIRECT_Y();
}

void TEST_SBC()
{
    TEST_SBC_IMMEDIATE();
    TEST_SBC_ZERO_PAGE();
    TEST_SBC_ZERO_PAGE_X();
    TEST_SBC_ABSOLUTE();
    TEST_SBC_ABSOLUTE_X();
    TEST_SBC_ABSOLUTE_Y();
    TEST_SBC_INDIRECT_X();
    TEST_SBC_INDIRECT_Y();
}

void TEST_CMP()
{
    TEST_CMP_IMMEDIATE();
    TEST_CMP_ZERO_PAGE();
    TEST_CMP_ZERO_PAGE_X();
    TEST_CMP_ABSOLUTE();
    TEST_CMP_ABSOLUTE_X();
    TEST_CMP_ABSOLUTE_Y();
    TEST_CMP_INDIRECT_X();
    TEST_CMP_INDIRECT_Y();
}

void TEST_CPX()
{
    TEST_CPX_IMMEDIATE();
    TEST_CPX_ZERO_PAGE();
    TEST_CPX_ABSOLUTE();

}

void TEST_CPY()
{
    TEST_CPY_IMMEDIATE();
    TEST_CPY_ZERO_PAGE();
    TEST_CPY_ABSOLUTE();
}

void TEST_BIT()
{
    TEST_BIT_ZERO_PAGE();
    TEST_BIT_ABSOLUTE();
}

void TEST_DEC()
{
    TEST_DEC_ZERO_PAGE();
    TEST_DEC_ZERO_PAGE_X();
    TEST_DEC_ABSOLUTE();
    TEST_DEC_ABSOLUTE_X();

}

void TEST_INC()
{
    TEST_INC_ZERO_PAGE();
    TEST_INC_ZERO_PAGE_X();
    TEST_INC_ABSOLUTE();
    TEST_INC_ABSOLUTE_X();
}

void TEST_ASL()
{
    TEST_ASL_ACCUMULATOR();
    TEST_ASL_ZERO_PAGE();
    TEST_ASL_ZERO_PAGE_X();
    TEST_ASL_ABSOLUTE();
    TEST_ASL_ABSOLUTE_X();
}


void TEST_LSR()
{
    TEST_LSR_ACCUMULATOR();
    TEST_LSR_ZERO_PAGE();
    TEST_LSR_ZERO_PAGE_X();
    TEST_LSR_ABSOLUTE();
    TEST_LSR_ABSOLUTE_X();
}


void TEST_ROL()
{
    TEST_ROL_ACCUMULATOR();
    TEST_ROL_ZERO_PAGE();
    TEST_ROL_ZERO_PAGE_X();
    TEST_ROL_ABSOLUTE();
    TEST_ROL_ABSOLUTE_X();
}

void TEST_ROR()
{
    TEST_ROR_ACCUMULATOR();
    TEST_ROR_ZERO_PAGE();
    TEST_ROR_ZERO_PAGE_X();
    TEST_ROR_ABSOLUTE();
    TEST_ROR_ABSOLUTE_X();
}

void TEST_TOTAL_INSTRUCTION()
{
 
/*     TEST_LDA();
    TEST_LDX();
    TEST_LDY();

    TEST_STA();
    TEST_STX();
    TEST_STY();

    TEST_AND();

    TEST_TAX();
    TEST_TAY();
    TEST_TSX();
    TEST_TXA();
    TEST_TXS();
    TEST_TYA();

    TEST_PHA();
    TEST_PHP();
    TEST_PLA();
    TEST_PLP(); */

    //TEST_BIT();

    //TEST_ADC();

    //TEST_SBC();

    //TEST_CMP();

    //TEST_CPX();

    //TEST_CPY();

    //TEST_DEC();

    //TEST_INC();

    //TEST_INX_IMPLIED();
    //TEST_INY_IMPLIED();

    //TEST_ASL();
    //TEST_LSR();

    //TEST_ROL();
    TEST_ROR();
}
#endif
