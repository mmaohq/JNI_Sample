#ifndef NZIO_BASE_H
#define NZIO_BASE_H

#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>
#include <wchar.h>
#include "nzbuffer.h"

// http://en.cppreference.com/w/c/header

// NZIO use error code from -1 to -100
#define NZIO_ERROR_PORT_INVALID     -1
#define NZIO_ERROR_WRITE_FAILED     -2
#define NZIO_ERROR_READ_FAILED      -3
#define NZIO_ERROR_TIMEOUT          -4

typedef void (*nzio_readed_event)(const unsigned char *buffer, const size_t count, void *private_data);
typedef void (*nzio_writed_event)(const unsigned char *buffer, const size_t count, void *private_data);

typedef void (*nzio_netprinter_discovered)(const char *local_ip, const char *disconvered_mac, const char *disconvered_ip, const char *discovered_name, const void *private_data);

class NZIO
{
private:
    nzio_readed_event readed_event;
    nzio_writed_event writed_event;
    void *readed_event_private_data;
    void *writed_event_private_data;

public:
    NZIO()
    {
        readed_event = NULL;
        writed_event = NULL;
        readed_event_private_data = NULL;
        writed_event_private_data = NULL;
    }

    virtual ~NZIO()
    {

    }

    virtual bool Open(const char *para)
    {
        (void)para;
        return false;
    }

    virtual bool IsOpened()
    {
        return false;
    }

    virtual void Close()
    {

    }

    // return writed bytes or negative value means error.
    virtual int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)buffer;
        (void)count;
        (void)timeout;

        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        return (int)NZIO_ERROR_WRITE_FAILED;
    }

    // return readed bytes or negative value means error.
    virtual int BaseRead(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)buffer;
        (void)count;
        (void)timeout;

        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        return (int)NZIO_ERROR_READ_FAILED;
    }

    virtual void SkipAvailable()
    {

    }

    virtual int Write(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        int nWrited = BaseWrite(buffer, count, timeout);
        if ((nWrited > 0) && (writed_event))
            writed_event(buffer, (size_t)nWrited, writed_event_private_data);
        return nWrited;
    }

    // return readed bytes or negative value means error.
    virtual int Read(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        int nReaded = BaseRead(buffer, count, timeout);
        if ((nReaded > 0) && (readed_event))
            readed_event(buffer, (size_t)nReaded, readed_event_private_data);
        return nReaded;
    }

    // return readed bytes or negative value means read error.
    virtual int ReadUntilByte(unsigned char *buffer, const size_t count, const unsigned long timeout, const unsigned char breakByte)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        size_t nTotalReaded = 0;
        unsigned long timesec = (timeout + 999)/1000;
        time_t beginTime = time(0);
        while (IsOpened()) {
            if (nTotalReaded == count)
                break;

            if (difftime(time(0), beginTime) > (double)timesec)
                break;

            int nReaded = Read(buffer, 1, timeout);
            if (nReaded < 0)
                return nReaded;
            else if (nReaded == 1) {
                nTotalReaded++;
                if (*buffer == breakByte)
                    break;
                else
                    buffer++;
            }
        }
        return (int)nTotalReaded;
    }

    // return readed bytes or negative value means read error.
    virtual int ReadUntilLength(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        size_t nTotalReaded = 0;
        unsigned long timesec = (timeout + 999)/1000;
        time_t beginTime = time(0);
        while (IsOpened()) {
            if (nTotalReaded == count)
                break;

            if (difftime(time(0), beginTime) > (double)timesec)
                break;

            int nReaded = Read(buffer + nTotalReaded, count - nTotalReaded, timeout);
            if (nReaded < 0) {
                return nReaded;
            } else if (nReaded > 0) {
                nTotalReaded += (size_t)nReaded;
            }
        }
        return (int)nTotalReaded;
    }

    // return writed bytes or negative value means read error.
    virtual int WriteUntilLength(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        size_t nTotalWrited = 0;
        unsigned long timesec = (timeout + 999)/1000;
        time_t beginTime = time(0);
        while (IsOpened()) {
            if (nTotalWrited == count)
                break;

            if (difftime(time(0), beginTime) > (double)timesec)
                break;

            int nWrited = Write(buffer + nTotalWrited, count - nTotalWrited, timeout);
            if (nWrited < 0) {
                return nWrited;
            } else if (nWrited > 0) {
                nTotalWrited += (size_t)nWrited;
            }
        }
        return (int)nTotalWrited;
    }

    // return status byte or negative value means read error.
    virtual int QueryOneByteStatus(const unsigned char *pQueryCmdData, const size_t nQueryCmdLength, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        SkipAvailable();
        unsigned long timesec = (timeout + 999)/1000;
        time_t beginTime = time(0);
        while (IsOpened()) {
            if (difftime(time(0), beginTime) > (double)timesec)
                return (int)NZIO_ERROR_TIMEOUT;

            int nWritted = Write(pQueryCmdData, nQueryCmdLength, timeout);
            if (nWritted < 0) {
                return nWritted;
            } else if ((size_t)nWritted != nQueryCmdLength) {
                return (int)NZIO_ERROR_WRITE_FAILED;
            } else {
                unsigned char status;
                int nReaded = Read(&status, 1, 1000);
                if (nReaded < 0)
                    return nReaded;
                else if (nReaded == 0)
                    continue;
                else if (nReaded != 1)
                    return (int)NZIO_ERROR_READ_FAILED;
                else
                    return (int)status;
            }
        }

        return (int)NZIO_ERROR_PORT_INVALID;
    }

    // return readed bytes or negative value means read error.
    virtual int QueryResponse(const unsigned char *pQueryCmdData, const size_t nQueryCmdLength, unsigned char *pResponseData, const size_t nResponseDataLength, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        SkipAvailable();
        int nWritted = Write(pQueryCmdData, nQueryCmdLength, timeout);
        if (nWritted < 0) {
            return nWritted;
        } else if ((size_t)nWritted != nQueryCmdLength) {
            return (int)NZIO_ERROR_WRITE_FAILED;
        }
        return ReadUntilLength(pResponseData, nResponseDataLength, timeout);
    }

    virtual void SetReadedEvent(const nzio_readed_event event, void *private_data)
    {
        readed_event = event;
        readed_event_private_data = private_data;
    }

    virtual void SetWritedEvent(const nzio_writed_event event, void *private_data)
    {
        writed_event = event;
        writed_event_private_data = private_data;
    }
};

class NZIOHolder
{
private:
    NZIO *io;

public:
    NZIOHolder()
    {
        io = NULL;
    }

    virtual ~NZIOHolder()
    {

    }

    virtual void SetIO(NZIO *newio)
    {
        io = newio;
    }

    virtual NZIO *GetIO()
    {
        return io;
    }

    virtual bool Open(const char *para)
    {
        return io && io->Open(para);
    }

    virtual bool IsOpened()
    {
        return io && io->IsOpened();
    }

    virtual void Close()
    {
        if (io)
            io->Close();
    }

    virtual void SkipAvailable()
    {
        if (io)
            io->SkipAvailable();
    }

    virtual int Write(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (io) {
            return io->Write(buffer, count, timeout);
        } else {
            return (int)NZIO_ERROR_PORT_INVALID;
		}
    }

    // return readed bytes or negative value means error.
    virtual int Read(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (io)
            return io->Read(buffer, count, timeout);
        else
            return (int)NZIO_ERROR_PORT_INVALID;
    }

    // return readed bytes or negative value means read error.
    virtual int ReadUntilByte(unsigned char *buffer, const size_t count, const unsigned long timeout, const unsigned char breakByte)
    {
        if (io)
            return io->ReadUntilByte(buffer, count, timeout, breakByte);
        else
            return (int)NZIO_ERROR_PORT_INVALID;
    }

    // return readed bytes or negative value means read error.
    virtual int ReadUntilLength(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (io)
            return io->ReadUntilLength(buffer, count, timeout);
        else
            return (int)NZIO_ERROR_PORT_INVALID;
    }

    // return writed bytes or negative value means read error.
    virtual int WriteUntilLength(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (io)
            return io->WriteUntilLength(buffer, count, timeout);
        else
            return (int)NZIO_ERROR_PORT_INVALID;
    }

    // return status byte or negative value means read error.
    virtual int QueryOneByteStatus(const unsigned char *pQueryCmdData, const size_t nQueryCmdLength, const unsigned long timeout)
    {
        if (io)
            return io->QueryOneByteStatus(pQueryCmdData, nQueryCmdLength, timeout);
        else
            return (int)NZIO_ERROR_PORT_INVALID;
    }

    // return readed bytes or negative value means read error.
    virtual int QueryResponse(const unsigned char *pQueryCmdData, const size_t nQueryCmdLength, unsigned char *pResponseData, const size_t nResponseDataLength, const unsigned long timeout)
    {
        if (io)
            return io->QueryResponse(pQueryCmdData, nQueryCmdLength, pResponseData, nResponseDataLength, timeout);
        else
            return (int)NZIO_ERROR_PORT_INVALID;
    }

    // io can not be null
    virtual void SetReadedEvent(const nzio_readed_event event, void *private_data)
    {
        if (io)
            io->SetReadedEvent(event, private_data);
    }

    // io can not be null
    virtual void SetWritedEvent(const nzio_writed_event event, void *private_data)
    {
        if (io)
            io->SetWritedEvent(event, private_data);
    }
};

class NZMemoryIO : virtual public NZIO
{
private:
    size_t buffer_size;
    unsigned char *buffer_data;
    size_t buffer_offset;

public:
    NZMemoryIO()
    {
        buffer_size = 0;
        buffer_data = NULL;
        buffer_offset = 0;
    }

    virtual ~NZMemoryIO()
    {

    }

    virtual bool Open(size_t nBufferSize)
    {
        if (nBufferSize) {
            buffer_data = (unsigned char *)malloc(nBufferSize);
            if (buffer_data) {
                buffer_size = nBufferSize;
                buffer_offset = 0;
            }
        }
        return (buffer_data != NULL);
    }

    virtual bool Open(const char *para)
    {
        int nBufferSize = atoi(para);
        if (nBufferSize <= 0)
            return false;
        return Open((size_t)nBufferSize);
    }

    virtual bool IsOpened()
    {
        return (buffer_data != NULL);
    }

    virtual void Close()
    {
        if (buffer_data) {
            free(buffer_data);
            buffer_data = NULL;
            buffer_size = 0;
            buffer_offset = 0;
        }
    }

    virtual int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)timeout;

        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        if (buffer_offset + count > buffer_size)
            return (int)NZIO_ERROR_WRITE_FAILED;

        memcpy(buffer_data, buffer, count);
        buffer_offset += count;
        return (int)count;
    }

    virtual unsigned char *GetBufferData()
    {
        return buffer_data;
    }

    virtual size_t GetBufferLength()
    {
        return buffer_offset;
    }

    virtual void ClearBufferData()
    {
        buffer_offset = 0;
    }
};

class NZFileIO : virtual public NZIO
{
private:
    FILE *fp;

public:
    NZFileIO()
    {
        fp = NULL;
    }

    virtual ~NZFileIO()
    {

    }

    virtual bool IsOpened()
    {
        return fp != NULL;
    }

    virtual void Close()
    {
        if (fp != NULL) {
            fclose(fp);
            fp = NULL;
        }
    }

    virtual int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)timeout;

        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        return (int)fwrite(buffer, 1, count, fp);
    }

    virtual bool Open(const char *para)
    {
        return OpenNew(para);
    }

    virtual bool OpenNew(const char *name)
    {
        fp = fopen(name, "wb+");
        if (fp)
            fseek(fp, 0, SEEK_SET);
        return fp != NULL;
    }

    virtual bool OpenAppend(const char *name)
    {
        fp = fopen(name, "ab+");
        if (fp)
            fseek(fp, 0, SEEK_END);
        return fp != NULL;
    }
};

class NZBufferedIO : virtual public NZIO
{
private:
    NZIORxBuffer *rx;

    size_t nReadPacketSize;
    const static size_t nMinReadPacketSize = 1;
    const static size_t nMaxReadPacketSize = 1024;

public:
    NZBufferedIO()
    {
        rx = new NZIORxBuffer(0x1000);
        SetReadPacketSize(64);
    }

    NZBufferedIO(size_t rxBufferSize, size_t rxPacketSize)
    {
        rx = new NZIORxBuffer(rxBufferSize);
        SetReadPacketSize(rxPacketSize);
    }

    ~NZBufferedIO()
    {
        delete rx;
    }

    void SkipAvailable()
    {
        rx->Clear();
    }

    void SetReadPacketSize(const size_t nPacketSize)
    {
        if (nPacketSize < nMinReadPacketSize)
            nReadPacketSize = nMinReadPacketSize;
        else if (nPacketSize > nMaxReadPacketSize)
            nReadPacketSize = nMaxReadPacketSize;
        else
            nReadPacketSize = nPacketSize;
    }

    virtual int BaseReadAvailable(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)buffer;
        (void)count;
        (void)timeout;

        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        return (int)NZIO_ERROR_READ_FAILED;
    }

    int BaseRead(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZIO_ERROR_PORT_INVALID;

        size_t nBytesReaded = 0;
        unsigned char tmpbuf[nMaxReadPacketSize];

        unsigned long timesec = (timeout + 999)/1000;
        time_t beginTime = time(0);
        while (IsOpened()) {
            if (difftime(time(0), beginTime) > (double)timesec)
                break;
            if (nBytesReaded == count)
                break;

            while ((!rx->IsEmpty()) && (nBytesReaded != count))
                buffer[nBytesReaded++] = rx->PopFront();

            if (nBytesReaded != count) {
                int readed = BaseReadAvailable(tmpbuf, nReadPacketSize, timeout);
                if (readed > 0) {
                    for (int i = 0; i < readed; ++i)
                        rx->PushBack(tmpbuf[i]);
                    while ((!rx->IsEmpty()) && (nBytesReaded != count))
                        buffer[nBytesReaded++] = rx->PopFront();
                } else if (readed < 0) {
                    return (int)NZIO_ERROR_READ_FAILED;
                }
            }
        }

        return (int)nBytesReaded;
    }
};

enum ComDataBits { ComDataBits_4 = 4, ComDataBits_5 = 5, ComDataBits_6 = 6, ComDataBits_7 = 7, ComDataBits_8 = 8 };
enum ComParity { ComParity_NoParity, ComParity_OddParity, ComParity_EvenParity, ComParity_MarkParity, ComParity_SpaceParity };
enum ComStopBits { ComStopBits_One, ComStopBits_OnePointFive, ComStopBits_Two };
enum ComFlowControl { ComFlowControl_None, ComFlowControl_XonXoff, ComFlowControl_RtsCts, ComFlowControl_DtrDsr };
class NZComIO : virtual public NZIO
{
public:
    virtual bool Open(const char *name, const unsigned int baudrate, const ComDataBits databits, const ComParity parity, const ComStopBits stopbits, const ComFlowControl flowcontrol)
    {
        (void)name;
        (void)baudrate;
        (void)databits;
        (void)parity;
        (void)stopbits;
        (void)flowcontrol;
        return false;
    }

    virtual bool SetBaudrate(const unsigned int baudrate)
    {
        (void)baudrate;
        return false;
    }

    virtual bool SetFlowControl(const ComFlowControl flowcontrol)
    {
        (void)flowcontrol;
        return false;
    }
};

class NZUsbIO : virtual public NZIO
{

};

class NZLptIO : virtual public NZIO
{

};

class NZPrnIO : virtual public NZIO
{

};

class NZSocketIO : virtual public NZIO
{
public:
    virtual ~NZSocketIO()
    {

    }

    virtual bool bind_addr(const char *local_ip, const unsigned short local_port)
    {
        (void)local_ip;
        (void)local_port;
        return false;
    }

    virtual bool setsockopt_linger(const bool on, const unsigned long timeout)
    {
        (void)on;
        (void)timeout;
        return false;
    }

    virtual bool setsockopt_reuseaddr(const bool on)
    {
        (void)on;
        return false;
    }

    virtual bool setsockopt_sndbuf(const size_t size)
    {
        (void)size;
        return false;
    }

    virtual bool setsockopt_rcvbuf(const size_t size)
    {
        (void)size;
        return false;
    }

    virtual bool setsockopt_rcvtimeo(const unsigned long timeout)
    {
        (void)timeout;
        return false;
    }

    virtual bool setsockopt_sndtimeo(const unsigned long timeout)
    {
        (void)timeout;
        return false;
    }
};

class NZTcpClientIO : virtual public NZSocketIO
{
public:
    virtual bool Open(const char *local_ip, const char *dest_ip, const unsigned short dest_port, const unsigned long timeout)
    {
        (void)local_ip;
        (void)dest_ip;
        (void)dest_port;
        (void)timeout;
        return false;
    }
    virtual bool Open(const char *para)
    {
        return Open(0, para, 9100, 5000);
    }

    virtual int socket_send(const unsigned char *buffer, const size_t count)
    {
        (void)buffer;
        (void)count;
        return -1;
    }
    virtual int socket_recv(unsigned char *buffer, const size_t count)
    {
        (void)buffer;
        (void)count;
        return -1;
    }

    virtual int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!setsockopt_sndtimeo(timeout))
            return -1;
        return socket_send(buffer, count);
    }

    virtual int BaseRead(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!setsockopt_rcvtimeo(timeout))
            return -1;
        return socket_recv(buffer, count);
    }

    virtual bool setsockopt_keepalive(const bool on)
    {
        (void)on;
        return false;
    }
    virtual bool setsockopt_keepalivevals(const bool onoff, const unsigned int keepalivetime, const unsigned int keepaliveinterval, const unsigned int keepalivecount)
    {
        (void)onoff;
        (void)keepalivetime;
        (void)keepaliveinterval;
        (void)keepalivecount;
        return false;
    }
};

class NZUdpClientIO : virtual public NZSocketIO
{
protected:
    unsigned long to_ip;
    unsigned short to_port;

public:
    NZUdpClientIO()
    {
        to_ip = 0;
        to_port = 0;
    }

    ~NZUdpClientIO()
    {

    }

public:
    virtual bool Open(const char *local_ip, const unsigned short local_port, const char *dest_ip, const unsigned short dest_port)
    {
        (void)local_ip;
        (void)local_port;
        (void)dest_ip;
        (void)dest_port;
        return false;
    }
    virtual bool Open(const char *para)
    {
        return Open(0, 0, para, 5100);
    }

    virtual int socket_sendto(const unsigned char *buffer, const size_t count, const unsigned long to_ip, const unsigned short to_port)
    {
        (void)buffer;
        (void)count;
        (void)to_ip;
        (void)to_port;
        return -1;
    }
    virtual int socket_recvfrom(unsigned char *buffer, const size_t count, unsigned long *from_ip, unsigned short *from_port)
    {
        (void)buffer;
        (void)count;
        (void)from_ip;
        (void)from_port;
        return -1;
    }

    virtual int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!setsockopt_sndtimeo(timeout))
            return -1;
        return socket_sendto(buffer, count, to_ip, to_port);
    }

    virtual int BaseRead(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        if (!setsockopt_rcvtimeo(timeout))
            return -1;
        return socket_recvfrom(buffer, count, 0, 0);
    }

    virtual bool setsockopt_broadcast(const bool on)
    {
        (void)on;
        return false;
    }
};

class NZWebSocketClientIO : virtual public NZIO
{
public:
    virtual bool Open(const char *local_ip, const char *dest_ip, const unsigned short dest_port, const unsigned long timeout)
    {
        (void)local_ip;
        (void)dest_ip;
        (void)dest_port;
        (void)timeout;
        return false;
    }
};

class NZEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        (void)buffer;
        (void)length;

        size_t count = 0;
        size_t required = 0;

        if (prequired)
            *prequired = required;
        return count;
    }
};

class NZComIOEnumerator : public NZEnumerator
{

};

class NZUsbIOEnumerator : public NZEnumerator
{

};

class NZLptIOEnumerator : public NZEnumerator
{

};

class NZPrnIOEnumerator : public NZEnumerator
{

};

class NZHostIPEnumerator : public NZEnumerator
{

};

class NZNetPrinterEnumerator : public NZEnumerator
{
public:
    NZHostIPEnumerator *nzHostIPEnumerator;
    NZUdpClientIO *udp;

public:
    NZNetPrinterEnumerator()
    {
        nzHostIPEnumerator = NULL;
        udp = NULL;
    }

    virtual ~NZNetPrinterEnumerator()
    {

    }

public:
    void DiscoverIpName(
            const char *local_ip, const unsigned short local_port,
            const char *dest_ip, const unsigned short dest_port,
            const unsigned long timeout,
            const nzio_netprinter_discovered on_discovered, const void *private_data)
    {
        if (udp == NULL)
            return;

        if (udp->Open(local_ip, local_port, dest_ip, dest_port)) {
            udp->setsockopt_reuseaddr(true);
            udp->setsockopt_broadcast(true);
            udp->setsockopt_sndtimeo(1000);
            udp->setsockopt_rcvtimeo(1000);

            unsigned char cmd[] = {
                0x45, 0x50, 0x53, 0x4f, 0x4e, 0x51,
                0x03, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00 };

            unsigned long timesec = (timeout + 999)/1000;
            time_t beginTime = time(0);
            while (udp->IsOpened()) {
                if (difftime(time(0), beginTime) > (double)timesec)
                    break;
                if (udp->Write(cmd, sizeof(cmd), 1000) != sizeof(cmd))
                    break;

                while (udp->IsOpened()) {
                    if (difftime(time(0), beginTime) > timesec)
                        break;

                    unsigned long from_ip = 0;
                    unsigned short from_port = 0;
                    unsigned char recbuf[200] = { 0, };
                    int nBytesReaded = udp->socket_recvfrom(recbuf, sizeof(recbuf), &from_ip, &from_port);
                    if (nBytesReaded <= 0) {
                        break;
                    } else if (nBytesReaded == 184) {
                        const char *EPSONq = "EPSONq";
                        if (memcmp(recbuf, EPSONq, strlen(EPSONq)) == 0) {
                            char discovered_mac[300] = { 0, };
                            char discovered_ip[300] = { 0, };
                            char discovered_name[300] = { 0, };

                            sprintf(discovered_mac, "%02X-%02X-%02X-%02X-%02X-%02X",
                                    recbuf[14] & 0xFF, recbuf[15] & 0xFF,
                                    recbuf[16] & 0xFF, recbuf[17] & 0xFF,
                                    recbuf[18] & 0xFF, recbuf[19] & 0xFF);

                            sprintf(discovered_ip, "%d.%d.%d.%d",
                                    (int)(from_ip & 0xFF), (int)((from_ip >> 8) & 0xFF),
                                    (int)((from_ip >> 16) & 0xFF), (int)((from_ip >> 24) & 0xFF));

                            if (strlen((char *)&recbuf[56])) {
                                sprintf(discovered_name, "%s", &recbuf[56]);
                            } else {
                                sprintf(discovered_name, "%d.%d.%d.%d",
                                        (int)(from_ip & 0xFF), (int)((from_ip >> 8) & 0xFF),
                                        (int)((from_ip >> 16) & 0xFF), (int)((from_ip >> 24) & 0xFF));
                            }

                            if (on_discovered) {
                                on_discovered(local_ip, discovered_mac, discovered_ip, discovered_name, private_data);
                            }
                        }
                    }
                }
            }

            udp->Close();
        }
    }

    void EnumNetPrinter(const unsigned long timeout, const nzio_netprinter_discovered on_discovered, const void *private_data)
    {
        if (nzHostIPEnumerator == NULL)
            return;

        char ips[100] = { 0, };
        if (nzHostIPEnumerator->Enumerate(ips, sizeof(ips), NULL)) {
            char *pdev = ips;
            while (strlen(pdev)) {
                DiscoverIpName(pdev, 0, "255.255.255.255", 3289, timeout, on_discovered, private_data);
                pdev += strlen(pdev) + 1;
            }
        }
    }
};

#endif // NZIO_BASE_H
