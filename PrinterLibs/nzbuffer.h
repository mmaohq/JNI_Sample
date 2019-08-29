#ifndef NZBUFFER_H
#define NZBUFFER_H

#include <stdlib.h>

class NZIORxBuffer {
private:
    size_t size;
    unsigned char *buffer;
    size_t readpos;
    size_t writepos;

public:
    NZIORxBuffer()
    {
        size = 0x1000;
        buffer = (unsigned char *)malloc(size);
        readpos = 0;
        writepos = 0;
    }
    NZIORxBuffer(size_t buffer_size)
    {
        size = buffer_size;
        buffer = (unsigned char *)malloc(size);
        readpos = 0;
        writepos = 0;
    }

    ~NZIORxBuffer()
    {
        free(buffer);
    }

    void PushBack(unsigned char data)
    {
        while (((writepos + 1) % size) == (readpos % size));

        buffer[writepos] = data;
        writepos = ((writepos + 1) % size);
    }
    unsigned char PopFront()
    {
        while ((writepos % size) == (readpos % size));

        unsigned char data = buffer[readpos];
        readpos = ((readpos + 1) % size);

        return data;
    }
    unsigned char Front()
    {
        while ((writepos % size) == (readpos % size));

        return buffer[readpos];
    }
    bool IsEmpty()
    {
        return (writepos % size) == (readpos % size);
    }
    bool IsFull()
    {
        return ((writepos + 1) % size) == (readpos % size);
    }
    void Clear()
    {
        while (!IsEmpty())
            PopFront();
    }
};

#endif // NZBUFFER_H
