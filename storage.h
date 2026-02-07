#ifndef STORAGE_H
#define STORAGE_H

#include <cstdint>

class Storage {
    public:
        Storage(){};

        virtual uint16_t readMem(uint16_t address){return storage[address];};
        virtual void writeMem(uint16_t address, uint16_t data){storage[address] = data;};
    protected:
        uint16_t storage[65536];
};

#endif