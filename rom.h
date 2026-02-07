#ifndef ROM_H
#define ROM_H

#include "storage.h"
#include <fstream>

class ROM : public Storage{
    public:
        ROM();

        uint16_t readMem(uint16_t address) override;
        void writeMem(uint16_t address, uint16_t data) override;
    private:
        void initializeMemory();
};

#endif ROM_H