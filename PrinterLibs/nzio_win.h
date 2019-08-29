#ifndef CPP_OS_WIN_NZIO_WIN_H
#define CPP_OS_WIN_NZIO_WIN_H

#include <WinSock2.h>
#include <Windows.h>
#include <WinSpool.h>
#include <mstcpip.h>
#include <setupapi.h>
#include <initguid.h>
#include <devguid.h>
#include <MMSystem.h>
#include <malloc.h>
#include <stdlib.h>
#include <stdio.h>
#include <iphlpapi.h>
#include "nzio_base.h"

#pragma comment(lib,"Winmm.lib")
#pragma comment(lib,"Winspool.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"IPHLPAPI.lib")
#pragma comment(lib,"setupapi.lib")
#pragma comment(lib,"ws2_32.lib")
#pragma warning(disable:4250)

class AsyncFileIOBase_win : virtual public NZBufferedIO
{
protected:
    HANDLE h;

public:
    AsyncFileIOBase_win()
    {
        h = INVALID_HANDLE_VALUE;
    }

    virtual ~AsyncFileIOBase_win()
    {

    }

    virtual bool IsOpened()
    {
        return h != INVALID_HANDLE_VALUE;
    }

    virtual void Close()
    {
        if (h != INVALID_HANDLE_VALUE) {
            CloseHandle(h);
            h = INVALID_HANDLE_VALUE;
        }
    }

    // CancelIo
    // Cancels all pending input and output (I/O) operations that are issued by the calling thread for the specified file.
    // The function does not cancel I/O operations that other threads issue for a file handle.
    int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        char msg[100] = { 0, };

        DWORD dwBytesWritten = 0;
        OVERLAPPED ov;
        ZeroMemory(&ov, sizeof(OVERLAPPED));
        ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (ov.hEvent) {
            if (WriteFile(h, buffer, (DWORD)count, &dwBytesWritten, &ov)) {
                //return dwBytesWritten;
            } else {
                DWORD dwError = 0;
                dwError = GetLastError();
                if (dwError == ERROR_IO_PENDING) {
                    DWORD start_ms = timeGetTime();
                    while (true) {
                        if (!IsOpened()) {
                            sprintf(&msg[strlen(msg)], "Write exit on closed\r\n");
                            OutputDebugStringA(msg);
                            //return -1;
                            dwBytesWritten = -1;
                            break;
                        }
                        if (timeGetTime() - start_ms > timeout) {
                            CancelIo(h);
                            sprintf(&msg[strlen(msg)], "Write canceled on timeout %d. Writted bytes %d.\r\n", timeout, dwBytesWritten);
                            OutputDebugStringA(msg);
                            //return dwBytesWritten;
                            break;
                        }
                        if (GetOverlappedResult(h, &ov, &dwBytesWritten, FALSE)) {
                            //return dwBytesWritten;
                            break;
                        } else {
                            dwError = GetLastError();
                            if (dwError == ERROR_IO_INCOMPLETE) {
                                Sleep(1);
                                continue;
                            } else {
                                sprintf(&msg[strlen(msg)], "Write failed on error %d\r\n", dwError);
                                OutputDebugStringA(msg);
                                //return -1;
                                dwBytesWritten = -1;
                                break;
                            }
                        }
                    }
                } else {
                    sprintf(&msg[strlen(msg)], "Write failed on error %d\r\n", dwError);
                    OutputDebugStringA(msg);
                    //return -1;
                    CancelIo(h);
                    dwBytesWritten = -1;
                }
            }
            CloseHandle(ov.hEvent);
        } else {
            OutputDebugStringA("CreateEvent failed");
        }
        return (dwBytesWritten == (DWORD)-1) ? -1 : (int)dwBytesWritten;
    }

    int BaseReadAvailable(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        char msg[100] = { 0, };

        DWORD dwBytesReaded = 0;
        OVERLAPPED ov = {0};
        ov.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
        if (ov.hEvent) {
            if (ReadFile(h, buffer, (DWORD)count, &dwBytesReaded, &ov)) {
                //return dwBytesReaded;
            } else {
                DWORD dwError = 0;
                dwError = GetLastError();
                if (dwError == ERROR_IO_PENDING) {
                    DWORD start_ms = timeGetTime();
                    while (true) {
                        if (!IsOpened()) {
                            sprintf(&msg[strlen(msg)], "Read exit on closed\r\n");
                            OutputDebugStringA(msg);
                            //return -1;
                            dwBytesReaded = -1;
                            break;
                        }
                        if (timeGetTime() - start_ms > timeout) {
                            CancelIo(h);
                            sprintf(&msg[strlen(msg)], "Read canceled on timeout %d. Readed bytes %d.\r\n", timeout, dwBytesReaded);
                            OutputDebugStringA(msg);
                            //return dwBytesReaded;
                            break;
                        }
                        if (GetOverlappedResult(h, &ov, &dwBytesReaded, FALSE)) {
                            //return dwBytesReaded;
                            break;
                        } else {
                            dwError = GetLastError();
                            if (dwError == ERROR_IO_INCOMPLETE) {
                                Sleep(1);
                                continue;
                            } else {
                                sprintf(&msg[strlen(msg)], "Read failed on error %d\r\n", dwError);
                                OutputDebugStringA(msg);
                                //return -1;
                                dwBytesReaded = -1;
                                break;
                            }
                        }
                    }
                } else {
                    sprintf(&msg[strlen(msg)], "Read failed on error %d\r\n", dwError);
                    OutputDebugStringA(msg);
                    //return -1;
                    CancelIo(h);
                    dwBytesReaded = -1;
                }
            }
            CloseHandle(ov.hEvent);
        } else {
            OutputDebugStringA("CreateEvent failed");
        }
        return (dwBytesReaded == (DWORD)-1) ? -1 : (int)dwBytesReaded;
    }

};

class NZUsbIO_win : public AsyncFileIOBase_win, public NZUsbIO
{
public:
    bool Open(const char *name)
    {
        h = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

        if (IsOpened()) {
            SetReadPacketSize(64);
        }

        return IsOpened();
    }
};

class NZLptIO_win : public AsyncFileIOBase_win, public NZLptIO
{
public:
    bool Open(const char *name)
    {
        h = CreateFileA(name, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        return IsOpened();
    }
};

class NZComIO_win : public AsyncFileIOBase_win, public NZComIO
{
public:
    bool Open(const char *name, const unsigned int baudrate, const ComDataBits databits, const ComParity parity, const ComStopBits stopbits, const ComFlowControl flowcontrol)
    {
        char pFileName[200] = { 0, };
        if (strstr(name, "\\\\.\\") != name)
            sprintf(pFileName, "\\\\.\\%s", name);
        else
            sprintf(pFileName, "%s", name);
        h = CreateFileA(pFileName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);
        if (IsOpened()) {
            if (IsOpened()) {
                DWORD dwErrors = 0;
                COMSTAT comstat;
                if (!ClearCommError(h, &dwErrors, &comstat))
                    Close();
            }

            if (IsOpened()) {
                if (!SetupComm(h, 4096, 4096))
                    Close();
            }

            if (IsOpened()) {
                COMMTIMEOUTS timeouts;
                ZeroMemory(&timeouts, sizeof(COMMTIMEOUTS));
                timeouts.ReadIntervalTimeout = 100;
                timeouts.ReadTotalTimeoutMultiplier = 1000 / ((baudrate/10) + 1) + 1;
                timeouts.ReadTotalTimeoutConstant = 2000;
                timeouts.WriteTotalTimeoutMultiplier = 1000 / ((baudrate/10) + 1) + 1;
                timeouts.WriteTotalTimeoutConstant = 2000;
                if (!SetCommTimeouts(h, &timeouts))
                    Close();
            }

            if (IsOpened()) {
                DCB dcb;
                ZeroMemory(&dcb, sizeof(DCB));
                dcb.DCBlength = sizeof(DCB);
                dcb.fBinary = TRUE;
                dcb.fParity = TRUE;
                dcb.fAbortOnError = TRUE;
                dcb.BaudRate = baudrate;
                dcb.ByteSize = 8;
                dcb.Parity = NOPARITY;
                dcb.StopBits = ONESTOPBIT;
                switch (databits) {
                case ComDataBits_4:
                    dcb.ByteSize = 4;
                    break;
                case ComDataBits_5:
                    dcb.ByteSize = 5;
                    break;
                case ComDataBits_6:
                    dcb.ByteSize = 6;
                    break;
                case ComDataBits_7:
                    dcb.ByteSize = 7;
                    break;
                case ComDataBits_8:
                    dcb.ByteSize = 8;
                    break;
                default:
                    break;
                }
                switch (parity) {
                case ComParity_NoParity:
                    dcb.Parity = NOPARITY;
                    break;
                case ComParity_OddParity:
                    dcb.Parity = ODDPARITY;
                    break;
                case ComParity_EvenParity:
                    dcb.Parity = EVENPARITY;
                    break;
                case ComParity_MarkParity:
                    dcb.Parity = MARKPARITY;
                    break;
                case ComParity_SpaceParity:
                    dcb.Parity = SPACEPARITY;
                    break;
                default:
                    break;
                }
                switch (stopbits) {
                case ComStopBits_One:
                    dcb.StopBits = ONESTOPBIT;
                    break;
                case ComStopBits_OnePointFive:
                    dcb.StopBits = ONE5STOPBITS;
                    break;
                case ComStopBits_Two:
                    dcb.StopBits = TWOSTOPBITS;
                    break;
                default:
                    break;
                }
                switch (flowcontrol) {
                case ComFlowControl_DtrDsr:
                    dcb.fOutxDsrFlow = TRUE;
                    break;

                case ComFlowControl_RtsCts:
                    dcb.fOutxCtsFlow = TRUE;
                    break;

                case ComFlowControl_XonXoff:
                    dcb.fOutX = TRUE;
                    dcb.XonChar = 0x11;
                    dcb.XoffChar = 0x13;
                    break;

                case ComFlowControl_None:
                default:
                    break;
                }
                if (!SetCommState(h, &dcb)) {
                    Close();
                }
            }

            if (IsOpened()) {
                if (!PurgeComm(h, PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR))
                    Close();
            }

            if (IsOpened()) {
                if (!SetCommMask(h, EV_BREAK | EV_CTS | EV_DSR | EV_ERR | EV_RING | EV_RLSD | EV_RXCHAR | EV_RXFLAG | EV_TXEMPTY))
                    Close();
            }
        }

        return IsOpened();
    }

    bool SetBaudrate(const unsigned int baudrate)
    {
        DCB dcb;
        ZeroMemory(&dcb, sizeof(DCB));

        if (GetCommState(h, &dcb)) {
            dcb.BaudRate = baudrate;
            if (SetCommState(h, &dcb))
                return true;
        }
        return false;
    }
};

class NZPrnIO_win : public NZPrnIO
{
protected:
    HANDLE h;

public:
    NZPrnIO_win()
    {
        h = INVALID_HANDLE_VALUE;
    }

    virtual ~NZPrnIO_win()
    {

    }

    virtual bool IsOpened()
    {
        return h != INVALID_HANDLE_VALUE;
    }

    bool Open(const char *name)
    {
        if (OpenPrinterA((char *)name, &h, NULL)) {
            const char *doc_name = "PrinterIO";
            const char *data_type = "RAW";
            if (Start_Doc((char *)doc_name, (char *)data_type)) {
                return true;
            }
            ClosePrinter(h);
            h = INVALID_HANDLE_VALUE;
        }
        return false;
    }

    virtual void Close()
    {
        if (h != INVALID_HANDLE_VALUE) {
            End_Doc();
            ClosePrinter(h);
            h = INVALID_HANDLE_VALUE;
        }
    }

    int Start_Doc(char *doc_name, char *data_type)
    {
        DOC_INFO_1A doc_info;
        doc_info.pDocName = doc_name;
        doc_info.pOutputFile = NULL;
        doc_info.pDatatype = data_type;
        return StartDocPrinterA(h, 1, (LPBYTE)&doc_info);
    }

    bool Start_Page()
    {
        return StartPagePrinter(h) ? true : false;
    }

    bool End_Page()
    {
        return EndPagePrinter(h) ? true : false;
    }

    bool End_Doc()
    {
        return EndDocPrinter(h) ? true : false;
    }

    int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)timeout;
        DWORD dwBytesWritten = 0;

        if (WritePrinter(h, (LPVOID)buffer, (DWORD)count, &dwBytesWritten))
            return (int)dwBytesWritten;

        return -1;
    }

    int BaseRead(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        (void)timeout;
        DWORD dwBytesReaded = 0;

        if (ReadPrinter(h, (LPVOID)buffer, (DWORD)count, &dwBytesReaded))
            return (int)dwBytesReaded;

        return -1;
    }
};

class NZSocketIO_win : virtual public NZSocketIO
{
protected:
    SOCKET s;

public:
    NZSocketIO_win()
    {
        s = INVALID_SOCKET;
        WSADATA wsadata;
        int ret = WSAStartup(MAKEWORD(2,2), &wsadata);
        if (ret != 0)
            fprintf(stderr, "Unable to load Winsock\n");
    }

    ~NZSocketIO_win()
    {
        WSACleanup();
    }

    virtual bool IsOpened()
    {
        return s != INVALID_SOCKET;
    }

    virtual void Close()
    {
        if (s != INVALID_SOCKET) {
            closesocket(s);
            s = INVALID_SOCKET;
        }
    }

    virtual bool bind_addr(const char *local_ip, const unsigned short local_port)
    {
        sockaddr_in local_addr;
        local_addr.sin_family = AF_INET;
        local_addr.sin_port = htons(local_port);
        local_addr.sin_addr.s_addr = inet_addr(local_ip);
        return bind(s, (sockaddr *)&local_addr, sizeof(local_addr)) == 0;
    }

    virtual bool setsockopt_linger(const bool on, const unsigned long timeout)
    {
        struct linger sl;
        sl.l_onoff = on ? 1 : 0;
        sl.l_linger = (unsigned short)timeout;
        return setsockopt(s, SOL_SOCKET, SO_LINGER, (char*)&sl, sizeof(sl)) == 0;
    }

    virtual bool setsockopt_reuseaddr(const bool on)
    {
        BOOL bReuseaddr = on ? TRUE : FALSE;
        return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuseaddr, sizeof(bReuseaddr)) == 0;
    }

    virtual bool setsockopt_sndbuf(const size_t size)
    {
        int nSize = (int)size;
        return setsockopt(s, SOL_SOCKET, SO_SNDBUF, (char*)&nSize, sizeof(nSize)) == 0;
    }

    virtual bool setsockopt_rcvbuf(const size_t size)
    {
        int nSize = (int)size;
        return setsockopt(s, SOL_SOCKET, SO_RCVBUF, (char*)&nSize, sizeof(nSize)) == 0;
    }

    virtual bool setsockopt_rcvtimeo(const unsigned long timeout)
    {
        DWORD dwTimeout = timeout;
        return setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, (char*)&dwTimeout, sizeof(DWORD)) == 0;
    }

    virtual bool setsockopt_sndtimeo(const unsigned long timeout)
    {
        DWORD dwTimeout = timeout;
        return setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, (char*)&dwTimeout, sizeof(DWORD)) == 0;
    }
};

class NZTcpClientIO_win : public NZSocketIO_win, public NZTcpClientIO
{
public:
    bool Open(const char *local_ip, const char *dest_ip, const unsigned short dest_port, const unsigned long timeout)
    {
        s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (IsOpened()) {
            if (IsOpened()) {
                if (local_ip && strlen(local_ip)) {
                    if (!bind_addr(local_ip, 0))
                        Close();
                }
            }

            if (IsOpened()) {
                u_long ulNonBlockIO = 1;
                if (ioctlsocket(s, FIONBIO, &ulNonBlockIO) != 0)
                    Close();
            }

            if (IsOpened()) {
                sockaddr_in server_addr;
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = inet_addr(dest_ip);
                server_addr.sin_port = htons(dest_port);
                int ret = connect(s, (SOCKADDR*)&server_addr, sizeof(server_addr));
                if (ret != 0) {
                    // With a nonblocking socket, the connection attempt cannot be completed immediately.
                    // In this case, connect will return SOCKET_ERROR, and WSAGetLastError will return WSAEWOULDBLOCK.
                    if ((ret == SOCKET_ERROR) && (WSAGetLastError() == WSAEWOULDBLOCK)) {
                        //Use the select function to determine the completion of the connection request by checking to see if the socket is writeable.
                        struct timeval tv;
                        tv.tv_sec = timeout / 1000;
                        tv.tv_usec = (timeout % 1000) * 1000;
                        fd_set w;
                        FD_ZERO(&w);
                        FD_SET(s, &w);
                        if (select(s + 1, 0, &w, 0, &tv) <= 0)
                            Close();
                    } else {
                        Close();
                    }
                }
            }

            if (IsOpened()) {
                u_long ulNonBlockIO = 0;
                if (ioctlsocket(s, FIONBIO, &ulNonBlockIO) != 0)
                    Close();
            }
        }

        return IsOpened();
    }

    virtual int socket_send(const unsigned char *buffer, const size_t count)
    {
        int ret = send(s, (char *)buffer, (int)count, 0);
        if (ret != SOCKET_ERROR) {
            return (int)ret;
        } else {
            int err = WSAGetLastError();
            //printf("send failed: %d\n", err);
            switch (err) {
            case WSAECONNRESET:
                return -1;
            case WSAETIMEDOUT:
                return 0;
            default:
                return -1;
            }
        }
    }

    virtual int socket_recv(unsigned char *buffer, const size_t count)
    {
        int ret = recv(s, (char *)buffer, (int)count, 0);
        if (ret != SOCKET_ERROR) {
            return (int)ret;
        } else {
            int err = WSAGetLastError();
            //printf("send failed: %d\n", err);
            switch (err) {
            case WSAECONNRESET:
                return -1;
            case WSAETIMEDOUT:
                return 0;
            default:
                return -1;
            }
        }
    }

    bool setsockopt_keepalive(const bool on)
    {
        BOOL bKeepAlive = on ? TRUE : FALSE;
        return setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, (char*)&bKeepAlive, sizeof(bKeepAlive)) == 0;
    }

    bool setsockopt_keepalivevals(const bool onoff, const unsigned int keepalivetime, const unsigned int keepaliveinterval, const unsigned int keepalivecount)
    {
        (void)keepalivecount;

        tcp_keepalive alive_in;
        alive_in.onoff = onoff;
        alive_in.keepalivetime = keepalivetime;
        alive_in.keepaliveinterval = keepaliveinterval;

        tcp_keepalive alive_out;
        memset(&alive_out, 0, sizeof(alive_out));
        ULONG ulBytesReturn = 0;

        return WSAIoctl(s, SIO_KEEPALIVE_VALS, &alive_in, sizeof(alive_in), &alive_out, sizeof(alive_out), &ulBytesReturn, NULL, NULL) == 0;
    }
};

class NZUdpClientIO_win : public NZSocketIO_win, public NZUdpClientIO
{
public:
    bool Open(const char *local_ip, const unsigned short local_port, const char *dest_ip, const unsigned short dest_port)
    {
        s = socket(AF_INET, SOCK_DGRAM, 0);
        if (IsOpened()) {
            if (IsOpened()) {
                if ((local_ip && strlen(local_ip)) || (local_port)) {
                    if (!bind_addr(local_ip, local_port))
                        Close();
                }
            }

            if (IsOpened()) {
                to_ip = inet_addr(dest_ip);
                to_port = dest_port;
            }
        }
        return IsOpened();
    }

    virtual int socket_sendto(const unsigned char *buffer, const size_t count, const unsigned long to_ip, const unsigned short to_port)
    {
        sockaddr_in to_addr;
        memset(&to_addr, 0, sizeof(to_addr));
        to_addr.sin_family = AF_INET;
        to_addr.sin_port = htons(to_port);
        to_addr.sin_addr.S_un.S_addr = to_ip;

        int ret = sendto(s, (char *)buffer, (int)count, 0, (sockaddr *)&to_addr, sizeof(to_addr));
        if (ret != SOCKET_ERROR) {
            return ret;
        } else {
            int err = WSAGetLastError();
            switch (err) {
            case WSAECONNRESET:
                return -1;
            case WSAETIMEDOUT:
                return 0;
            default:
                return -1;
            }
        }
    }

    virtual int socket_recvfrom(unsigned char *buffer, const size_t count, unsigned long *from_ip, unsigned short *from_port)
    {
        sockaddr_in from_addr;
        memset(&from_addr, 0, sizeof(from_addr));
        int from_addr_size = sizeof(from_addr);
        int ret = recvfrom(s, (char *)buffer, (int)count, 0, (sockaddr *)&from_addr, &from_addr_size);
        if (ret != SOCKET_ERROR) {
            if (from_ip) {
                *from_ip = from_addr.sin_addr.s_addr;
            }
            if (from_port)
                *from_port = from_addr.sin_port;
            return ret;
        } else {
            int err = WSAGetLastError();
            //printf("recv failed: %d\n", err);
            switch (err) {
            case WSAECONNRESET:
                return -1;
            case WSAETIMEDOUT:
                return 0;
            default:
                return -1;
            }
        }
    }

    virtual bool setsockopt_broadcast(const bool on)
    {
        BOOL bBroadcast = on ? TRUE : FALSE;
        return setsockopt(s, SOL_SOCKET, SO_BROADCAST, (char*)&bBroadcast, sizeof(bBroadcast)) == 0;
    }
};

class NZUsbIOEnumerator_win : public NZUsbIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        HDEVINFO h_dev_info;
        SP_DEVINFO_DATA dev_info_data;
        SP_DEVICE_INTERFACE_DATA dev_interface_data;
        // reference <usbprint.h>
        GUID guid_usbprint = { 0x28d78fad, 0x5a12, 0x11d1, { 0xae, 0x5b, 0x0, 0x0, 0xf8, 0x3, 0xa8, 0xc2 } };
        LPGUID pGuid = (LPGUID)&guid_usbprint;

        h_dev_info = SetupDiGetClassDevsA(pGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
        if (h_dev_info != INVALID_HANDLE_VALUE) {
            ZeroMemory(&dev_info_data, sizeof(SP_DEVINFO_DATA));
            dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
            DWORD dev_index = 0;
            while (SetupDiEnumDeviceInfo(h_dev_info, dev_index++, &dev_info_data)) {
                DWORD dwRequiredSize;

                ZeroMemory(&dev_interface_data, sizeof(SP_DEVICE_INTERFACE_DATA));
                dev_interface_data.cbSize = sizeof(SP_DEVICE_INTERFACE_DATA);

                DWORD iface_index = 0;
                while (SetupDiEnumDeviceInterfaces(h_dev_info, &dev_info_data, pGuid, iface_index++, &dev_interface_data)) {
                    PSP_DEVICE_INTERFACE_DETAIL_DATA_A p_dev_interface_detail_data = NULL;
                    SetupDiGetDeviceInterfaceDetailA(h_dev_info, &dev_interface_data, NULL, 0, &dwRequiredSize, NULL);
                    p_dev_interface_detail_data = (PSP_DEVICE_INTERFACE_DETAIL_DATA_A)malloc(dwRequiredSize);
                    if (p_dev_interface_detail_data) {
                        p_dev_interface_detail_data->cbSize = sizeof(SP_DEVICE_INTERFACE_DETAIL_DATA_A);
                        if (SetupDiGetDeviceInterfaceDetailA(h_dev_info, &dev_interface_data, p_dev_interface_detail_data, dwRequiredSize, &dwRequiredSize, NULL)) {
                            char *device_path = p_dev_interface_detail_data->DevicePath;
                            size_t device_path_length = strlen(device_path) + 1;
                            required += device_path_length + 1;
                            if (length > device_path_length) {
                                memcpy(buffer, device_path, device_path_length);
                                buffer[device_path_length] = 0;
                                buffer += device_path_length;
                                length -= device_path_length;
                                ++count;
                            }
                        }
                        free(p_dev_interface_detail_data);
                    }
                }
            }

            SetupDiDestroyDeviceInfoList(h_dev_info);
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

class NZComIOEnumerator_win : public NZComIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        HDEVINFO h_dev_info;
        SP_DEVINFO_DATA dev_info_data;
        LPGUID pGuid = (LPGUID)&GUID_CLASS_COMPORT;

        h_dev_info = SetupDiGetClassDevsA(pGuid, NULL, NULL, DIGCF_PRESENT | DIGCF_INTERFACEDEVICE);
        if (h_dev_info != INVALID_HANDLE_VALUE) {
            ZeroMemory(&dev_info_data, sizeof(SP_DEVINFO_DATA));
            dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
            DWORD dev_index = 0;
            while (SetupDiEnumDeviceInfo(h_dev_info, dev_index++, &dev_info_data)) {
                BOOL bResult;

                BYTE PropertyBuffer[MAX_PATH] = { 0, };
                DWORD dwPropertyRegDataType;
                DWORD dwRequiredSize;
                bResult = SetupDiGetDeviceRegistryPropertyA(h_dev_info, &dev_info_data, SPDRP_FRIENDLYNAME, &dwPropertyRegDataType, PropertyBuffer, sizeof(PropertyBuffer) - 1, &dwRequiredSize);
                if (FALSE == bResult)
                    continue;

                char *ps = strstr((char *)PropertyBuffer, "(COM");
                char *pe = strchr((char *)PropertyBuffer, (int)')');
                if (ps && pe && (pe - ps > 4)) {
                    ++ps;
                    *pe = 0;

                    char *device_path = ps;
                    size_t device_path_length = strlen(device_path) + 1;
                    required += device_path_length + 1;
                    if (length > device_path_length) {
                        memcpy(buffer, device_path, device_path_length);
                        buffer[device_path_length] = 0;
                        buffer += device_path_length;
                        length -= device_path_length;
                        ++count;
                    }
                }
            }

            SetupDiDestroyDeviceInfoList(h_dev_info);
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

class NZLptIOEnumerator_win : public NZLptIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        HDEVINFO h_dev_info;
        SP_DEVINFO_DATA dev_info_data;
        LPGUID pGuid = (LPGUID)&GUID_DEVCLASS_PORTS;

        h_dev_info = SetupDiGetClassDevsA(pGuid, NULL, NULL, DIGCF_PRESENT);
        if (h_dev_info != INVALID_HANDLE_VALUE) {
            ZeroMemory(&dev_info_data, sizeof(SP_DEVINFO_DATA));
            dev_info_data.cbSize = sizeof(SP_DEVINFO_DATA);
            DWORD dev_index = 0;
            while (SetupDiEnumDeviceInfo(h_dev_info, dev_index++, &dev_info_data)) {
                                BOOL bResult;

                BYTE PropertyBuffer[MAX_PATH] = { 0, };
                DWORD dwPropertyRegDataType;
                DWORD dwRequiredSize;
                bResult = SetupDiGetDeviceRegistryPropertyA(h_dev_info, &dev_info_data, SPDRP_FRIENDLYNAME, &dwPropertyRegDataType, PropertyBuffer, sizeof(PropertyBuffer) - 1, &dwRequiredSize);
                if (FALSE == bResult)
                    continue;

                char *ps = strstr((char *)PropertyBuffer, "(LPT");
                char *pe = strchr((char *)PropertyBuffer, (int)')');
                if (ps && pe && (pe - ps > 4)) {
                    ++ps;
                    *pe = 0;

                    char *device_path = ps;
                    size_t device_path_length = strlen(device_path) + 1;
                    required += device_path_length + 1;
                    if (length > device_path_length) {
                        memcpy(buffer, device_path, device_path_length);
                        buffer[device_path_length] = 0;
                        buffer += device_path_length;
                        length -= device_path_length;
                        ++count;
                    }
                }
            }

            SetupDiDestroyDeviceInfoList(h_dev_info);
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

class NZPrnIOEnumerator_win : public NZPrnIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        PRINTER_INFO_2A * pPrinterEnum = NULL;
        DWORD pcbNeeded;
        DWORD pcReturned;

        if (!EnumPrintersA(PRINTER_ENUM_LOCAL, NULL, 2, NULL, 0, &pcbNeeded, &pcReturned)) {
            if (GetLastError() == ERROR_INSUFFICIENT_BUFFER) {
                pPrinterEnum = (PRINTER_INFO_2A *)malloc(pcbNeeded);
            }
        }
        if (NULL != pPrinterEnum) {
            if (EnumPrintersA(PRINTER_ENUM_LOCAL, NULL, 2, (BYTE *)pPrinterEnum, pcbNeeded, &pcbNeeded, &pcReturned)) {
                for (DWORD i = 0; i < pcReturned; ++i) {
                    char *device_path = (pPrinterEnum + i)->pPrinterName;
                    size_t device_path_length = (int)strlen(device_path) + 1;
                    required += device_path_length + 1;
                    if (length > device_path_length) {
                        memcpy(buffer, device_path, device_path_length);
                        buffer[device_path_length] = 0;
                        buffer += device_path_length;
                        length -= device_path_length;
                        ++count;
                    }
                }
            }
            free(pPrinterEnum);
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

class NZHostIPEnumerator_win : public NZHostIPEnumerator
{
public:
    NZHostIPEnumerator_win()
    {
        WSADATA wsadata;
        int ret = WSAStartup(MAKEWORD(2,2), &wsadata);
        if (ret != 0)
            fprintf(stderr, "Unable to load Winsock\n");
    }

    ~NZHostIPEnumerator_win()
    {
        WSACleanup();
    }

    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        char hostname[300] = { 0, };
        if (gethostname(hostname, sizeof(hostname)) == 0) {
            hostent *pHostNet = gethostbyname(hostname);
            if (pHostNet && pHostNet->h_addr_list) {
                struct in_addr *host_addr = NULL;
                while (host_addr = (struct in_addr *)(*pHostNet->h_addr_list++)) {
                    char *device_path = inet_ntoa(*host_addr);
                    size_t device_path_length = (int)strlen(device_path) + 1;
                    required += device_path_length + 1;
                    if (length > device_path_length) {
                        memcpy(buffer, device_path, device_path_length);
                        buffer[device_path_length] = 0;
                        buffer += device_path_length;
                        length -= device_path_length;
                        ++count;
                    }
                }
            }
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

#endif
