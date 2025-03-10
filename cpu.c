#include "cpu.h"

void RESET_CPU(CPU6502_T* cpu)
{
    memset(cpu->MEMORY, 0x00, ADRESSING_SIZE);

    cpu->TOTAL_CYCLE_COUNT = 0;
    
    cpu->REGISTER_STACK_POINTER = 0x01FF;
    cpu->REGISTER_PROGRAM_COUNTER = 0xFFFC;

    cpu->REGISTER_ACCUMULATOR = 0;
    cpu->REGISTER_INDEX_X = 0;
    cpu->REGISTER_INDEX_Y = 0;


    cpu->REGISTER_PROCESSOR_T.REGISTER_STATUS = 0;
}

inline void PUSH_TO_STACK(CPU6502_T* cpu, BYTE data)
{
    cpu->MEMORY[cpu->REGISTER_STACK_POINTER] = data;
    --cpu->REGISTER_STACK_POINTER;

    ++cpu->TOTAL_CYCLE_COUNT;
    ++cpu->TOTAL_CYCLE_COUNT;

}

inline void POP_FROM_STACK(CPU6502_T* cpu, BYTE* data)
{
    ++cpu->REGISTER_STACK_POINTER;
    *data= cpu->MEMORY[cpu->REGISTER_STACK_POINTER];
    cpu->TOTAL_CYCLE_COUNT++;
    cpu->TOTAL_CYCLE_COUNT++;

    cpu->MEMORY[cpu->REGISTER_STACK_POINTER] = 0;
    cpu->TOTAL_CYCLE_COUNT++;
}

inline void FETCHING_BYTE(CPU6502_T* cpu, BYTE* data)
{
    *data = cpu->MEMORY[cpu->REGISTER_PROGRAM_COUNTER];
    cpu->MEMORY[cpu->REGISTER_PROGRAM_COUNTER] = 0;

    cpu->REGISTER_PROGRAM_COUNTER ++;
    cpu->TOTAL_CYCLE_COUNT++;

}

inline void FETCHING_WORD(CPU6502_T* cpu, WORD* data)
{
    *data = (cpu->MEMORY[cpu->REGISTER_PROGRAM_COUNTER] << 8);
    cpu->MEMORY[cpu->REGISTER_PROGRAM_COUNTER] = 0;

    cpu->REGISTER_PROGRAM_COUNTER ++;
    cpu->TOTAL_CYCLE_COUNT++;

    *data |= cpu->MEMORY[cpu->REGISTER_PROGRAM_COUNTER];
    cpu->MEMORY[cpu->REGISTER_PROGRAM_COUNTER] = 0;

    cpu->REGISTER_PROGRAM_COUNTER ++;
    cpu->TOTAL_CYCLE_COUNT++;

}

inline void READ_FROM_BYTE_ADDRESS(CPU6502_T* cpu, BYTE* Address, BYTE* data)
{
    *data = cpu->MEMORY[*Address];
    cpu->TOTAL_CYCLE_COUNT++;
}

inline void READ_FROM_WORD_ADDRESS(CPU6502_T* cpu, WORD* Address, BYTE* data)
{
    *data = cpu->MEMORY[*Address];
    cpu->TOTAL_CYCLE_COUNT++;
}

inline void WRITE_TO_BYTE_ADDRESS(CPU6502_T* cpu, BYTE* Address, BYTE* data)
{
    cpu->MEMORY[*Address] = *data;
    cpu->TOTAL_CYCLE_COUNT++;
}

inline void WRITE_TO_WORD_ADDRESS(CPU6502_T* cpu, WORD* Address, BYTE* data)
{
    cpu->MEMORY[*Address] = *data;
    cpu->TOTAL_CYCLE_COUNT++;
}

inline void IMMEDIATE_MODE(CPU6502_T* cpu, BYTE* data)
{
    FETCHING_BYTE(cpu,data);
}

inline void ZERO_PAGE_MODE(CPU6502_T* cpu, BYTE* data, unsigned int type)
{
    BYTE ZeroPageAdress;
    FETCHING_BYTE(cpu,&ZeroPageAdress);

    if(type)
        READ_FROM_BYTE_ADDRESS(cpu, &ZeroPageAdress, data);
    else
        WRITE_TO_BYTE_ADDRESS(cpu, &ZeroPageAdress, data);

}

inline void ZERO_PAGE_INDEX_MODE(CPU6502_T* cpu, BYTE* index, BYTE* data, unsigned int type)
{
    BYTE ZeroPageAddress;
    FETCHING_BYTE(cpu, &ZeroPageAddress);

    ZeroPageAddress += *index;
    cpu->TOTAL_CYCLE_COUNT++;
    
    if(type)
        READ_FROM_BYTE_ADDRESS(cpu, &ZeroPageAddress, data);
    else
        WRITE_TO_BYTE_ADDRESS(cpu, &ZeroPageAddress, data);
}

inline void ABSOLUTE_MODE(CPU6502_T* cpu, BYTE* data, unsigned int type)
{
    WORD Address;
    FETCHING_WORD(cpu, &Address);

    if(type)
        READ_FROM_WORD_ADDRESS(cpu, &Address, data);
    else
        WRITE_TO_WORD_ADDRESS(cpu, &Address, data);
}

inline void ABSOLUTE_INDEX_MODE(CPU6502_T* cpu, BYTE* index, BYTE* data, unsigned int type)
{
    WORD Address;
    FETCHING_WORD(cpu, &Address);

    if(type)
    {
        BYTE MSB = Address >> 8;
        Address += *index;

        cpu->TOTAL_CYCLE_COUNT += ((Address ^ (MSB << 8))& 0xFF00);
    
        READ_FROM_WORD_ADDRESS(cpu, &Address, data);
    }
    else
        {
            Address += *index;
            cpu->TOTAL_CYCLE_COUNT++;
            WRITE_TO_WORD_ADDRESS(cpu, &Address, data);
        }


}

inline void INDIRECT_X_MODE(CPU6502_T* cpu, BYTE* data, unsigned int type)
{
    BYTE ZeroPageAddress;
    FETCHING_BYTE(cpu, &ZeroPageAddress);

    WORD Address = ((cpu->MEMORY[ZeroPageAddress + cpu->REGISTER_INDEX_X + 1]) << 8);
    cpu->TOTAL_CYCLE_COUNT ++;

    Address |= ((cpu->MEMORY[ZeroPageAddress + cpu->REGISTER_INDEX_X]));
    cpu->TOTAL_CYCLE_COUNT ++;
    cpu->TOTAL_CYCLE_COUNT ++;


    if(type)
        READ_FROM_WORD_ADDRESS(cpu, &Address, data);
    else
        WRITE_TO_WORD_ADDRESS(cpu, &Address, data);

}


inline void INDIRECT_Y_MODE(CPU6502_T* cpu, BYTE* data, unsigned int type)
{
    BYTE ZeroPageAddress;
    FETCHING_BYTE(cpu, &ZeroPageAddress);

    WORD Address = ((cpu->MEMORY[ZeroPageAddress + 1]) << 8);
    cpu->TOTAL_CYCLE_COUNT ++;

    Address |= ((cpu->MEMORY[ZeroPageAddress]));
    cpu->TOTAL_CYCLE_COUNT ++;


    if(type)
        {
            BYTE MSB = Address >> 8;
            Address += cpu->REGISTER_INDEX_Y;
    
            cpu->TOTAL_CYCLE_COUNT += ((Address ^ (MSB << 8))& 0xFF00);

            READ_FROM_WORD_ADDRESS(cpu, &Address, data);
        }
    else
        {
            Address += cpu->REGISTER_INDEX_Y;
            cpu->TOTAL_CYCLE_COUNT++;
            WRITE_TO_WORD_ADDRESS(cpu, &Address, data);
        }
}

void RUN_CPU(CPU6502_T* cpu)
{
    BYTE opcode;
    while(1)
    {
        FETCHING_BYTE(cpu, &opcode);
        switch (opcode)
        {
            case LDA_IMMEDIATE:
            {
                IMMEDIATE_MODE(cpu, PTR_ACCUMULATOR);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;
            
            case LDA_ZERO_PAGE:
            {
                ZERO_PAGE_MODE(cpu, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case LDA_ZERO_PAGE_X:
            {
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case LDA_ABSOLUTE:
            {
                ABSOLUTE_MODE(cpu, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case LDA_ABSOLUTE_X:
            {
                ABSOLUTE_INDEX_MODE(cpu, PTR_X, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case LDA_ABSOLUTE_Y:
            {
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case LDA_INDIRECT_X:
            {

                INDIRECT_X_MODE(cpu, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case LDA_INDIRECT_Y:
            {
                INDIRECT_Y_MODE(cpu, PTR_ACCUMULATOR, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;
            
            case LDX_IMMEDIATE:
            {
                IMMEDIATE_MODE(cpu, PTR_X);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;             
            }
            break;

            case LDX_ZERO_PAGE:
            {
                ZERO_PAGE_MODE(cpu, PTR_X, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;
            }
            break;

            case LDX_ZERO_PAGE_Y:
            {
                ZERO_PAGE_INDEX_MODE(cpu, PTR_Y, PTR_X, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;
            }
            break;

            case LDX_ABSOLUTE:
            {
                ABSOLUTE_MODE(cpu, PTR_X, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;
            }
            break;

            case LDX_ABSOLUTE_Y:
            {
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y, PTR_X, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;
            }
            break;

            case LDY_IMMEDIATE:
            {
                IMMEDIATE_MODE(cpu, PTR_Y);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_Y >> 7) == 1) ;             
            }
            break;

            case LDY_ZERO_PAGE:
            {
                ZERO_PAGE_MODE(cpu, PTR_Y, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_Y >> 7) == 1) ;
            }
            break;

            case LDY_ZERO_PAGE_X:
            {
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X, PTR_Y, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_Y >> 7) == 1) ;
            }
            break;

            case LDY_ABSOLUTE:
            {
                ABSOLUTE_MODE(cpu, PTR_Y, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_Y >> 7) == 1) ;
            }
            break;

            case LDY_ABSOLUTE_X:
            {
                ABSOLUTE_INDEX_MODE(cpu, PTR_X, PTR_Y, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_Y >> 7) == 1) ;
            }
            break;

            case STA_ZERO_PAGE:
            {
                ZERO_PAGE_MODE(cpu, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STA_ZERO_PAGE_X:
            {
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STA_ABSOLUTE:
            {
                ABSOLUTE_MODE(cpu, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STA_ABSOLUTE_X:
            {
                ABSOLUTE_INDEX_MODE(cpu, PTR_X, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STA_ABSOLUTE_Y:
            {
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STA_INDIRECT_X:
            {
                INDIRECT_X_MODE(cpu, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STA_INDIRECT_Y:
            {
                INDIRECT_Y_MODE(cpu, PTR_ACCUMULATOR, WRITE);
            }
            break;

            case STX_ZERO_PAGE:
            {
                ZERO_PAGE_MODE(cpu, PTR_X, WRITE);
            }
            break;

            case STX_ZERO_PAGE_Y:
            {
                ZERO_PAGE_INDEX_MODE(cpu, PTR_Y, PTR_X, WRITE);
            }
            break;

            case STX_ABSOLUTE:
            {
                ABSOLUTE_MODE(cpu, PTR_X, WRITE);
            }
            break;

            case STY_ZERO_PAGE:
            {
                ZERO_PAGE_MODE(cpu, PTR_Y, WRITE);
            }
            break;

            case STY_ZERO_PAGE_X:
            {
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X, PTR_Y, WRITE);
            }
            break;

            case STY_ABSOLUTE:
            {
                ABSOLUTE_MODE(cpu, PTR_Y, WRITE);
            }
            break;

            case TAX_IMPLIED:
            {
                cpu->REGISTER_INDEX_X = cpu->REGISTER_ACCUMULATOR;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;
            }
            break;

            case TAY_IMPLIED:
            {
                cpu->REGISTER_INDEX_Y = cpu->REGISTER_ACCUMULATOR;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_Y >> 7) == 1) ;
            }
            break;

            case TSX_IMPLIED:
            {
                cpu->REGISTER_INDEX_X = cpu->REGISTER_STACK_POINTER;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_INDEX_X >> 7) == 1) ;
            }
            break;

            case TXA_IMPLIED:
            {
                cpu->REGISTER_ACCUMULATOR = cpu->REGISTER_INDEX_X;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case TXS_IMPLIED:
            {
                cpu->REGISTER_STACK_POINTER = cpu->REGISTER_INDEX_X;
                cpu->TOTAL_CYCLE_COUNT += 1;
            }
            break;

            case TYA_IMPLIED:
            {
                cpu->REGISTER_ACCUMULATOR = cpu->REGISTER_INDEX_Y;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case PHA_IMPLIED:
            {
                PUSH_TO_STACK(cpu, cpu->REGISTER_ACCUMULATOR);
            }
            break;

            case PHP_IMPLIED:
            {
                PUSH_TO_STACK(cpu, cpu->REGISTER_PROCESSOR_T.REGISTER_STATUS);

            }
            break;

            case PLA_IMPLIED:
            {
                POP_FROM_STACK(cpu, PTR_ACCUMULATOR);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case PLP_IMPLIED:
            {
                POP_FROM_STACK(cpu, PTR_STATUS);
            }
            break;

            case AND_IMMEDIATE:
            {
                BYTE operand;
                IMMEDIATE_MODE(cpu, &operand);
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_ZERO_PAGE:
            {
                BYTE operand;
                ZERO_PAGE_MODE(cpu, &operand, READ);

                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_ZERO_PAGE_X:
            {
                BYTE operand;
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X,&operand, READ);
                
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_ABSOLUTE:
            {
                BYTE operand;
                ABSOLUTE_MODE(cpu, &operand, READ);
                
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_ABSOLUTE_X:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_X,&operand, READ);
                
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_ABSOLUTE_Y:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y,&operand, READ);
                
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_INDIRECT_X:
            {
                BYTE operand;
                INDIRECT_X_MODE(cpu, &operand, READ);
                
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case AND_INDIRECT_Y:
            {
                BYTE operand;
                INDIRECT_Y_MODE(cpu, &operand, READ);
                
                cpu->REGISTER_ACCUMULATOR &= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case BIT_ZERO_PAGE:
            {
                BYTE Mask;
                ZERO_PAGE_MODE(cpu, &Mask, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = ((cpu->REGISTER_ACCUMULATOR & Mask) != 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (Mask >> 6);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (Mask >> 7);
            }
            break;

            case BIT_ABSOLUTE:
            {
                BYTE Mask;
                ABSOLUTE_MODE(cpu, &Mask, READ);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = ((cpu->REGISTER_ACCUMULATOR & Mask) != 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (Mask >> 6);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (Mask >> 7);
            }
            break;

            default:
                break;
        }
        break;
    }
}
