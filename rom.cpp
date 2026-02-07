#include "rom.h"

ROM::ROM(){
    initializeMemory();
}

uint16_t ROM::readMem(uint16_t address){

}

void ROM::writeMem(uint16_t address, uint16_t data){

}



void ROM::initializeMemory(){
    std::ifstream file("rom.txt", std::ios::binary);
    if(!file){
        throw std::runtime_error("Failed to open file");
    }
}