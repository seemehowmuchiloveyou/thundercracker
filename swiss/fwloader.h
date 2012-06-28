#ifndef FW_LOADER_H
#define FW_LOADER_H

#include "iodevice.h"
#include <stdio.h>

class FwLoader
{
public:
    FwLoader(IODevice &_dev);

    static int run(int argc, char **argv, IODevice &dev);
    bool load(const char* path, int vid, int pid);

private:

    // bootloader versions that we know how to talk to
    static const unsigned VERSION_COMPAT_MIN = 1;
    static const unsigned VERSION_COMPAT_MAX = 1;

    bool bootloaderVersionIsCompatible();
    void resetWritePointer();
    bool checkFileDetails(FILE *f, uint32_t &plainsz, uint32_t &crc);
    bool sendFirmwareFile(FILE *f);
    bool sendDetails(uint32_t size, uint32_t crc);

    IODevice &dev;
};

#endif // FW_LOADER_H
