package printer;

import ui.GMainFrame;

public class PrinterLibs {

    public static int HORIZONTALALIGNMENT_LEFT		= -1;
    public static int HORIZONTALALIGNMENT_CENTER	= -2;
    public static int HORIZONTALALIGNMENT_RIGHT		= -3;

    public static int POS_BARCODE_TYPE_UPCA			= 0x41;
    public static int POS_BARCODE_TYPE_UPCE			= 0x42;
    public static int POS_BARCODE_TYPE_EAN13		= 0x43;
    public static int POS_BARCODE_TYPE_EAN8			= 0x44;
    public static int POS_BARCODE_TYPE_CODE39		= 0x45;
    public static int POS_BARCODE_TYPE_ITF			= 0x46;
    public static int POS_BARCODE_TYPE_CODABAR		= 0x47;
    public static int POS_BARCODE_TYPE_CODE93		= 0x48;
    public static int POS_BARCODE_TYPE_CODE128		= 0x49;

    public static int ENCODING_GBK					= 0;
    public static int ENCODING_UTF8					= 1;
    public static int ENCODING_BIG5					= 3;
    public static int ENCODING_SHIFTJIS				= 4;
    public static int ENCODING_EUCKR				= 5;

    public static int BINARYALGORITHM_DITHER		= 0;
    public static int BINARYALGORITHM_THRESHOLD		= 1;

    public static int COMPRESSMETHOD_NONE			= 0;
    public static int COMPRESSMETHOD_COMPRESS		= 1;

    public static int PARITY_NONE					= 0;
    public static int PARITY_ODD					= 1;
    public static int PARITY_EVEN					= 2;
    public static int PARITY_MARK					= 3;
    public static int PARITY_SPACE					= 4;

	static
	{
		try
		{
			System.load(GMainFrame.rootPath + "libprinter.so");
		}
		catch (UnsatisfiedLinkError e) {
			System.out.println("Cannot load libprinter.so:\n" + e.toString());
		}
	}
	
	public static native boolean Port_OpenCom(String pName, int dwBaudrate, int dwParity);
	
	public static native boolean Port_OpenTcp(String szIp, int nPort);
	
	public static native boolean Port_OpenUsb(String pName);

    public static native boolean Port_OpenLpt(String pName);

    public static native int Port_Write(byte[] buffer, int count, long timeout);
    
    public static native int Port_Read(byte[] buffer, int count, long timeout);
    
    public static native void Port_Close();
    
    public static native void Port_EnumCom(byte[] pBuf, int cbBuf, PrinterRefInteger pcbNeeded, PrinterRefInteger pcnReturned);

    public static native void Port_EnumLpt(byte[] pBuf, int cbBuf, PrinterRefInteger pcbNeeded, PrinterRefInteger pcnReturned);

    public static native void Port_EnumUSB(byte[] pBuf, int cbBuf, PrinterRefInteger pcbNeeded, PrinterRefInteger pcnReturned);

    // 打印文本
    public static native boolean POS_TextOut(String pszString, int x, int nWidthScale, int nHeightScale, int nFontType, int nFontStyle, int nEncoding);

    // 打印条码
    public static native boolean POS_SetBarcode(String pszBarcodeContent, int x, int nBarcodeUnitWidth, int nBarcodeHeight, int nHriFontType, int nHriFontPosition, int nBarcodeType);

    // 打印QR码
    public static native boolean POS_SetQRCode(String pszContent, int x, int nQRCodeUnitWidth, int nVersion, int nEcLevel);

    // 打印bmp图片
    public static native boolean POS_PrintPicture(String szFileName, int x, int dstw, int dsth, int nBinaryAlgorithm, int nCompressMethod);

    public static native boolean POS_SelfTest();

    // 进纸
    public static native boolean POS_FeedLine();

    public static native boolean POS_FeedNLine(int nLine);

    public static native boolean POS_FeedNDot(int nDot);

    // 查询打印机状态
    public static native boolean POS_QueryStatus(int type, PrinterRefLong status, char timeout);

    // 实时查询打印机状态
    public static native boolean POS_RTQueryStatus(int type, PrinterRefInteger status, char timeout);

    // 单据打印结果查询
    public static native boolean POS_TicketSucceed(int dwSendIndex, long timeout);

    // 设置
    public static native boolean POS_SetMotionUnit(int nHorizontal, int nVertical);

    public static native boolean POS_SetLineHeight(int nDistance);

    public static native boolean POS_SetRightSpacing(int nDistance);
    
    public static native boolean POS_Reset();

    // 开钱箱
    public static native boolean POS_KickOutDrawer(int nID, int nHighLevelTime, int nLowLevelTime);

    // 进纸到切刀位置并切纸
    public static native boolean POS_FeedAndCutPaper();

    // 直接全切
    public static native boolean POS_FullCutPaper();

    // 直接半切
    public static native boolean POS_HalfCutPaper();

    public static native boolean POS_Beep(int nBeepCount, int nBeepMillis);
    
    public static void LibTest()
	{
		String str = "/dev/usb/lp1";
//		PrinterLibs.POS_FeedNLine(2);
//		PrinterLibs.POS_FeedLine();
		PrinterLibs.POS_TextOut("\r\n" + "Device name:" + str + "\r\n", PrinterLibs.HORIZONTALALIGNMENT_CENTER, 0, 0, 0, 0, PrinterLibs.ENCODING_UTF8);

		PrinterLibs.POS_TextOut("libprinter测试页\r\n", PrinterLibs.HORIZONTALALIGNMENT_LEFT, 0, 0, 0, 0, PrinterLibs.ENCODING_UTF8);
		PrinterLibs.POS_TextOut("問題がありません。理解しました。\r\n", PrinterLibs.HORIZONTALALIGNMENT_LEFT, 0, 0, 0, 0, PrinterLibs.ENCODING_UTF8);
		PrinterLibs.POS_TextOut("中文可以打印？\r\n", PrinterLibs.HORIZONTALALIGNMENT_LEFT, 0, 0, 0, 0, PrinterLibs.ENCODING_UTF8);

		PrinterLibs.POS_SetBarcode("No.123456", 0, 2, 50, 0, 2, PrinterLibs.POS_BARCODE_TYPE_CODE128);

        PrinterLibs.POS_SetQRCode("CaysnPrinter", 0, 4, 0, 2);
        
        PrinterLibs.POS_FeedAndCutPaper();
        PrinterLibs.POS_KickOutDrawer(0, 100, 100);
        PrinterLibs.POS_Beep(1, 5);
        
        PrinterLibs.POS_TicketSucceed(0, 5000);
	}
    
    public void usbTest()
    {	
    	PrinterRefInteger ref_pcbNeeded = new PrinterRefInteger();
    	PrinterRefInteger ref_pcnReturned = new PrinterRefInteger();
    	PrinterLibs.Port_EnumUSB(null, 0, ref_pcbNeeded, ref_pcnReturned);
    	int pcbNeeded = ref_pcbNeeded.getRefValue();
    	
    	System.out.println("usbTest() 11 :pcbNeeded=" + pcbNeeded);
        if (pcbNeeded > 0)
        {
            byte[] pBuf = new byte[pcbNeeded];
            if (pBuf != null)
            {
                PrinterLibs.Port_EnumUSB(pBuf, pcbNeeded, ref_pcbNeeded, ref_pcnReturned);

                System.out.println("usbTest() 22 :pcbNeeded=" + ref_pcbNeeded.getRefValue());
                String devList = new String(pBuf);
                System.out.println("usbTest():devList =[" + devList + "]");
                
                assert(pcbNeeded == ref_pcbNeeded.getRefValue());
                String[] device_name = devList.split("\0");
                for (String str : device_name) {
                	System.out.println("usbTest():str =[" + str + "]");
                	if (PrinterLibs.Port_OpenUsb(str))
                    {
                		System.out.println("usbTest():str =[" + str + "]" + "open OK");
                    	LibTest();

                        PrinterLibs.Port_Close();
                        return;
                    }
                }
            }
        }
    }
}
