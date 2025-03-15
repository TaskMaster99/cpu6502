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

inline void DIRECT_ZERO_PAGE_MODE_WRITE(CPU6502_T* cpu, BYTE* ZeroPageAdress, BYTE* data)
{
    cpu->TOTAL_CYCLE_COUNT++;
    WRITE_TO_BYTE_ADDRESS(cpu, ZeroPageAdress, data);
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

inline void DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(CPU6502_T* cpu, BYTE* index, BYTE* ZeroPageAddress, BYTE* data)
{
    *ZeroPageAddress += *index;
    cpu->TOTAL_CYCLE_COUNT++;

    WRITE_TO_BYTE_ADDRESS(cpu, ZeroPageAddress, data);
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

inline void DIRECT_ABSOLUTE_MODE_WRITE(CPU6502_T* cpu, WORD* Address, BYTE* data)
{
    WRITE_TO_WORD_ADDRESS(cpu, Address, data);
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

inline void ABSOLUTE_INDEX_MODE_(CPU6502_T* cpu, BYTE* index, BYTE* data, unsigned int type)
{
    WORD Address;
    FETCHING_WORD(cpu, &Address);
    Address += *index;
    cpu->TOTAL_CYCLE_COUNT++;

    if(type)
    {    
        READ_FROM_WORD_ADDRESS(cpu, &Address, data);
    }
    else
    {
        WRITE_TO_WORD_ADDRESS(cpu, &Address, data);
    }

}

inline void DIRECT_ABSOLUTE_INDEX_MODE_WRITE(CPU6502_T* cpu, BYTE* index, WORD* Address, BYTE* data)
{
    *Address += *index;
    cpu->TOTAL_CYCLE_COUNT++;

    WRITE_TO_WORD_ADDRESS(cpu, Address, data);
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

            case EOR_IMMEDIATE:
            {
                BYTE operand;
                IMMEDIATE_MODE(cpu, &operand);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_ZERO_PAGE:
            {
                BYTE operand;
                ZERO_PAGE_MODE(cpu, &operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_ZERO_PAGE_X:
            {
                BYTE operand;
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X,&operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_ABSOLUTE:
            {
                BYTE operand;
                ABSOLUTE_MODE(cpu,&operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_ABSOLUTE_X:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_X,&operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_ABSOLUTE_Y:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y,&operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_INDIRECT_X:
            {
                BYTE operand;
                INDIRECT_X_MODE(cpu, &operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case EOR_INDIRECT_Y:
            {
                BYTE operand;
                INDIRECT_Y_MODE(cpu, &operand, READ);
                cpu->REGISTER_ACCUMULATOR ^= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_IMMEDIATE:
            {
                BYTE operand;
                IMMEDIATE_MODE(cpu, &operand);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_ZERO_PAGE:
            {
                BYTE operand;
                ZERO_PAGE_MODE(cpu, &operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_ZERO_PAGE_X:
            {
                BYTE operand;
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X,&operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_ABSOLUTE:
            {
                BYTE operand;
                ABSOLUTE_MODE(cpu,&operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_ABSOLUTE_X:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_X,&operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_ABSOLUTE_Y:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y,&operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_INDIRECT_X:
            {
                BYTE operand;
                INDIRECT_X_MODE(cpu, &operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ORA_INDIRECT_Y:
            {
                BYTE operand;
                INDIRECT_Y_MODE(cpu, &operand, READ);
                cpu->REGISTER_ACCUMULATOR |= operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = ((cpu->REGISTER_ACCUMULATOR >> 7) == 1) ;
            }
            break;

            case ADC_IMMEDIATE:
            {
                BYTE operand;
                IMMEDIATE_MODE(cpu, &operand);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                printf("operand = 0x%04x.\n", cpu->MEMORY[operand]);


                WORD result = cpu->REGISTER_ACCUMULATOR + cpu->MEMORY[operand] + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;


                printf("value = 0x%04x.\n", result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 0xFF);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ cpu->MEMORY[operand]) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case ADC_ZERO_PAGE:
            {
                BYTE operand;
                ZERO_PAGE_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;
                printf("operand = 0x%04x.\n", operand);

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                printf("value = 0x%04x.\n", result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 0xFF);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case ADC_ZERO_PAGE_X:
            {
                BYTE operand;
                ZERO_PAGE_INDEX_MODE(cpu, &cpu->REGISTER_INDEX_X,&operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;


                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);
            }
            break;

            case ADC_ABSOLUTE:
            {
                BYTE operand;
                ABSOLUTE_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case ADC_ABSOLUTE_X:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, &cpu->REGISTER_INDEX_X, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);
            }
            break;

            case ADC_ABSOLUTE_Y:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, &cpu->REGISTER_INDEX_Y, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case ADC_INDIRECT_X:
            {
                BYTE operand;
                INDIRECT_X_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);
            }
            break;
            
            case ADC_INDIRECT_Y:
            {
                BYTE operand;
                INDIRECT_Y_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR + operand + cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_IMMEDIATE:
            {
                BYTE operand;
                IMMEDIATE_MODE(cpu, &operand);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - cpu->MEMORY[operand] - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_ZERO_PAGE:
            {
                BYTE operand;
                ZERO_PAGE_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );
                printf("result = 0x%04x.\n", result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_ZERO_PAGE_X:
            {
                BYTE operand;
                ZERO_PAGE_INDEX_MODE(cpu, &cpu->REGISTER_INDEX_X,&operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_ABSOLUTE:
            {
                BYTE operand;
                ABSOLUTE_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_ABSOLUTE_X:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, &cpu->REGISTER_INDEX_X, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_ABSOLUTE_Y:
            {
                BYTE operand;
                ABSOLUTE_INDEX_MODE(cpu, &cpu->REGISTER_INDEX_Y, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;

            case SBC_INDIRECT_X:
            {
                BYTE operand;
                INDIRECT_X_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;
            
            case SBC_INDIRECT_Y:
            {
                BYTE operand;
                INDIRECT_Y_MODE(cpu, &operand, READ);

                BYTE a = cpu->REGISTER_ACCUMULATOR ;

                WORD result = cpu->REGISTER_ACCUMULATOR - operand - (1 - cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY );

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = !(result > 255);
                cpu->REGISTER_ACCUMULATOR = result;
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.OVERFLOW = (((a ^ operand) & (a ^ result) & 0x80)>> 7);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_ACCUMULATOR >> 7);

            }
            break;            

            case CMP_IMMEDIATE:
            {
                BYTE OperandAddress;
                IMMEDIATE_MODE(cpu, &OperandAddress);

                BYTE result = cpu->REGISTER_ACCUMULATOR - cpu->MEMORY[OperandAddress];

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= cpu->MEMORY[OperandAddress]);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == cpu->MEMORY[OperandAddress]);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_ZERO_PAGE:
            {
                BYTE Operand;
                ZERO_PAGE_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_ZERO_PAGE_X:
            {
                BYTE Operand;
                ZERO_PAGE_INDEX_MODE(cpu, PTR_X, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_ABSOLUTE:
            {
                BYTE Operand;
                ABSOLUTE_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_ABSOLUTE_X:
            {
                BYTE Operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_X, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;
                

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_ABSOLUTE_Y:
            {
                BYTE Operand;
                ABSOLUTE_INDEX_MODE(cpu, PTR_Y, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_INDIRECT_X:
            {
                BYTE Operand;
                INDIRECT_X_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CMP_INDIRECT_Y:
            {
                BYTE Operand;
                INDIRECT_Y_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_ACCUMULATOR - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_ACCUMULATOR == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CPX_IMMEDIATE:
            {
                BYTE OperandAddress;
                IMMEDIATE_MODE(cpu, &OperandAddress);

                BYTE result = cpu->REGISTER_INDEX_X - cpu->MEMORY[OperandAddress];

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X >= cpu->MEMORY[OperandAddress]);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_INDEX_X == cpu->MEMORY[OperandAddress]);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CPX_ZERO_PAGE:
            {
                BYTE Operand;
                ZERO_PAGE_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_INDEX_X - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_INDEX_X == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CPX_ABSOLUTE:
            {
                BYTE Operand;
                ABSOLUTE_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_INDEX_X - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_X >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_INDEX_X == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CPY_IMMEDIATE:
            {
                BYTE OperandAddress;
                IMMEDIATE_MODE(cpu, &OperandAddress);

                BYTE result = cpu->REGISTER_INDEX_Y - cpu->MEMORY[OperandAddress];

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y >= cpu->MEMORY[OperandAddress]);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_INDEX_Y == cpu->MEMORY[OperandAddress]);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CPY_ZERO_PAGE:
            {
                BYTE Operand;
                ZERO_PAGE_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_INDEX_Y - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_INDEX_Y == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case CPY_ABSOLUTE:
            {
                BYTE Operand;
                ABSOLUTE_MODE(cpu, &Operand, READ);

                BYTE result = cpu->REGISTER_INDEX_Y - Operand;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_INDEX_Y >= Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO  = (cpu->REGISTER_INDEX_Y == Operand);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;  

            case DEC_ZERO_PAGE:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress] - 1;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ZERO_PAGE_MODE_WRITE(cpu, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);

            }
            break;

            case DEC_ZERO_PAGE_X:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] - 1;
                cpu->TOTAL_CYCLE_COUNT++;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);

            }
            break;

            case DEC_ABSOLUTE:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress] - 1;
                cpu->TOTAL_CYCLE_COUNT++;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ABSOLUTE_MODE_WRITE(cpu, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case DEC_ABSOLUTE_X:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] - 1;
                cpu->TOTAL_CYCLE_COUNT++;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ABSOLUTE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case INC_ZERO_PAGE:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress] + 1;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ZERO_PAGE_MODE_WRITE(cpu, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);

            }
            break;

            case INC_ZERO_PAGE_X:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] + 1;
                cpu->TOTAL_CYCLE_COUNT++;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);

            }
            break;

            case INC_ABSOLUTE:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress] + 1;
                cpu->TOTAL_CYCLE_COUNT++;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ABSOLUTE_MODE_WRITE(cpu, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case INC_ABSOLUTE_X:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                BYTE result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] + 1;
                cpu->TOTAL_CYCLE_COUNT++;
                cpu->TOTAL_CYCLE_COUNT++;

                DIRECT_ABSOLUTE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case DEX_IMPLIED:
            {                

                cpu->REGISTER_INDEX_X -= 1;
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_INDEX_X >> 7);
            }
            break;

            case DEY_IMPLIED:
            {
                cpu->REGISTER_INDEX_Y -= 1;
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_INDEX_Y >> 7);
            }
            break;

            case INX_IMPLIED:
            {
                cpu->REGISTER_INDEX_X += 1;

                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_INDEX_X == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_INDEX_X >> 7);
            }
            break;

            case INY_IMPLIED:
            {
                cpu->REGISTER_INDEX_Y += 1;

                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_INDEX_Y == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (cpu->REGISTER_INDEX_Y >> 7);
            }
            break;

            case ASL_ACCUMULATOR:
            {
                WORD result = cpu->REGISTER_ACCUMULATOR << 1;
                cpu->REGISTER_ACCUMULATOR = result;

                cpu->TOTAL_CYCLE_COUNT++;
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result >> 8);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ASL_ZERO_PAGE:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] << 1;
                cpu->TOTAL_CYCLE_COUNT += 1;
                DIRECT_ZERO_PAGE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);
                                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result >> 8);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ASL_ZERO_PAGE_X:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] << 1;
                cpu->TOTAL_CYCLE_COUNT += 2;
                DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(cpu, PTR_X,&DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result >> 8);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ASL_ABSOLUTE:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] << 1;
                cpu->TOTAL_CYCLE_COUNT += 2;

                DIRECT_ABSOLUTE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result >> 8);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ASL_ABSOLUTE_X:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] << 1;
                cpu->TOTAL_CYCLE_COUNT += 2;

                DIRECT_ABSOLUTE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result >> 8);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case LSR_ACCUMULATOR:
            {
                WORD result = cpu->REGISTER_ACCUMULATOR >> 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (result >> 0);


                cpu->REGISTER_ACCUMULATOR = result;

                cpu->TOTAL_CYCLE_COUNT++;
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case LSR_ZERO_PAGE:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] >> 1;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress] >> 0);

                DIRECT_ZERO_PAGE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case LSR_ZERO_PAGE_X:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 1;

                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 0);

                DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(cpu, PTR_X,&DataAddress,(BYTE*)&result);
                
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case LSR_ABSOLUTE:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] >> 1;
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress] >> 0);


                DIRECT_ABSOLUTE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case LSR_ABSOLUTE_X:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 1;
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 0);

                DIRECT_ABSOLUTE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROL_ACCUMULATOR:
            {
                WORD result = cpu->REGISTER_ACCUMULATOR << 1 | cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >> 7);

                cpu->REGISTER_ACCUMULATOR = result;

                cpu->TOTAL_CYCLE_COUNT++;
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROL_ZERO_PAGE:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] << 1 | cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress] >> 7);

                DIRECT_ZERO_PAGE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROL_ZERO_PAGE_X:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] << 1 | cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 7);

                DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(cpu, PTR_X,&DataAddress,(BYTE*)&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROL_ABSOLUTE:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] << 1 | cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress] >> 7);


                DIRECT_ABSOLUTE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROL_ABSOLUTE_X:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] << 1 | cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY;

                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 7);

                DIRECT_ABSOLUTE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROR_ACCUMULATOR:
            {
                WORD result = cpu->REGISTER_ACCUMULATOR >> 1 | (cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY << 7);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->REGISTER_ACCUMULATOR >> 0);

                cpu->REGISTER_ACCUMULATOR = result;

                cpu->TOTAL_CYCLE_COUNT++;
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (cpu->REGISTER_ACCUMULATOR == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROR_ZERO_PAGE:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] >> 1 | (cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY << 7);
                cpu->TOTAL_CYCLE_COUNT += 1;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress] >> 0);

                DIRECT_ZERO_PAGE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROR_ZERO_PAGE_X:
            {
                BYTE DataAddress;
                
                FETCHING_BYTE(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 1 | (cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY << 7);
                
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 0);

                DIRECT_ZERO_PAGE_INDEX_MODE_WRITE(cpu, PTR_X,&DataAddress,(BYTE*)&result);

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROR_ABSOLUTE:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress] >> 1 | (cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY << 7);
                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress] >> 0);


                DIRECT_ABSOLUTE_MODE_WRITE(cpu, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;

            case ROR_ABSOLUTE_X:
            {
                WORD DataAddress;
                
                FETCHING_WORD(cpu, &DataAddress);

                WORD result = cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 1 | (cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY << 7);

                cpu->TOTAL_CYCLE_COUNT += 2;

                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.CARRY = (cpu->MEMORY[DataAddress + cpu->REGISTER_INDEX_X] >> 0);

                DIRECT_ABSOLUTE_INDEX_MODE_WRITE(cpu, PTR_X, &DataAddress,(BYTE*)&result);
                
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.ZERO = (result == 0);
                cpu->REGISTER_PROCESSOR_T.STATUS_FLAGS_T.NEGATIVE = (result >> 7);
            }
            break;
            default:
                break;
        }
        break;
    }
}
