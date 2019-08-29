#ifndef NZPRINTER_BASE_H
#define NZPRINTER_BASE_H

#include <stdlib.h>
#include <string.h>
#include "nzio_base.h"

#define NZPRINTER_RETURNCODE_SUCCESS                    0
#define NZPRINTER_RETURNCODE_ERROR_PORT_INVALID         NZIO_ERROR_PORT_INVALID
#define NZPRINTER_RETURNCODE_ERROR_WRITE_FAILED         NZIO_ERROR_WRITE_FAILED
#define NZPRINTER_RETURNCODE_ERROR_READ_FAILED          NZIO_ERROR_READ_FAILED
#define NZPRINTER_RETURNCODE_ERROR_TIMEOUT              NZIO_ERROR_TIMEOUT
#define NZPRINTER_RETURNCODE_ERROR_UNKNOWN_ERROR        -101
#define NZPRINTER_RETURNCODE_ERROR_PRINTER_OFFLINE      -102
#define NZPRINTER_RETURNCODE_ERROR_PRINTER_NOPAPER      -103
#define NZPRINTER_RETURNCODE_ERROR_UNKNOWN_PRINTER      -104

#define nz_max(a,b) (((a) > (b)) ? (a) : (b))
#define nz_min(a,b) (((a) < (b)) ? (a) : (b))

#define POS_PRINTRESULT_SUCCESS 0
#define POS_PRINTRESULT_PORT_CLOSED -1
#define POS_PRINTRESULT_PORT_WRITEFAILED -2
#define POS_PRINTRESULT_PORT_READFAILED -3
#define POS_PRINTRESULT_PRINTER_OFFLINE -102
#define POS_PRINTRESULT_PRINTER_NOPAPER -103
#define POS_PRINTRESULT_OTHER_RERROR -101

#define POS_PRINTERSTATUS_DRAWER_OPENED(status) (((status >> 0) & 0x04) == 0x00)
#define POS_PRINTERSTATUS_OFFLINE(status) (((status >> 0) & 0x08) == 0x08)
#define POS_PRINTERSTATUS_COVERUP(status) (((status >> 8) & 0x04) == 0x04)
#define POS_PRINTERSTATUS_FEED_PRESSED(status) (((status >> 8) & 0x08) == 0x08)
#define POS_PRINTERSTATUS_NOPAPER(status) (((status >> 8) & 0x20) == 0x20)
#define POS_PRINTERSTATUS_ERROR_OCCURED(status) (((status >> 8) & 0x40) == 0x40)
#define POS_PRINTERSTATUS_CUTTER_ERROR(status) (((status >> 16) & 0x08) == 0x08)
#define POS_PRINTERSTATUS_UNRECOVERABLE_ERROR(status) (((status >> 16) & 0x20) == 0x20)
#define POS_PRINTERSTATUS_DEGREE_OR_VOLTAGE_OVERRANGE(status) (((status >> 16) & 0x40) == 0x40)
#define POS_PRINTERSTATUS_PAPER_NEAREND(status) (((status >> 24) & 0x0C) == 0x0C)
#define POS_PRINTERSTATUS_PAPER_TAKEOUT(status) (((status >> 24) & 0x04) == 0x04)

#ifndef EnumTypePrintDensity
#define EnumTypePrintDensity
enum PrintDensity { PrintDensity_Light, PrintDensity_Normal, PrintDensity_Dark };
#endif

#ifndef EnumTypePosAlignment
#define EnumTypePosAlignment
enum PosAlignment { PosAlignment_Left, PosAlignment_HCenter, PosAlignment_Right };
#endif

#ifndef EnumTypePageModeDrawDirection
#define EnumTypePageModeDrawDirection
enum PageModeDrawDirection { PageModeDrawDirection_LeftToRight, PageModeDrawDirection_BottomToTop, PageModeDrawDirection_RightToLeft, PageModeDrawDirection_TopToBottom };
#endif

#ifndef EnumTypeMultiByteModeEncoding
#define EnumTypeMultiByteModeEncoding
enum MultiByteModeEncoding { MultiByteModeEncoding_GBK = 0, MultiByteModeEncoding_UTF8 = 1, MultiByteModeEncoding_BIG5 = 3, MultiByteModeEncoding_ShiftJIS = 4, MultiByteModeEncoding_EUCKR = 5 };
#endif

#ifndef EnumTypeAsciiTextFontType
#define EnumTypeAsciiTextFontType
enum AsciiTextFontType { AsciiTextFontType_A, AsciiTextFontType_B, AsciiTextFontType_C, AsciiTextFontType_D, AsciiTextFontType_E };
#endif

#ifndef EnumTypeTextUnderline
#define EnumTypeTextUnderline
enum TextUnderline { TextUnderline_None, TextUnderline_One, TextUnderline_Two };
#endif

#ifndef EnumTypeCharacterSet
#define EnumTypeCharacterSet
enum CharacterSet {
    CharacterSet_USA = 0,
    CharacterSet_FRANCE = 1,
    CharacterSet_GERMANY = 2,
    CharacterSet_UK = 3,
    CharacterSet_DENMARK_I = 4,
    CharacterSet_SWEDEN = 5,
    CharacterSet_ITALY = 6,
    CharacterSet_SPAIN_I = 7,
    CharacterSet_JAPAN = 8,
    CharacterSet_NORWAY = 9,
    CharacterSet_DENMARK_II = 10,
    CharacterSet_SPAIN_II = 11,
    CharacterSet_LATIN = 12,
    CharacterSet_KOREA = 13,
    CharacterSet_SLOVENIA = 14,
    CharacterSet_CHINA = 15
};
#endif

#ifndef EnumTypeCharacterCodepage
#define EnumTypeCharacterCodepage
enum CharacterCodepage {
    CharacterCodepage_CP437 = 0,
    CharacterCodepage_KATAKANA = 1,
    CharacterCodepage_CP850 = 2,
    CharacterCodepage_CP860 = 3,
    CharacterCodepage_CP863 = 4,
    CharacterCodepage_CP865 = 5,
    CharacterCodepage_WCP1251 = 6,
    CharacterCodepage_CP866 = 7,
    CharacterCodepage_MIK = 8,
    CharacterCodepage_CP755 = 9,
    CharacterCodepage_IRAN = 10,
    CharacterCodepage_CP862 = 15,
    CharacterCodepage_WCP1252 = 16,
    CharacterCodepage_WCP1253 = 17,
    CharacterCodepage_CP852 = 18,
    CharacterCodepage_CP858 = 19,
    CharacterCodepage_IRAN_II = 20,
    CharacterCodepage_LATVIAN = 21,
    CharacterCodepage_CP864 = 22,
    CharacterCodepage_ISO_8859_1 = 23,
    CharacterCodepage_CP737 = 24,
    CharacterCodepage_WCP1257 = 25,
    CharacterCodepage_THAI = 26,
    CharacterCodepage_CP720 = 27,
    CharacterCodepage_CP855 = 28,
    CharacterCodepage_CP857 = 29,
    CharacterCodepage_WCP1250 = 30,
    CharacterCodepage_CP775 = 31,
    CharacterCodepage_WCP1254 = 32,
    CharacterCodepage_WCP1255 = 33,
    CharacterCodepage_WCP1256 = 34,
    CharacterCodepage_WCP1258 = 35,
    CharacterCodepage_ISO_8859_2 = 36,
    CharacterCodepage_ISO_8859_3 = 37,
    CharacterCodepage_ISO_8859_4 = 38,
    CharacterCodepage_ISO_8859_5 = 39,
    CharacterCodepage_ISO_8859_6 = 40,
    CharacterCodepage_ISO_8859_7 = 41,
    CharacterCodepage_ISO_8859_8 = 42,
    CharacterCodepage_ISO_8859_9 = 43,
    CharacterCodepage_ISO_8859_15 = 44,
    CharacterCodepage_THAI_2 = 45,
    CharacterCodepage_CP856 = 46,
    CharacterCodepage_CP874 = 47,
    CharacterCodepage_TCVN3 = 48
};
#endif

#ifndef EnumTypeBarcodeReadableTextFontType
#define EnumTypeBarcodeReadableTextFontType
enum BarcodeReadableTextFontType { BarcodeReadableTextFontType_Standard, BarcodeReadableTextFontType_Small };
#endif

#ifndef EnumTypeBarcodeReadableTextPosition
#define EnumTypeBarcodeReadableTextPosition
enum BarcodeReadableTextPosition { BarcodeReadableTextPosition_None, BarcodeReadableTextPosition_AboveBarcode, BarcodeReadableTextPosition_BelowBarcode, BarcodeReadableTextPosition_AboveAndBelowBarcode };
#endif

#ifndef EnumTypePosBarcodeType
#define EnumTypePosBarcodeType
enum PosBarcodeType {
    PosBarcodeType_UPCA = 0x41,
    PosBarcodeType_UPCE = 0x42,
    PosBarcodeType_EAN13 = 0x43,
    PosBarcodeType_EAN8 = 0x44,
    PosBarcodeType_CODE39 = 0x45,
    PosBarcodeType_ITF = 0x46,
    PosBarcodeType_CODEBAR = 0x47,
    PosBarcodeType_CODE93 = 0x48,
    PosBarcodeType_CODE128 = 0x49
};
#endif

#ifndef EnumTypeQRCodeECC
#define EnumTypeQRCodeECC
enum QRCodeECC { QRCodeECC_L = 1, QRCodeECC_M = 2, QRCodeECC_Q = 3, QRCodeECC_H = 4 };
#endif

#ifndef EnumTypeLabelRotation
#define EnumTypeLabelRotation
enum LabelRotation{ LabelRotation_0, LabelRotation_90, LabelRotation_180, LabelRotation_270 };
#endif

#define LABEL_TEXT_STYLE_BOLD (1<<0)
#define LABEL_TEXT_STYLE_UNDERLINE (1<<1)
#define LABEL_TEXT_STYLE_HIGHLIGHT (1<<2)
#define LABEL_TEXT_STYLE_STRIKETHROUGH (1<<3)
#define LABEL_TEXT_STYLE_ROTATION_0 (0<<4)
#define LABEL_TEXT_STYLE_ROTATION_90 (1<<4)
#define LABEL_TEXT_STYLE_ROTATION_180 (2<<4)
#define LABEL_TEXT_STYLE_ROTATION_270 (3<<4)
#define LABEL_TEXT_STYLE_WIDTH_ENLARGEMENT(n) ((n)<<8)
#define LABEL_TEXT_STYLE_HEIGHT_ENLARGEMENT(n) ((n)<<12)

#ifndef EnumTypeLabelBarcodeType
#define EnumTypeLabelBarcodeType
enum LabelBarcodeType {
    LabelBarcodeType_UPCA = 0,
    LabelBarcodeType_UPCE = 1,
    LabelBarcodeType_EAN13 = 2,
    LabelBarcodeType_EAN8 = 3,
    LabelBarcodeType_CODE39 = 4,
    LabelBarcodeType_ITF = 5,
    LabelBarcodeType_CODEBAR = 6,
    LabelBarcodeType_CODE93 = 7,
    LabelBarcodeType_CODE128 = 8,
    LabelBarcodeType_CODE11 = 9,
    LabelBarcodeType_MSI = 10,
    LabelBarcodeType_128M = 11,
    LabelBarcodeType_EAN128 = 12,
    LabelBarcodeType_25C = 13,
    LabelBarcodeType_39C = 14,
    LabelBarcodeType_39 = 15,
    LabelBarcodeType_EAN13PLUS2 = 16,
    LabelBarcodeType_EAN13PLUS5 = 17,
    LabelBarcodeType_EAN8PLUS2 = 18,
    LabelBarcodeType_EAN8PLUS5 = 19,
    LabelBarcodeType_POST = 20,
    LabelBarcodeType_UPCAPLUS2 = 21,
    LabelBarcodeType_UPCAPLUS5 = 22,
    LabelBarcodeType_UPCEPLUS2 = 23,
    LabelBarcodeType_UPCEPLUS5 = 24,
    LabelBarcodeType_CPOST = 25,
    LabelBarcodeType_MSIC = 26,
    LabelBarcodeType_PLESSEY = 27,
    LabelBarcodeType_ITF14 = 28,
    LabelBarcodeType_EAN14 = 29
};
#endif

#ifndef EnumTypeLabelColor
#define EnumTypeLabelColor
enum LabelColor{ LabelColor_White, LabelColor_Black };
#endif

// Query Encoding Hardware
//
//   Query Functions return int  to indicate query result
//   Write Functions return bool to indicate write result
//   Read  Functions return int  to indicate read  result
class NZPrinter : public NZIOHolder
{
protected:
    // 10 seconds
    unsigned long default_short_timeout_ms;
    // 10 minutes
    unsigned long default_long_timeout_ms;

public:
    NZPrinter()
    {
        default_short_timeout_ms = 10000;
        default_long_timeout_ms = 600000;
    }

    int POS_QueryPrinterBufferEmpty(unsigned long timeout)
    {
        unsigned char cmd[] = { 0x1d, 0x72, 0x01 };
        return QueryOneByteStatus(cmd, sizeof(cmd), timeout);
    }
    int POS_RTQueryStatus(unsigned char nQueryType, unsigned long timeout)
    {
        unsigned char cmd[] = { 0x10, 0x04, nQueryType };
        return QueryOneByteStatus(cmd, sizeof(cmd), timeout);
    }
    long POS_QueryPrinterStatus(unsigned long timeout)
    {
        unsigned char cmd[] = { 0x10, 0x04, 0x01, 0x10, 0x04, 0x02, 0x10, 0x04, 0x03, 0x10, 0x04, 0x04 };
        unsigned char status[4];
        int nBytesReaded = QueryResponse(cmd, sizeof(cmd), status, sizeof(status), timeout);
        if (nBytesReaded < 0)
            return (long)-1;
        else if ((size_t)nBytesReaded != sizeof(status))
            return (long)-1;
        else
            return (long)(status[0] | (status[1] << 8) | (status[2] << 16) | (status[3] << 24));
    }
    int POS_QueryPrintResult(unsigned long nIndex, unsigned long timeout)
    {
        if (!IsOpened())
            return (int)NZPRINTER_RETURNCODE_ERROR_PORT_INVALID;

        unsigned char recbuf[7];
        unsigned char rtqueryCmd[] = { 0x10, 0x04, 0x01 };
        unsigned char ticketCmd[] = { 0x1d, 0x28, 0x48, 0x06, 0x00, 0x30, 0x30, (unsigned char)nIndex, (unsigned char)(nIndex >> 8), (unsigned char)(nIndex >> 16), (unsigned char)(nIndex >> 24) };

        SkipAvailable();
        int nBytesWrited = Write(ticketCmd, sizeof(ticketCmd), timeout);
        if (nBytesWrited < 0) {
            return nBytesWrited;
        } else if ((size_t)nBytesWrited != sizeof(ticketCmd)) {
            return (int)NZPRINTER_RETURNCODE_ERROR_WRITE_FAILED;
        } else {
            unsigned long timesec = (timeout + 999)/1000;
            time_t beginTime = time(0);
            while (true) {
                if (!IsOpened())
                    return (int)NZPRINTER_RETURNCODE_ERROR_PORT_INVALID;
                if (difftime(time(0), beginTime) > (double)timesec)
                    return (int)NZPRINTER_RETURNCODE_ERROR_TIMEOUT;

                int nBytesReaded = Read(&recbuf[0], 1, 1000);
                if (nBytesReaded < 0) {
                    return nBytesReaded;
                } else if (nBytesReaded == 0) {
                    nBytesWrited = Write(rtqueryCmd, sizeof(rtqueryCmd), timeout);
                    if (nBytesWrited < 0) {
                        return nBytesWrited;
                    } else if ((size_t)nBytesWrited != sizeof(rtqueryCmd)) {
                        return (int)NZPRINTER_RETURNCODE_ERROR_WRITE_FAILED;
                    }
                } else if (nBytesReaded == 1) {
                    if (recbuf[0] == 0x37) {
                        if (Read(&recbuf[1], 1, timeout) == 1) {
                            if ((recbuf[1] == 0x22) || (recbuf[1] == 0x33)) {
                                if (Read(&recbuf[2], 5, timeout) == 5) {
                                    unsigned long dwRecvIndex = (unsigned long)((recbuf[2]) | (recbuf[3] << 8) | (recbuf[4] << 16) | (recbuf[5] << 24));
                                    if(nIndex == dwRecvIndex) {
                                        printf("Ticket Result: %02X %02X %02X %02X %02X %02X %02X\r\n", recbuf[0], recbuf[1], recbuf[2], recbuf[3], recbuf[4], recbuf[5], recbuf[6]);
                                        switch (recbuf[1]) {
                                        case 0x22:
                                            return (int)NZPRINTER_RETURNCODE_SUCCESS;
                                        case 0x33:
                                            return (int)NZPRINTER_RETURNCODE_ERROR_PRINTER_NOPAPER;
                                        default:
                                            return (int)NZPRINTER_RETURNCODE_ERROR_UNKNOWN_ERROR;
                                        }
                                    } else {
                                        printf("dwSendIndex:%ld dwRecvIndex:%ld\r\n", nIndex, dwRecvIndex);
                                    }
                                }
                            }
                        }
                    } else if ((recbuf[0] & 0x12) == 0x12) {
                        printf("Printer RT Status: %02X\r\n",recbuf[0]);
                        if ((recbuf[0] & 0x08) != 0) {
                            return (int)NZPRINTER_RETURNCODE_ERROR_PRINTER_OFFLINE;
                        }
                    }
                }
            }
        }
    }

    bool POS_KickDrawer(unsigned char nDrawerIndex, unsigned char nHightLevelTime, unsigned char nLowLevelTime)
    {
        unsigned char data[] = {0x1b, 0x70, nDrawerIndex, nHightLevelTime, nLowLevelTime };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_Beep(unsigned char nBeepCount, unsigned char nBeepMillis)
    {
        unsigned char data[] = {0x1b, 0x42, nBeepCount, nBeepMillis};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_FeedAndCutPaper()
    {
        unsigned char data[] = {0x1d, 0x56, 0x42, 0x00};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_FullCutPaper()
    {
        unsigned char data[] = { 0x1b, 0x69 };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_HalfCutPaper()
    {
        unsigned char data[] = { 0x1b, 0x6d };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_Reset()
    {
        unsigned char data[] = {0x1b, 0x40};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetPrinter(unsigned char nSetType, unsigned char *buffer, unsigned short count)
    {
        bool result = false;
        size_t cmdlen = count + 5;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1f;
            cmd[1] = 0x28;
            cmd[2] = nSetType;
            cmd[3] = (unsigned char)(count);
            cmd[4] = (unsigned char)(count >> 8);
            if (count)
                memcpy(&cmd[5], buffer, count);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }
        return result;
    }
    bool POS_SetPrintSpeed(unsigned short nSpeed)
    {
        unsigned char data[] = {0x1F, 0x28, 0x73, 0x02, 0x00, (unsigned char)(nSpeed), (unsigned char)(nSpeed >> 8)};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetPrintDensity(unsigned char nDensity)
    {
        unsigned char data[] = {0x1F, 0x28, 0x73, 0x01, 0x00, nDensity };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetPrintHeatPara(unsigned short nMaxHeatDots, unsigned short nHeatOnTime, unsigned short nHeatOffTime)
    {
        return POS_SetPrintHeatParaBase((unsigned char)(nMaxHeatDots/8-1), (unsigned char)(nHeatOnTime/10), (unsigned char)(nHeatOffTime/10));
    }
    bool POS_SetPrintHeatParaBase(unsigned char nMaxHeatDots, unsigned char nHeatOnTime, unsigned char nHeatOffTime)
    {
        unsigned char data[] = { 0x1b, 0x37, nMaxHeatDots, nHeatOnTime, nHeatOffTime };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_PrintSelfTestPage()
    {
		printf("NZPrinter->POS_PrintSelfTestPage:12 54\n");
        unsigned char data[] = {0x12, 0x54};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }

    bool POS_SetSingleByteMode()
    {
        unsigned char data[] = { 0x1c, 0x2e };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetCharacterSet(unsigned char nCharacterSet)
    {
        unsigned char data[] = { 0x1b, 0x52, nCharacterSet };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetCharacterCodepage(unsigned char nCharacterCodepage)
    {
        unsigned char data[] = { 0x1b, 0x74, nCharacterCodepage };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetMultiByteMode()
    {
        unsigned char data[] = { 0x1c, 0x26 };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetMultiByteEncoding(unsigned char nEncoding)
	{
        unsigned char data[] = { 0x1b, 0x39, nEncoding };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }

    // default io timeout is 10000 if func para dos't pass timeout
    void SetIODefaultShortTimeout(unsigned long timeout)
    {
        default_short_timeout_ms = timeout;
    }

    void SetIODefaultLongTimeout(unsigned long timeout)
    {
        default_long_timeout_ms = timeout;
    }
};

class NZPosPrinter : public NZPrinter
{
public:
    bool POS_SetMovementUnit(unsigned char nHorizontalMU, unsigned char nVerticalMU)
    {
        unsigned char data[] = { 0x1d, 0x50, nHorizontalMU, nVerticalMU };
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetPrintAreaLeftMargin(unsigned short nLeftMargin)
    {
        unsigned char cmd[] = {0x1d, 0x4c, (unsigned char)(nLeftMargin%0x100), (unsigned char)(nLeftMargin/0x100)};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetPrintAreaWidth(unsigned short nWidth)
    {
        unsigned char cmd[] = {0x1d, 0x57, (unsigned char)(nWidth%0x100), (unsigned char)(nWidth/0x100)};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SelectPageMode()
    {
        unsigned char cmd[] = {0x1b, 0x4c};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_ExitPageMode()
    {
        unsigned char cmd[] = {0x1b, 0x53};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_ClearPage()
    {
        unsigned char cmd[] = {0x18};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_PrintPage()
    {
        unsigned char cmd[] = {0x1b, 0x0c};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetPageArea(unsigned short x, unsigned short y, unsigned short w, unsigned short h)
    {
        unsigned char cmd[] = {
            0x1b, 0x57,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00,
            0x00, 0x00
        };
        cmd[2] = (unsigned char)(x);
        cmd[3] = (unsigned char)(x >> 8);
        cmd[4] = (unsigned char)(y);
        cmd[5] = (unsigned char)(y >> 8);
        cmd[6] = (unsigned char)(w);
        cmd[7] = (unsigned char)(w >> 8);
        cmd[8] = (unsigned char)(h);
        cmd[9] = (unsigned char)(h >> 8);

        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetPageModeDrawDirection(unsigned char nDirection)
    {
        unsigned char cmd[] = {0x1b, 0x54, nDirection};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetHorizontalAbsolutePrintPosition(unsigned short nPosition)
    {
        unsigned char cmd[] = {0x1b, 0x24, (unsigned char)(nPosition), (unsigned char)(nPosition >> 8)};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetHorizontalRelativePrintPosition(unsigned short nPosition)
    {
        unsigned char cmd[] = {0x1b, 0x5c, (unsigned char)(nPosition), (unsigned char)(nPosition >> 8)};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetHorizontalRelativePrintPositionAcceptNegative(int nPosition)
    {
        if (nPosition < 0)
            nPosition = 65536 + nPosition;
        return POS_SetHorizontalRelativePrintPosition((unsigned short)nPosition);
    }
    bool POS_SetVerticalAbsolutePrintPosition(unsigned short nPosition)
    {
        unsigned char cmd[] = {0x1d, 0x24, (unsigned char)(nPosition), (unsigned char)(nPosition >> 8)};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetVerticalRelativePrintPosition(unsigned short nPosition)
    {
        unsigned char cmd[] = {0x1d, 0x5c, (unsigned char)(nPosition), (unsigned char)(nPosition >> 8)};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetVerticalRelativePrintPositionAcceptNegative(int nPosition)
    {
        if (nPosition < 0)
            nPosition = 65536 + nPosition;
        return POS_SetVerticalRelativePrintPosition((unsigned short)nPosition);
    }
    bool POS_SetAlignment(unsigned char nAlignment)
    {
        unsigned char ESC_a_n[] = {0x1b, 0x61, nAlignment};
        return (Write(ESC_a_n, sizeof(ESC_a_n), default_short_timeout_ms) == sizeof(ESC_a_n));
    }
    bool POS_FeedLines(unsigned char lines)
    {
        unsigned char data[] = {0x1b, 0x64, lines};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_FeedDots(unsigned char dots)
    {
        unsigned char data[] = {0x1b, 0x4a, dots};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }

    bool POS_PrintString(const char *str)
    {
        return Write((unsigned char *)str, strlen(str), default_long_timeout_ms) == (int)strlen(str);
    }

    bool POS_SetTextScale(unsigned char nWidthScale, unsigned char nHeightScale)
    {
        unsigned char cmd[] = { 0x1d, 0x21, (unsigned char)((nHeightScale & 0x07) | ((nWidthScale & 0x07) << 4)) };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetAsciiTextFontType(unsigned char nFontType)
    {
        unsigned char cmd[] = { 0x1b, 0x4d, nFontType };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetTextBold(unsigned char nBold)
    {
        unsigned char cmd[] = { 0x1b, 0x45, nBold };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetAsciiTextUnderline(unsigned char nUnderline)
    {
        unsigned char cmd[] = { 0x1b, 0x2d, nUnderline };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetKanjiTextUnderline(unsigned char nUnderline)
    {
        unsigned char cmd[] = { 0x1c, 0x2d, nUnderline };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetTextUpsideDown(unsigned char nUpsideDown)
    {
        unsigned char cmd[] = { 0x1b, 0x7b, nUpsideDown };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetTextWhiteOnBlack(unsigned char nWhiteOnBlack)
    {
        unsigned char cmd[] = { 0x1d, 0x42, nWhiteOnBlack };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetTextRotate(unsigned char nRotate)
    {
        unsigned char cmd[] = { 0x1b, 0x56, nRotate };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetTextLineHeightToDefault()
    {
        unsigned char cmd[] = { 0x1b, 0x32 };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetTextLineHeight(unsigned char nLineHeight)
    {
        unsigned char cmd[] = { 0x1b, 0x33, nLineHeight };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetAsciiTextCharRightSpacing(unsigned char nSpacing)
    {
        unsigned char cmd[] = { 0x1b, 0x20, nSpacing };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetKanjiTextCharSpacing(unsigned char nLeftSpacing, unsigned char nRightSpacing)
    {
        unsigned char cmd[] = { 0x1c, 0x53, nLeftSpacing, nRightSpacing };
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }

    bool POS_PrintBarcode(unsigned char nType, const char *str)
    {
        bool result = false;

        size_t str_len = strlen(str);
        unsigned char GS_k_m_n_[] = { 0x1d, 0x6b, nType, (unsigned char)str_len};
        size_t datalen =
                sizeof(GS_k_m_n_) +
                str_len;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            size_t offset = 0;
            memcpy(&data[offset], GS_k_m_n_, sizeof(GS_k_m_n_)); offset += sizeof(GS_k_m_n_);
            memcpy(&data[offset], str, str_len); offset += str_len;
            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);
            free(data);
        }

        return result;
    }
    bool POS_SetBarcodeUnitWidth(unsigned char nUnitWidth)
    {
        unsigned char cmd[] = {0x1d, 0x77, nUnitWidth};
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetBarcodeHeight(unsigned char nHeight)
    {
        unsigned char cmd[] = {0x1d, 0x68, nHeight};
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetBarcodeReadableTextFontType(unsigned char nFontType)
    {
        unsigned char cmd[] = {0x1d, 0x66, nFontType};
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }
    bool POS_SetBarcodeReadableTextPosition(unsigned char nTextPosition)
    {
        unsigned char cmd[] = {0x1d, 0x48, nTextPosition};
        return Write(cmd, sizeof(cmd), default_short_timeout_ms) == sizeof(cmd);
    }

    // nECCLevel 1 2 3 4
    bool POS_PrintQRCode(unsigned char nVersion, unsigned char nECCLevel, const char *str)
    {
        bool result = false;

        size_t str_len = strlen(str);
        unsigned char GS_k_m_v_r_nL_nH[] = {0x1d, 0x6b, 0x61, nVersion, nECCLevel, (unsigned char)(str_len), (unsigned char)(str_len >> 8)};
        size_t datalen =
                sizeof(GS_k_m_v_r_nL_nH) +
                str_len;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            size_t offset = 0;
            memcpy(&data[offset], GS_k_m_v_r_nL_nH, sizeof(GS_k_m_v_r_nL_nH)); offset += sizeof(GS_k_m_v_r_nL_nH);
            memcpy(&data[offset], str, str_len); offset += str_len;
            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);
            free(data);
        }

        return result;
    }

    // nECCLevel 1 2 3 4
    bool POS_EPSON_PrintQRCode(unsigned char nUnitWidth, unsigned char nECCLevel, const char *str)
    {
        bool result = false;

        unsigned char GS_leftbracket_k_pL_pH_cn_67_n[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 0x31, 0x43, nUnitWidth };
        unsigned char GS_leftbracket_k_pL_pH_cn_69_n[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 0x31, 0x45, (unsigned char)(47 + nECCLevel) };
        size_t str_len = strlen(str);
        unsigned char GS_leftbracket_k_pL_pH_cn_80_m_d1_dk[] = {
            0x1d, 0x28, 0x6b, (unsigned char)(str_len + 3), (unsigned char)((str_len + 3) >> 8), 0x31, 0x50, 0x30 };
        unsigned char GS_leftbracket_k_pL_pH_cn_fn_m[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 0x31, 0x51, 0x30 };
        size_t datalen =
                sizeof(GS_leftbracket_k_pL_pH_cn_67_n) +
                sizeof(GS_leftbracket_k_pL_pH_cn_69_n) +
                sizeof(GS_leftbracket_k_pL_pH_cn_80_m_d1_dk) +
                str_len +
                sizeof(GS_leftbracket_k_pL_pH_cn_fn_m);
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            size_t offset = 0;
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_cn_67_n, sizeof(GS_leftbracket_k_pL_pH_cn_67_n)); offset += sizeof(GS_leftbracket_k_pL_pH_cn_67_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_cn_69_n, sizeof(GS_leftbracket_k_pL_pH_cn_69_n)); offset += sizeof(GS_leftbracket_k_pL_pH_cn_69_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_cn_80_m_d1_dk, sizeof(GS_leftbracket_k_pL_pH_cn_80_m_d1_dk)); offset += sizeof(GS_leftbracket_k_pL_pH_cn_80_m_d1_dk);
            memcpy(&data[offset], str, str_len); offset += str_len;
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_cn_fn_m, sizeof(GS_leftbracket_k_pL_pH_cn_fn_m)); offset += sizeof(GS_leftbracket_k_pL_pH_cn_fn_m);
            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);
            free(data);
        }

        return result;
    }

    bool POS_PrintDoubleQRcode(
            const char *QR1Data, unsigned short QR1Position, unsigned char QR1ECCLevel, unsigned char QR1Version,
            const char *QR2Data, unsigned short QR2Position, unsigned char QR2ECCLevel, unsigned char QR2Version,
            unsigned char ModuleSize)
    {
        bool result = false;

        unsigned char US_Q_m_n[] = { 0x1f, 0x51, 0x02, ModuleSize };
        size_t QR1DataLen = strlen(QR1Data);
        unsigned char QR1_head[] = {
            (unsigned char)(QR1Position >> 8), (unsigned char)(QR1Position),
            (unsigned char)(QR1DataLen >> 8), (unsigned char)(QR1DataLen),
            QR1ECCLevel, QR1Version };

        size_t QR2DataLen = strlen(QR2Data);
        unsigned char QR2_head[] = {
            (unsigned char)(QR2Position >> 8), (unsigned char)(QR2Position),
            (unsigned char)(QR2DataLen >> 8), (unsigned char)(QR2DataLen),
            QR2ECCLevel, QR2Version };

        size_t datalen =
                sizeof(US_Q_m_n) +
                sizeof(QR1_head) +
                QR1DataLen +
                sizeof(QR2_head) +
                QR2DataLen;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            size_t offset = 0;
            memcpy(&data[offset], US_Q_m_n, sizeof(US_Q_m_n)); offset += sizeof(US_Q_m_n);
            memcpy(&data[offset], QR1_head, sizeof(QR1_head)); offset += sizeof(QR1_head);
            memcpy(&data[offset], QR1Data, QR1DataLen); offset += QR1DataLen;
            memcpy(&data[offset], QR2_head, sizeof(QR2_head)); offset += sizeof(QR2_head);
            memcpy(&data[offset], QR2Data, QR2DataLen); offset += QR2DataLen;
            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);
            free(data);
        }

        return result;
    }

    bool POS_EPSON_PrintPDF417(
            unsigned char columnCount, unsigned char rowCount, unsigned char unitWidth, unsigned char rowHeight,
            unsigned char nECCLevel, unsigned char dataProcessingMode, const char *str)
    {
        bool result = false;

        unsigned char GS_leftbracket_k_pL_pH_48_65_n[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 48, 65, columnCount };
        unsigned char GS_leftbracket_k_pL_pH_48_66_n[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 48, 66, rowCount };
        unsigned char GS_leftbracket_k_pL_pH_48_67_n[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 48, 67, unitWidth };
        unsigned char GS_leftbracket_k_pL_pH_48_68_n[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 48, 68, rowHeight };
        unsigned char GS_leftbracket_k_pL_pH_48_69_m_n[] = {
            0x1d, 0x28, 0x6b, 0x04, 0x00, 48, 69, 48, (unsigned char)(48+nECCLevel) };
        unsigned char GS_leftbracket_k_pL_pH_48_70_m[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 48, 70, dataProcessingMode };
        size_t str_len = strlen(str);
        unsigned char GS_leftbracket_k_pL_pH_48_80_m_d1_dk[] = {
            0x1d, 0x28, 0x6b, (unsigned char)(str_len + 3), (unsigned char)((str_len + 3) >> 8), 48, 80, 48 };
        unsigned char GS_leftbracket_k_pL_pH_48_81_m[] = {
            0x1d, 0x28, 0x6b, 0x03, 0x00, 48, 81, 48 };

        size_t datalen =
                sizeof(GS_leftbracket_k_pL_pH_48_65_n) +
                sizeof(GS_leftbracket_k_pL_pH_48_66_n) +
                sizeof(GS_leftbracket_k_pL_pH_48_67_n) +
                sizeof(GS_leftbracket_k_pL_pH_48_68_n) +
                sizeof(GS_leftbracket_k_pL_pH_48_69_m_n) +
                sizeof(GS_leftbracket_k_pL_pH_48_70_m) +
                sizeof(GS_leftbracket_k_pL_pH_48_80_m_d1_dk) +
                str_len +
                sizeof(GS_leftbracket_k_pL_pH_48_81_m);
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            size_t offset = 0;
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_65_n, sizeof(GS_leftbracket_k_pL_pH_48_65_n)); offset += sizeof(GS_leftbracket_k_pL_pH_48_65_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_66_n, sizeof(GS_leftbracket_k_pL_pH_48_66_n)); offset += sizeof(GS_leftbracket_k_pL_pH_48_66_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_67_n, sizeof(GS_leftbracket_k_pL_pH_48_67_n)); offset += sizeof(GS_leftbracket_k_pL_pH_48_67_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_68_n, sizeof(GS_leftbracket_k_pL_pH_48_68_n)); offset += sizeof(GS_leftbracket_k_pL_pH_48_68_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_69_m_n, sizeof(GS_leftbracket_k_pL_pH_48_69_m_n)); offset += sizeof(GS_leftbracket_k_pL_pH_48_69_m_n);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_70_m, sizeof(GS_leftbracket_k_pL_pH_48_70_m)); offset += sizeof(GS_leftbracket_k_pL_pH_48_70_m);
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_80_m_d1_dk, sizeof(GS_leftbracket_k_pL_pH_48_80_m_d1_dk)); offset += sizeof(GS_leftbracket_k_pL_pH_48_80_m_d1_dk);
            memcpy(&data[offset], str, str_len); offset += str_len;
            memcpy(&data[offset], GS_leftbracket_k_pL_pH_48_81_m, sizeof(GS_leftbracket_k_pL_pH_48_81_m)); offset += sizeof(GS_leftbracket_k_pL_pH_48_81_m);
            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);
            free(data);
        }

        return result;
    }

    bool POS_SetUserCharacterPattern(char ch, unsigned char vBytes, unsigned char hDots, unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 6 + vBytes * hDots;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1b;
            cmd[1] = 0x26;
            cmd[2] = vBytes;
            cmd[3] = (unsigned char)ch;
            cmd[4] = (unsigned char)ch;
            cmd[5] = hDots;
            memcpy(&cmd[6], data, vBytes * hDots);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }
    bool POS_ClearUserCharacterPattern(char ch)
    {
        unsigned char data[] = {0x1b, 0x3f, (unsigned char)ch};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetUserCharacterEnabled(bool enable)
    {
        unsigned char data[] = {0x1b, 0x25, (unsigned char)(enable ? 1 : 0)};
        return Write(data, sizeof(data), default_short_timeout_ms) == (int)sizeof(data);
    }
    bool POS_SetUserKanjiPattern(unsigned char c1, unsigned char c2, unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 4 + 72;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1c;
            cmd[1] = 0x32;
            cmd[2] = c1;
            cmd[3] = c2;
            memcpy(&cmd[4], data, 72);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }

    bool POS_PrintRasterImage(unsigned short width_stride, unsigned short height, unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 8 + width_stride * height;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1d;
            cmd[1] = 0x76;
            cmd[2] = 0x30;
            cmd[3] = 0x00;
            cmd[4] = (unsigned char)(width_stride);
            cmd[5] = (unsigned char)(width_stride >> 8);
            cmd[6] = (unsigned char)(height);
            cmd[7] = (unsigned char)(height >> 8);
            memcpy(&cmd[8], data, width_stride * height);
            result = (Write(cmd, cmdlen, default_long_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }

    bool POS_FillDotImageToCurrentLine(unsigned short hDots, unsigned char *data)
    {
        bool result = false;

        size_t vBytes = 3;
        size_t cmdlen = 5 + vBytes * hDots;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1b;
            cmd[1] = 0x2a;
            cmd[2] = 33;
            cmd[3] = (unsigned char)(hDots);
            cmd[4] = (unsigned char)(hDots >> 8);
            memcpy(&cmd[5], data, vBytes * hDots);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }

    bool POS_PrintNVImage(unsigned char no)
    {
        unsigned char cmd[] = {0x1c, 0x70, no, 0x00};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }

    bool POS_PrintRAMImage()
    {
        unsigned char cmd[] = {0x1d, 0x2f, 0x00};
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetRAMImage(unsigned char hDotsDiv8, unsigned char vDotsDiv8, const unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 4 + hDotsDiv8 * vDotsDiv8 * 8;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1d;
            cmd[1] = 0x2a;
            cmd[2] = hDotsDiv8;
            cmd[3] = vDotsDiv8;
            memcpy(&cmd[4], data, hDotsDiv8 * vDotsDiv8 * 8);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }

    bool POS_PrintMsbRasterImageUseDC2VCmd(size_t hBytes, unsigned short vDots, unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 4 + hBytes * vDots;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x12;
            cmd[1] = 0x56;
            cmd[2] = (unsigned char)vDots;
            cmd[3] = (unsigned char)(vDots >> 8);
            memcpy(&cmd[4], data, hBytes * vDots);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }
    bool POS_PrintLsbRasterImageUseDC2vCmd(size_t hBytes, unsigned short vDots, unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 4 + hBytes * vDots;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x12;
            cmd[1] = 0x76;
            cmd[2] = (unsigned char)vDots;
            cmd[3] = (unsigned char)(vDots >> 8);
            memcpy(&cmd[4], data, hBytes * vDots);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }
    bool POS_PrintBitRasterImageUseDC2Star(unsigned char hBytes, unsigned char vDots, unsigned char *data)
    {
        bool result = false;

        size_t cmdlen = 4 + hBytes * vDots;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x12;
            cmd[1] = 0x2a;
            cmd[2] = vDots;
            cmd[3] = hBytes;
            memcpy(&cmd[4], data, hBytes * vDots);
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }

    bool POS_PrintHorizontalLineSpecifyThickness(unsigned short nLineStartPosition, unsigned short nLineEndPosition, size_t nLineThickness)
    {
        bool result = false;

        if (nLineThickness > 0) {
            size_t cmdlen = 7 * nLineThickness;
            unsigned char *cmd = (unsigned char *)malloc(cmdlen);
            if (cmd) {
                for (size_t i = 0; i < nLineThickness; ++i) {
                    cmd[i*7+0] = 0x1d;
                    cmd[i*7+1] = 0x27;
                    cmd[i*7+2] = 0x01;
                    cmd[i*7+3] = (unsigned char)nLineStartPosition;
                    cmd[i*7+4] = (unsigned char)(nLineStartPosition >> 8);
                    cmd[i*7+5] = (unsigned char)nLineEndPosition;
                    cmd[i*7+6] = (unsigned char)(nLineEndPosition >> 8);
                }
                result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
                free(cmd);
            }
        }

        return result;
    }
    bool POS_PrintMultipleHorizontalLines(unsigned char nLineCount, unsigned short *pLineStartPosition, unsigned short *pLineEndPosition)
    {
        bool result = false;

        size_t cmdlen = 3 + nLineCount * 4;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1d;
            cmd[1] = 0x27;
            cmd[2] = nLineCount;
            for (size_t i = 0; i < nLineCount; ++i) {
                cmd[3+i*4+0] = (unsigned char)pLineStartPosition[i];
                cmd[3+i*4+1] = (unsigned char)(pLineStartPosition[i] >> 8);
                cmd[3+i*4+2] = (unsigned char)pLineEndPosition[i];
                cmd[3+i*4+3] = (unsigned char)(pLineEndPosition[i] >> 8);
            }
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }
    bool POS_PrintMultipleHorizontalLines(unsigned char nLineCount, int *pLineStartPosition, int *pLineEndPosition)
    {
        bool result = false;

        size_t cmdlen = 3 + nLineCount * 4;
        unsigned char *cmd = (unsigned char *)malloc(cmdlen);
        if (cmd) {
            cmd[0] = 0x1d;
            cmd[1] = 0x27;
            cmd[2] = nLineCount;
            for (size_t i = 0; i < nLineCount; ++i) {
                cmd[3+i*4+0] = (unsigned char)pLineStartPosition[i];
                cmd[3+i*4+1] = (unsigned char)(pLineStartPosition[i] >> 8);
                cmd[3+i*4+2] = (unsigned char)pLineEndPosition[i];
                cmd[3+i*4+3] = (unsigned char)(pLineEndPosition[i] >> 8);
            }
            result = (Write(cmd, cmdlen, default_short_timeout_ms) == (int)cmdlen);
            free(cmd);
        }

        return result;
    }

    bool POS_DrawRect(unsigned short left, unsigned short top, unsigned short right, unsigned short bottom, unsigned char color)
    {
        unsigned char cmd[] = { 0x1F, 0x28, 0x52, 0x0a, 0x00, (unsigned char)(left >> 8), (unsigned char)(left), (unsigned char)(top >> 8), (unsigned char)(top), (unsigned char)(right >> 8), (unsigned char)(right), (unsigned char)(bottom >> 8), (unsigned char)(bottom), color, 0x01 };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }

    bool POS_EnableBlackMarkMode()
    {
        unsigned char cmd[] = { 0x1F, 0x1B, 0x1F, 0x80, 0x04, 0x05, 0x06, 0x44 };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_DisableBlackMarkMode()
    {
        unsigned char cmd[] = { 0x1F, 0x1B, 0x1F, 0x80, 0x04, 0x05, 0x06, 0x66 };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetBlackMarkMaxFindLength(unsigned short maxFindLength)
    {
        unsigned char cmd[] = { 0x1F, 0x1B, 0x1F, 0x81, 0x04, 0x05, 0x06, (unsigned char)maxFindLength, (unsigned char)(maxFindLength >> 8) };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetBlackMarkHeight(unsigned short blackMarkHeight)
    {
        unsigned char cmd[] = { 0x1F, 0x1B, 0x1F, 0x82, 0x04, 0x05, 0x06, (unsigned char)blackMarkHeight, (unsigned char)(blackMarkHeight >> 8) };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetBlackMarkMaxFindLengthRuntime(unsigned short maxFindLength)
    {
        unsigned char cmd[] = { 0x12, 0x6d, 0x00, (unsigned char)maxFindLength, (unsigned char)(maxFindLength >> 8) };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_FindBlackMark()
    {
        unsigned char cmd[] = { 0x1d, 0x0c };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetBlackMarkPaperPrintPositionBase(unsigned char direction, unsigned short position)
    {
        unsigned char cmd[] = { 0x1d, 0x28, 0x46, 0x04, 0x00, 1, direction, (unsigned char)(position), (unsigned char)((position) >> 8) };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetBlackMarkPaperPrintPosition(int position)
    {
        if (position < 0)
            return POS_SetBlackMarkPaperPrintPositionBase(1, (unsigned short)(-position));
        else
            return POS_SetBlackMarkPaperPrintPositionBase(0, (unsigned short)(position));
    }
    bool POS_SetBlackMarkPaperCutPositionBase(unsigned char direction, unsigned short position)
    {
        unsigned char cmd[] = { 0x1d, 0x28, 0x46, 0x04, 0x00, 2, direction, (unsigned char)(position), (unsigned char)((position) >> 8) };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_SetBlackMarkPaperCutPosition(int position)
    {
        if (position < 0)
            return POS_SetBlackMarkPaperCutPositionBase(1, (unsigned short)(-position));
        else
            return POS_SetBlackMarkPaperCutPositionBase(0, (unsigned short)(position));
    }

    bool POS_FullCutBlackMarkPaper()
    {
        unsigned char cmd[] = { 0x1d, 0x56, 0x00 };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
    bool POS_HalfCutBlackMarkPaper()
    {
        unsigned char cmd[] = { 0x1d, 0x56, 0x01 };
        return (Write(cmd, sizeof(cmd), default_short_timeout_ms) == (int)sizeof(cmd));
    }
};

class NZLabelPrinter : public NZPrinter
{
public:
    bool PageBegin(unsigned short startx, unsigned short starty, unsigned short width, unsigned short height, unsigned char rotate)
    {
        unsigned char data[12];
        size_t datalen = sizeof(data);

        data[0] = 0x1A;
        data[1] = 0x5B;
        data[2] = 0x01;

        data[3] = (unsigned char) (startx);
        data[4] = (unsigned char) (startx >> 8);
        data[5] = (unsigned char) (starty);
        data[6] = (unsigned char) (starty >> 8);

        data[7] = (unsigned char) (width);
        data[8] = (unsigned char) (width >> 8);
        data[9] = (unsigned char) (height);
        data[10] = (unsigned char) (height >> 8);

        data[11] = rotate;

        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool PageEnd()
    {
        unsigned char data[] = { 0x1A, 0x5D, 0x00 };
        size_t datalen = sizeof(data);
        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool PagePrint(unsigned char num)
    {
        unsigned char data[] = { 0x1A, 0x4F, 0x01, num };
        size_t datalen = sizeof(data);
        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }

    bool FeedLabel()
    {
        unsigned char data[] = { 0x1A, 0x0C, 0x00 };
        size_t datalen = sizeof(data);
        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool CalibrateLabel()
    {
        unsigned char data[] = { 0x1f, 0x63 };
        size_t datalen = sizeof(data);
        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool EnableLabelMode()
    {
        unsigned char data[] = { 0x1f, 0x28, 0x4c, 0x01, 0x00, 0x31 };
        size_t datalen = sizeof(data);
        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool DisableLabelMode()
    {
        unsigned char data[] = { 0x1f, 0x28, 0x4c, 0x01, 0x00, 0x00 };
        size_t datalen = sizeof(data);
        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }

    bool DrawLine(unsigned short startx, unsigned short starty, unsigned short endx, unsigned short endy, unsigned short width, unsigned char color)
    {
        unsigned char data[14];
        size_t datalen = sizeof(data);

        data[0] = 0x1A;
        data[1] = 0x5C;
        data[2] = 0x01;

        data[3] = (unsigned char) (startx & 0xFF);
        data[4] = (unsigned char) ((startx >> 8) & 0xFF);
        data[5] = (unsigned char) (starty & 0xFF);
        data[6] = (unsigned char) ((starty >> 8) & 0xFF);

        data[7] = (unsigned char) (endx & 0xFF);
        data[8] = (unsigned char) ((endx >> 8) & 0xFF);
        data[9] = (unsigned char) (endy & 0xFF);
        data[10] = (unsigned char) ((endy >> 8) & 0xFF);

        data[11] = (unsigned char) (width & 0xFF);
        data[12] = (unsigned char) ((width >> 8) & 0xFF);

        data[13] = (unsigned char) (color & 0xFF);

        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool DrawBox(unsigned short left, unsigned short top, unsigned short right, unsigned short bottom, unsigned short borderwidth, unsigned char bordercolor)
    {
        unsigned char data[14];
        size_t datalen = sizeof(data);

        data[0] = 0x1A;
        data[1] = 0x26;
        data[2] = 0x01;

        data[3] = (unsigned char) (left & 0xFF);
        data[4] = (unsigned char) ((left >> 8) & 0xFF);
        data[5] = (unsigned char) (top & 0xFF);
        data[6] = (unsigned char) ((top >> 8) & 0xFF);

        data[7] = (unsigned char) (right & 0xFF);
        data[8] = (unsigned char) ((right >> 8) & 0xFF);
        data[9] = (unsigned char) (bottom & 0xFF);
        data[10] = (unsigned char) ((bottom >> 8) & 0xFF);

        data[11] = (unsigned char) (borderwidth & 0xFF);
        data[12] = (unsigned char) ((borderwidth >> 8) & 0xFF);

        data[13] = (unsigned char) (bordercolor & 0xFF);

        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }
    bool DrawRectangel(unsigned short left, unsigned short top, unsigned short right, unsigned short bottom, unsigned char color)
    {
        unsigned char data[12];
        size_t datalen = sizeof(data);

        data[0] = 0x1A;
        data[1] = 0x2A;
        data[2] = 0x00;

        data[3] = (unsigned char) (left & 0xFF);
        data[4] = (unsigned char) ((left >> 8) & 0xFF);
        data[5] = (unsigned char) (top & 0xFF);
        data[6] = (unsigned char) ((top >> 8) & 0xFF);

        data[7] = (unsigned char) (right & 0xFF);
        data[8] = (unsigned char) ((right >> 8) & 0xFF);
        data[9] = (unsigned char) (bottom & 0xFF);
        data[10] = (unsigned char) ((bottom >> 8) & 0xFF);

        data[11] = (unsigned char) (color & 0xFF);

        return Write(data, datalen, default_short_timeout_ms) == (int)datalen;
    }

    bool DrawString(unsigned short startx, unsigned short starty, unsigned short font, unsigned short style, const char *str)
    {
        bool result = false;

        size_t strtotallen = strlen(str) + 1;
        size_t datalen = 11 + strtotallen;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            data[0] = 0x1A;
            data[1] = 0x54;
            data[2] = 0x01;

            data[3] = (unsigned char) (startx & 0xFF);
            data[4] = (unsigned char) ((startx >> 8) & 0xFF);
            data[5] = (unsigned char) (starty & 0xFF);
            data[6] = (unsigned char) ((starty >> 8) & 0xFF);

            data[7] = (unsigned char) (font & 0xFF);
            data[8] = (unsigned char) ((font >> 8) & 0xFF);
            data[9] = (unsigned char) (style & 0xFF);
            data[10] = (unsigned char) ((style >> 8) & 0xFF);

            memcpy(&data[11], str, strtotallen);

            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);

            free(data);
        }

        return result;
    }

    bool DrawBarcode(unsigned short startx, unsigned short starty, unsigned char type, unsigned char height, unsigned char unitwidth, unsigned char rotate, const char *str)
    {
        bool result = false;

        size_t strtotallen = strlen(str) + 1;
        size_t datalen = 11 + strtotallen;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            data[0] = 0x1A;
            data[1] = 0x30;
            data[2] = 0x00;

            data[3] = (unsigned char) (startx & 0xFF);
            data[4] = (unsigned char) ((startx >> 8) & 0xFF);
            data[5] = (unsigned char) (starty & 0xFF);
            data[6] = (unsigned char) ((starty >> 8) & 0xFF);

            data[7] = (unsigned char) (type & 0xFF);
            data[8] = (unsigned char) (height & 0xFF);
            data[9] = (unsigned char) (unitwidth & 0xFF);
            data[10] = (unsigned char) (rotate & 0xFF);

            memcpy(&data[11], str, strtotallen);

            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);

            free(data);
        }

        return result;
    }

    bool DrawQRCode(unsigned short startx, unsigned short starty, unsigned char version, unsigned char ecc, unsigned char unitwidth, unsigned char rotate, const char *str)
    {
        bool result = false;

        size_t strtotallen = strlen(str) + 1;
        size_t datalen = 11 + strtotallen;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            data[0] = 0x1A;
            data[1] = 0x31;
            data[2] = 0x00;

            data[3] = (unsigned char) (version & 0xFF);
            data[4] = (unsigned char) (ecc & 0xFF);

            data[5] = (unsigned char) (startx & 0xFF);
            data[6] = (unsigned char) ((startx >> 8) & 0xFF);
            data[7] = (unsigned char) (starty & 0xFF);
            data[8] = (unsigned char) ((starty >> 8) & 0xFF);

            data[9] = (unsigned char) (unitwidth & 0xFF);
            data[10] = (unsigned char) (rotate & 0xFF);

            memcpy(&data[11], str, strtotallen);

            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);

            free(data);
        }

        return result;
    }

    bool DrawPDF417(unsigned short startx, unsigned short starty, unsigned char colnum, unsigned char lwratio, unsigned char ecc, unsigned char unitwidth, unsigned char rotate, const char *str)
    {
        bool result = false;

        size_t strtotallen = strlen(str) + 1;
        size_t datalen = 12 + strtotallen;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            data[0] = 0x1A;
            data[1] = 0x31;
            data[2] = 0x01;

            data[3] = (unsigned char) (colnum & 0xFF);
            data[4] = (unsigned char) (ecc & 0xFF);
            data[5] = (unsigned char) (lwratio & 0xFF);

            data[6] = (unsigned char) (startx & 0xFF);
            data[7] = (unsigned char) ((startx >> 8) & 0xFF);
            data[8] = (unsigned char) (starty & 0xFF);
            data[9] = (unsigned char) ((starty >> 8) & 0xFF);

            data[10] = (unsigned char) (unitwidth & 0xFF);
            data[11] = (unsigned char) (rotate & 0xFF);

            memcpy(&data[12], str, strtotallen);

            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);

            free(data);
        }

        return result;
    }

    bool DrawRasterImage(unsigned short startx, unsigned short starty, unsigned short width_stride, unsigned short width, unsigned short height, unsigned short style, const unsigned char *img)
    {
        bool result = false;

        size_t img_data_len = width_stride * height;
        width = width_stride * 8;

        size_t datalen = 13 + img_data_len;
        unsigned char *data = (unsigned char *)malloc(datalen);
        if (data) {
            data[0] = 0x1A;
            data[1] = 0x21;
            data[2] = 0x01;

            data[3] = (unsigned char) (startx & 0xFF);
            data[4] = (unsigned char) ((startx >> 8) & 0xFF);
            data[5] = (unsigned char) (starty & 0xFF);
            data[6] = (unsigned char) ((starty >> 8) & 0xFF);

            data[7] = (unsigned char) (width & 0xFF);
            data[8] = (unsigned char) ((width >> 8) & 0xFF);
            data[9] = (unsigned char) (height & 0xFF);
            data[10] = (unsigned char) ((height >> 8) & 0xFF);

            data[11] = (unsigned char) (style & 0xFF);
            data[12] = (unsigned char) ((style >> 8) & 0xFF);

            memcpy(&data[13], img, img_data_len);

            result = (Write(data, datalen, default_short_timeout_ms) == (int)datalen);

            free(data);
        }

        return result;
    }
};

#endif // NZPRINTER_BASE_H
