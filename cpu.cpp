#include "cpu.h"

CPU::CPU(){
    registers[PC] = 0; //Program Counter
    registers[SP] = 64535; //Stack Pointer
    registers[STATUS] = 0; //Status Flags
    registers[BRANCH] = 0; //Branch destination

    for(int i = 0; i < 11; i++){
        registers[i] = 0;
    }

    storage[0] = Ram;
}

void CPU::processInstruction(uint16_t instruction){
    uint8_t opcode = (instruction & 0xF000) >> 12;
    uint16_t op1 = instruction & 0x0F00 >> 8;
    uint16_t op2 = instruction & 0x00F0 >> 4;
    uint16_t dest = instruction & 0x000F;

    bool statusUpdated = false;

    switch(opcode){
        //Move data
        case 0b0000:
            registers[dest] = (op1 << 4) + (op2);
            break;
        case 0b0001:
            registers[dest] = registers[op2];
            break;
        case 0b0010:
            registers[10] = (op1 << 8) + (op2 << 4) + (dest);
            if(registers[10] == 0){registers[STATUS] |= 0x0001;}
            else{registers[STATUS] &= ~0x0001;}
            if((registers[10] & 0x8000) > 0){registers[STATUS] |= 0x0002;}
            else{registers[STATUS] &= ~0x0002;}
            statusUpdated = true;
            break;
        case 0b0011:
            registers[dest] = storage[(op1 << 4) + (op2)].readMem(registers[10]);
            break;
        case 0b0100:
            storage[(op1 << 4) + (op2)].writeMem(registers[10], registers[dest]);
            statusUpdated = true;
            break;
        case 0b0101:
            //Ram is the zeroeth storage device
            if(op1 & 0x8 > 0){
                storage[0].writeMem(registers[SP], instruction & 0x07FF);
            } else {
                storage[0].writeMem(registers[SP], registers[dest]);
            }
            registers[SP]++;
            statusUpdated = true;
            break;
        case 0b0110:
            registers[SP]--;
            statusUpdated = true;
            if(op1 & 0x8 > 0){
                registers[dest] = storage[0].readMem(registers[SP]);
                statusUpdated = false;
            }
            break;
        case 0b0111:
            registers[dest] = registers[op1] + registers[op2];
            if(registers[dest] < registers[op1]){registers[STATUS] |= 0x0004;}
            else{registers[STATUS] &= ~0x0004;}
            if((registers[op1] & 0x8000) == (registers[op2] & 0x8000) && (registers[op1] & 0x8000) != (registers[dest] & 0x8000)){registers[STATUS] |= 0x0008;}
            else{registers[STATUS] &= ~0x0008;}
            break;
        case 0b1000:
            registers[dest] = registers[op1] - registers[op2];
            if(registers[dest] <= registers[op1]){registers[STATUS] |= 0x0004;}
            else{registers[STATUS] &= ~0x0004;}
            if((registers[op1] & 0x8000) != (registers[op2] & 0x8000) && (registers[dest] & 0x8000) != (registers[op1] & 0x8000)){registers[STATUS] |= 0x0008;}
            else{registers[STATUS] &= ~0x0008;}
            break;
        case 0b1001:
            registers[dest] = ~registers[op2];
            break;
        case 0b1010:
            registers[dest] = registers[op1] & registers[op2];
            break;
        case 0b1011:
            registers[dest] = registers[op1] | registers[op2];
            break;
        case 0b1100:
            registers[dest] = registers[op1] ^ registers[op2];
            break;
        case 0b1101: {
            registers[dest] = registers[op1] << registers[op2];
            uint16_t shift = registers[op2] & 0x000F;
            uint16_t mask = ((1 << shift) - 1) << (16-shift);
            if((registers[op1] & mask) > 0){registers[STATUS] |= 0x0004;}
            else{registers[STATUS] &= ~0x0004;}
            if((registers[op1] & 0x8000) != (registers[dest] & 0x8000)){registers[STATUS] |= 0x0008;}
            else{registers[STATUS] &= ~0x0008;}
            break;
        }
        case 0b1110:
            registers[dest] = registers[op1] >> registers[op2];
            if((registers[op1] & 0x8000) != (registers[dest] & 0x8000)){registers[STATUS] |= 0x0008;}
            else{registers[STATUS] &= ~0x0008;}
            break;
        case 0b1111:
            if(ifEvaluateBranch(instruction & 0x0FFF)){
                registers[PC] = registers[BRANCH];
            }
            statusUpdated = true;
            break;
        default:
            break;     
    }
    if(statusUpdated == false){
        if(registers[dest] == 0){
            registers[STATUS] |= 0x0001;
        } else {
            registers[STATUS] &= ~0x0001; 
        }
        if((registers[dest] & 0x8000) > 0){
            registers[STATUS] |= 0x0002;
        } else {
            registers[STATUS] &= ~0x0002;
        }
    }
}

uint16_t CPU::getProgramCounterData(){
    return registers[PC];
}



bool CPU::ifEvaluateBranch(uint16_t condition){
    uint16_t statusFlags = registers[STATUS];

    switch(condition){
        case 0b0:
            //Branch Always
            return true;
            break;
        case 0b1:
            //Branch if 0
            return (statusFlags & 0x0001) > 0;
            break;
        case 0b10:
            //Branch if not 0
            return (statusFlags & 0x0001) == 0;
            break;
        case 0b11:
            //Branch if greater than 0 (unsigned)
            return (statusFlags & 0x0001) == 0;
            break;
        case 0b100:
            //Branch if greater than 0 (signed)
            return (statusFlags & 0x0001) == 0 && (statusFlags & 0x0002) == 0;
            break;
        case 0b101:
            //Branch if greater than or equal to 0 (signed)
            return (statusFlags & 0x0002) == 0;
            break;
        case 0b110:
            //Branch if less than 0 (signed)
            return (statusFlags & 0x0002) > 0;
            break;
        case 0b111:
            //Branch if less than or equal to 0 (signed)
            return (statusFlags & 0x0002) > 0 || (statusFlags & 0x0001) > 0;
            break;
        case 0b1000:
            //Branch if carry out
            return (statusFlags & 0x0004) > 0;
            break;
        case 0b1001:
            //Branch if signed overflow
            return (statusFlags & 0x0008) > 0;
            break;
        default:
            break;
    }
    return false;
}