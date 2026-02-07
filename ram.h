#ifndef RAM_H
#define RAM_H

#include "storage.h"
#include <cstdint>

//Stack: addresses 64535-65535

class RAM : public Storage{
    public:
        RAM();
};

#endif