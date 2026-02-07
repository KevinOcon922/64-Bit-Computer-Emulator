#ifndef CPU_H
#define CPU_H

#include <cstdint>
//Although not realistic because RAM is outside of the CPU, this made programming implementation more straightforward
#include "ram.h"
#include <cmath>

//Instruction: * General
//  * bits 0-3 give OPCODE:
//      * 0000: move data imm to reg2 (combines op1 and op2 as one byte)
//      * 0001: move data reg1 to reg2 (use only op2 for source)
//      * 0010: load data (bits 4-15) into register 10
//      * 0011: read from storage (reg 10 is used as address within storage device), dest is still register destination, op1-op2 used to select memory device
//      * 0100: write to storage (reg 10 is used as address within storage device), op1-op2 used to select memory device, dest is source register address
//      * 0101: push to stack (stack is ram addresses 64535-65535) if bit 4 is 1, use immediate value from bits 5-15, otherwise push value in dest register
//      * 0110: pop from stack. If bit 4 is high, write popped data to dest, otherwise, throw it out
//      * 0111: add values in registers addressed by op1 and op2 and store the value in dest register
//      * 1000: sub register values op1 - op2 and store result in dest register
//      * 1001: bitwise not. Performs bitwise not on the value in the register addressed by op2 and stores it in the dest register. op1 unnused
//      * 1010: bitwise and. Performs bitwise and on the values addressed by op1 and op2 and stores the restul in the dest register
//      * 1011: bitwise or. Performs bitwise or on the values addressed by op1 and op2 and stores the result in the dest register
//      * 1100: bitwise xor. Performs bitwise xor on the values addressed by op1 and op2 and stores the result in the dest register
//      * 1101: shift left. Shifts the value in register addressed by op1 an amount stored in the register addressed by op2 and stores the result in dest register
//      * 1110: shift right. Shifts the value in register addressed by op1 an amount stored in the register addressed by op2 and stores the result in dest register
//      * 1111: branch. Branches depending on the conditions defined by the bits following the opcode. The branch address is stored in register 15:
//          *
//  * bit 4-7 give op1
//  * bits 8-11 give op2
//  * bits 12-15 give destination (dest)

//Registers:
//  * Registers 0-11 are general purpose
//  * Register 12 is program counter
//  * Register 13 is stack pointer
//  * Register 14 is status flags
//      * Bit 0: Z (1 if last operation resulted in 0, 0 otherwise)
//      * Bit 1: N (1 if MSB is high, 0 otherwise)
//      * Bit 2: C (1 if was a carry out from the MSB and 0 otherwise)
//      * Bit 3: O (1 if there was a signed overflow)
//  * Register 15 is branch destination. It stores the address in RAM that the program will be jumped to if a branch is taken

#define PC 12
#define SP 13
#define STATUS 14
#define BRANCH 15

class CPU {
    public:
        //CPU object initializer - sets every element of registers to 0
        CPU();

        //How the CPU object is interacted with externally. A 16 bit instruction is input, which is then decoded and executed by internal program logic
        void processInstruction(uint16_t instruction);
        uint16_t getProgramCounterData();
    private:
        RAM Ram;
        
        Storage storage[3];
        uint16_t registers[16];

        bool ifEvaluateBranch(uint16_t condition);
};

#endif