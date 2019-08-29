#ifndef PRINTERLIBS_H
#define PRINTERLIBS_H

#include "nzio_base.h"
#include "nzprinter_base.h"

#if defined(PLATFORM_WIN)
#include "nzio_win.h"
#elif defined(PLATFORM_MAC)
#include "nzio_mac.h"
#else
#include "nzio_linux.h"
#endif

#if defined(PLATFORM_WIN)
typedef NZComIO_win NZComIOClass;
typedef NZUsbIO_win NZUsbIOClass;
typedef NZLptIO_win NZLptIOClass;
typedef NZPrnIO_win NZPrnIOClass;
typedef NZTcpClientIO_win NZTcpClientIOClass;
typedef NZUdpClientIO_win NZUdpClientIOClass;
typedef NZComIOEnumerator_win NZComIOEnumeratorClass;
typedef NZUsbIOEnumerator_win NZUsbIOEnumeratorClass;
typedef NZLptIOEnumerator_win NZLptIOEnumeratorClass;
typedef NZPrnIOEnumerator_win NZPrnIOEnumeratorClass;
typedef NZHostIPEnumerator_win NZHostIPEnumeratorClass;
#elif defined(PLATFORM_MAC)
typedef NZComIO_mac NZComIOClass;
typedef NZUsbIO_mac NZUsbIOClass;
typedef NZLptIO_mac NZLptIOClass;
typedef NZPrnIO_mac NZPrnIOClass;
typedef NZTcpClientIO_mac NZTcpClientIOClass;
typedef NZUdpClientIO_mac NZUdpClientIOClass;
typedef NZComIOEnumerator_mac NZComIOEnumeratorClass;
typedef NZUsbIOEnumerator_mac NZUsbIOEnumeratorClass;
typedef NZLptIOEnumerator_mac NZLptIOEnumeratorClass;
typedef NZPrnIOEnumerator_mac NZPrnIOEnumeratorClass;
typedef NZHostIPEnumerator_mac NZHostIPEnumeratorClass;
#else
typedef NZComIO_linux NZComIOClass;
typedef NZUsbIO_linux NZUsbIOClass;
typedef NZLptIO_linux NZLptIOClass;
typedef NZPrnIO_linux NZPrnIOClass;
typedef NZTcpClientIO_linux NZTcpClientIOClass;
typedef NZUdpClientIO_linux NZUdpClientIOClass;
typedef NZComIOEnumerator_linux NZComIOEnumeratorClass;
typedef NZUsbIOEnumerator_linux NZUsbIOEnumeratorClass;
typedef NZLptIOEnumerator_linux NZLptIOEnumeratorClass;
typedef NZPrnIOEnumerator_linux NZPrnIOEnumeratorClass;
typedef NZHostIPEnumerator_linux NZHostIPEnumeratorClass;
#endif

#endif
