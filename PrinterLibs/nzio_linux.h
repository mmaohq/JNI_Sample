#ifndef NZIO_LINUX_H
#define NZIO_LINUX_H

#include <stdio.h>
#include <wchar.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <ifaddrs.h>
#include <fcntl.h>
#include <termios.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <pthread.h>
#include <netdb.h>
#include <sys/ioctl.h>
#include <linux/serio.h>
#include "nzio_base.h"

class SelectIOBase_linux : virtual public NZBufferedIO
{
protected:
    int fd;

public:
    SelectIOBase_linux()
    {
        fd = -1;
    }

    virtual ~SelectIOBase_linux()
    {

    }

    bool IsOpened()
    {
        return fd != -1;
    }

    void Close()
    {
        if (fd != -1) {
            close(fd);
            fd = -1;
        }
    }

    int SelectWrite(const unsigned long timeout)
    {
        struct timeval tvtimeout;
        fd_set w;
        FD_ZERO(&w);
        FD_SET(fd, &w);
        tvtimeout.tv_sec = timeout / 1000;
        tvtimeout.tv_usec = (timeout % 1000) * 1000;
        return select(fd + 1, 0, &w, 0, &tvtimeout);
    }

    int SelectRead(const unsigned long timeout)
    {
        struct timeval tvtimeout;
        fd_set r;
        FD_ZERO(&r);
        FD_SET(fd, &r);
        tvtimeout.tv_sec = timeout / 1000;
        tvtimeout.tv_usec = (timeout % 1000) * 1000;
        return select(fd + 1, &r, 0, 0, &tvtimeout);
    }

    int BaseWrite(const unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        int ret = SelectWrite(timeout);
        if (ret <= 0)
            return ret;
        return (int)write(fd, buffer, count);
    }

    int BaseReadAvailable(unsigned char *buffer, const size_t count, const unsigned long timeout)
    {
        int ret = SelectRead(timeout);
        if (ret <= 0)
            return ret;
        return (int)read(fd, buffer, count);
    }
};

class NZComIO_linux : public SelectIOBase_linux, public NZComIO
{
public:
    bool Open(const char *name, const unsigned int baudrate, const ComDataBits databits, const ComParity parity, const ComStopBits stopbits, const ComFlowControl flowcontrol)
    {
        (void)databits;
        (void)parity;
        (void)stopbits;
        (void)flowcontrol;

        if ((fd = open(name, O_RDWR | O_NOCTTY)) < 0)
            return false;

        // get the current options for the port
        struct termios tios;
        tcgetattr(fd,&tios);

        // set the baudrate
        unsigned int baudrates[] = { 9600, 19200, 38400, 57600, 115200, 230400, 460800, 500000, 576000, 921600, 1000000, 1152000, 1500000};
        unsigned int baudflags[] = {B9600,B19200,B38400,B57600,B115200,B230400,B460800,B500000,B576000,B921600,B1000000,B1152000,B1500000};
        for(size_t i = 0; i < (sizeof(baudrates)/sizeof(unsigned int)); ++i) {
            if(baudrates[i] == baudrate) {
                cfsetospeed(&tios,baudflags[i]);
                cfsetispeed(&tios,baudflags[i]);
            }
        }

        // enable the receiver and set local mode
        tios.c_cflag |= CLOCAL | CREAD;
        // select 8 data bits
        tios.c_cflag &= ~CSIZE;
        tios.c_cflag |= CS8;
        // disable parity bit
        tios.c_cflag &= ~PARENB;
        // 1 stop bits
        tios.c_cflag &= ~CSTOPB;
        // disable RTS CTS
        tios.c_cflag &= ~CRTSCTS;

        // select raw mode
        tios.c_lflag &= ~(ICANON | ECHO | ISIG);
        tios.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
        // disable XON XOFF
        tios.c_iflag &= ~(IXON | IXOFF | IXANY);
        // ignore break;
        tios.c_iflag |= IGNBRK;
        // ignore parity error
        tios.c_iflag |= IGNPAR;

        // choosing raw output
        tios.c_oflag &= ~(OPOST | ONLCR | OLCUC | OCRNL | ONOCR | ONLRET | OFILL);

        // VMIN and VTIME is only affect on block mode
        tios.c_cc[VMIN] = 0;
        tios.c_cc[VTIME] = 0;

        // apply options for the port
        tcsetattr(fd,TCSANOW,&tios);

        return true;
    }
};

class NZUsbIO_linux : public SelectIOBase_linux, public NZUsbIO
{
public:
    bool Open(const char *name)
    {
        if ((fd = open(name, O_RDWR)) < 0)
            return false;

        return true;
    }
};

class NZLptIO_linux : public SelectIOBase_linux, public NZLptIO
{
public:
    bool Open(const char *name)
    {
        if ((fd = open(name, O_RDWR)) < 0)
            return false;

        return true;
    }
};

class NZPrnIO_linux : public NZPrnIO
{
public:

};

class NZSocketIO_linux : virtual public NZSocketIO
{
protected:
    int s;

public:
    NZSocketIO_linux()
    {
        s = -1;
    }

    virtual ~NZSocketIO_linux() { }

    virtual bool IsOpened()
    {
        return s != -1;
    }

    virtual void Close()
    {
        if (s != -1) {
            close(s);
            s = -1;
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
        sl.l_linger = (int)timeout;
        return setsockopt(s, SOL_SOCKET, SO_LINGER, &sl, sizeof(sl)) == 0;
    }

    virtual bool setsockopt_reuseaddr(const bool on)
    {
        int bReuseaddr = on ? 1 : 0;
        return setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &bReuseaddr, sizeof(bReuseaddr)) == 0;
    }

    virtual bool setsockopt_sndbuf(const size_t size)
    {
        int nSize = (int)size;
        return setsockopt(s, SOL_SOCKET, SO_SNDBUF, &nSize, sizeof(nSize)) == 0;
    }

    virtual bool setsockopt_rcvbuf(const size_t size)
    {
        int nSize = (int)size;
        return setsockopt(s, SOL_SOCKET, SO_RCVBUF, &nSize, sizeof(nSize)) == 0;
    }

    virtual bool setsockopt_rcvtimeo(const unsigned long timeout)
    {
        struct timeval tv;
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        return setsockopt(s, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) == 0;
    }

    virtual bool setsockopt_sndtimeo(const unsigned long timeout)
    {
        struct timeval tv;
        tv.tv_sec = timeout / 1000;
        tv.tv_usec = (timeout % 1000) * 1000;
        return setsockopt(s, SOL_SOCKET, SO_SNDTIMEO, &tv, sizeof(tv)) == 0;
    }

};

class NZTcpClientIO_linux : public NZSocketIO_linux, public NZTcpClientIO
{
public:
    bool Open(const char *local_ip, const char *dest_ip, const unsigned short dest_port, const unsigned long timeout)
    {
        (void)timeout;
        s = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
        if (IsOpened()) {
            if (IsOpened()) {
                if (local_ip && strlen(local_ip)) {
                    if (!bind_addr(local_ip, 0))
                        Close();
                }
            }

            if (IsOpened()) {
                sockaddr_in server_addr;
                server_addr.sin_family = AF_INET;
                server_addr.sin_addr.s_addr = inet_addr(dest_ip);
                server_addr.sin_port = htons(dest_port);
                int ret = connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr));
                if (ret != 0) {
                    Close();
                }
            }
        }

        return IsOpened();
    }

    virtual int socket_send(const unsigned char *buffer, const size_t count)
    {
        ssize_t ret = send(s, buffer, count, 0);
        if (ret < 0)
            printf("socket send error: %d\n", errno);
        return (int)ret;
    }

    virtual int socket_recv(unsigned char *buffer, const size_t count)
    {
        ssize_t ret = recv(s, buffer, count, 0);
        if (ret < 0) {
            switch(errno) {
            case EAGAIN:
                ret = 0;
                break;
            default:
                printf("socket recv error: %d\n", errno);
                break;
            }
        }
        return (int)ret;
    }

    bool setsockopt_keepalive(const bool on)
    {
        int bKeepAlive = on ? 1 : 0;
        return setsockopt(s, SOL_SOCKET, SO_KEEPALIVE, &bKeepAlive, sizeof(bKeepAlive)) == 0;
    }

    bool setsockopt_keepidle(const unsigned int keepidle)
    {
        int val = (int)(keepidle/1000);
        return setsockopt(s, SOL_TCP, TCP_KEEPIDLE, &val, sizeof(val)) == 0;
    }

    bool setsockopt_keepintvl(const unsigned int keepintvl)
    {
        int val = (int)(keepintvl/1000);
        return setsockopt(s, SOL_TCP, TCP_KEEPINTVL, &val, sizeof(val)) == 0;
    }

    bool setsockopt_keepcnt(const unsigned int keepcnt)
    {
        int val = (int)keepcnt;
        return setsockopt(s, SOL_TCP, TCP_KEEPCNT, &val, sizeof(val)) == 0;
    }

    bool setsockopt_keepalivevals(const bool onoff, const unsigned int keepalivetime, const unsigned int keepaliveinterval, const unsigned int keepalivecount)
    {
        if (!setsockopt_keepalive(onoff))
            return false;

        if (!setsockopt_keepidle(keepalivetime))
            return false;

        if (!setsockopt_keepintvl(keepaliveinterval))
            return false;

        if (!setsockopt_keepcnt(keepalivecount))
            return false;

        return true;
    }
};

class NZUdpClientIO_linux : public NZSocketIO_linux, public NZUdpClientIO
{
public:
    bool Open(const char *local_ip, const unsigned short local_port, const char *dest_ip, const unsigned short dest_port)
    {
        s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
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
        to_addr.sin_addr.s_addr = (in_addr_t)to_ip;

        ssize_t ret = sendto(s, buffer, count, 0, (sockaddr *)&to_addr, (socklen_t)sizeof(to_addr));
        if (ret < 0)
            printf("socket recv error: %d\n", errno);
        return (int)ret;
    }

    virtual int socket_recvfrom(unsigned char *buffer, const size_t count, unsigned long *from_ip, unsigned short *from_port)
    {
        sockaddr_in from_addr;
        memset(&from_addr, 0, sizeof(from_addr));
        socklen_t from_addr_size = (socklen_t)sizeof(from_addr);
        ssize_t ret = recvfrom(s, (char *)buffer, (int)count, 0, (sockaddr *)&from_addr, &from_addr_size);
        if (ret >= 0) {
            if (from_ip) {
                *from_ip = from_addr.sin_addr.s_addr;
            }
            if (from_port)
                *from_port = from_addr.sin_port;
        }
        if (ret < 0) {
            switch(errno) {
            case EAGAIN:
                ret = 0;
                break;
            default:
                printf("socket recv error: %d\n", errno);
                break;
            }
        }
        return (int)ret;
    }

    virtual bool setsockopt_broadcast(const bool on)
    {
        int bBroadcast = on ? 1 : 0;
        return setsockopt(s, SOL_SOCKET, SO_BROADCAST, &bBroadcast, sizeof(bBroadcast)) == 0;
    }
};

class NZDirectoryList_linux
{
public:
    static size_t ListFiles(const char *dir_name, const char **prefix_list, char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        DIR *p_dir = opendir(dir_name);
        if (p_dir) {
            dirent *p_dirent;
            while ((p_dirent = readdir(p_dir)) != NULL) {
                if (p_dirent->d_type == DT_DIR)
                    continue;
                if (prefix_list) {
                    char **temp_prefix_list = (char **)prefix_list;
                    char *prefix_name;
                    char *substrpos = NULL;
                    while ((prefix_name = *temp_prefix_list++) != NULL) {
                        substrpos = strstr(p_dirent->d_name, prefix_name);
                        if (substrpos == NULL)
                            continue;
                        if (substrpos != p_dirent->d_name)
                            continue;
                        if (substrpos == p_dirent->d_name)
                            break;
                    }
                    if (substrpos != p_dirent->d_name)
                        continue;
                }

                if (strlen(p_dirent->d_name)) {
                    char file_name[300];
                    sprintf(file_name, "%s/%s", dir_name, p_dirent->d_name);
                    char *device_path = file_name;
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
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

class NZUsbIOEnumerator_linux : public NZUsbIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        const char *prefix[] = { "lp", 0, };
        return NZDirectoryList_linux::ListFiles("/dev/usb", prefix, buffer, length, prequired);
    }
};

class NZComIOEnumerator_linux : public NZComIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        const char *prefix[] = { "ttyS", "ttyUSB", 0, };
        return NZDirectoryList_linux::ListFiles("/dev", prefix, buffer, length, prequired);
    }
};

class NZLptIOEnumerator_linux : public NZLptIOEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        const char *prefix[] = { "lp", 0, };
        return NZDirectoryList_linux::ListFiles("/dev", prefix, buffer, length, prequired);
    }
};

class NZPrnIOEnumerator_linux : public NZPrnIOEnumerator
{
public:

};

class NZHostIPEnumerator_linux : public NZHostIPEnumerator
{
public:
    virtual size_t Enumerate(char *buffer, size_t length, size_t *prequired)
    {
        size_t count = 0;
        size_t required = 0;

        struct ifaddrs *org_ifa;
        if (getifaddrs(&org_ifa) == 0) {
            struct ifaddrs *ifa = org_ifa;
            while (ifa != NULL) {
                if ((ifa->ifa_addr != NULL) &&
                        (ifa->ifa_addr->sa_family == AF_INET) &&
                        (strncmp(ifa->ifa_name, "lo", 2) != 0)) {
                    struct sockaddr_in *addr = (struct sockaddr_in*)ifa->ifa_addr;

                    char *device_path = inet_ntoa(addr->sin_addr);
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
                ifa = ifa->ifa_next;
            }
            freeifaddrs(org_ifa);
        }

        if (prequired)
            *prequired = required;
        return count;
    }
};

#endif // NZIO_LINUX_H
