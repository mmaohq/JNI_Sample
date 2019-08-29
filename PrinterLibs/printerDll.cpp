#include <iostream>
#include "pch.h"
#include "printerDll.h"
#include "printerlibs.h"

using namespace std;

NZComIOEnumeratorClass nzComIOEnumerator;
NZLptIOEnumeratorClass nzLptIOEnumerator;
NZUsbIOEnumeratorClass nzUsbIOEnumerator;
NZPrnIOEnumeratorClass nzPrnIOEnumerator;

NZMemoryIO mem;
NZFileIO file;
NZComIOClass com;
NZUsbIOClass usb;
NZLptIOClass lpt;
NZPrnIOClass prn;
NZTcpClientIOClass tcp;
NZUdpClientIOClass udp;
NZWebSocketClientIO ws;

NZPosPrinter pos;

//NetDiscoverThread netDiscoverThread;

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_OpenCom
 * Signature: (Ljava/lang/String;II)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_Port_1OpenCom
(JNIEnv* env, jclass, jstring port_name, jint baudrate, jint parity)
{
	jboolean isCopy;
	const char * com_port = env->GetStringUTFChars(port_name, &isCopy);

	const ComDataBits databits = ComDataBits_8;
	const ComStopBits stopbits = ComStopBits_One;
	const ComFlowControl flowCtrl = ComFlowControl_XonXoff;

	jboolean res = com.Open(com_port, baudrate, databits, (ComParity)parity, stopbits, flowCtrl) ? JNI_TRUE : JNI_FALSE;
	if (res == JNI_TRUE) {
		pos.SetIO(&com);
	}

	env->ReleaseStringUTFChars(port_name, com_port);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_OpenTcp
 * Signature: (Ljava/lang/String;I)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_Port_1OpenTcp
(JNIEnv* env, jclass, jstring ip_addr, jint port_number)
{
	jboolean isCopy;
    const char *ip_str = env->GetStringUTFChars(ip_addr, &isCopy);

    jboolean res = tcp.Open(0, ip_str, port_number, 5000) ? JNI_TRUE : JNI_FALSE;
	if (res == JNI_TRUE) {
		pos.SetIO(&tcp);
	}

	env->ReleaseStringUTFChars(ip_addr, ip_str);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_OpenUsb
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_Port_1OpenUsb
(JNIEnv* env, jclass, jstring port_name)
{
	jboolean isCopy;
	std::cout << "Port_OpenUsb jstring port:[" << port_name << "]" << endl;
	const char *port = env->GetStringUTFChars(port_name, &isCopy);
	std::cout << "Port_OpenUsb utf port:[" << port << "]" << endl;

	jboolean res = usb.Open(port) ? JNI_TRUE : JNI_FALSE;
	if (res == JNI_TRUE) {
		pos.SetIO(&usb);
	}

	env->ReleaseStringUTFChars(port_name, port);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_OpenLpt
 * Signature: (Ljava/lang/String;)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_Port_1OpenLpt
(JNIEnv* env, jclass, jstring port_name)
{
	jboolean isCopy;
	const char *port = env->GetStringUTFChars(port_name, &isCopy);

	jboolean res = lpt.Open(port) ? JNI_TRUE : JNI_FALSE;
	if (res == JNI_TRUE) {
		pos.SetIO(&lpt);
	}

	env->ReleaseStringUTFChars(port_name, port);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_Write
 * Signature: ([BCC)I
 */
JNIEXPORT jint JNICALL Java_printer_PrinterLibs_Port_1Write
(JNIEnv* env, jclass, jbyteArray jBuff, jchar jCnt, jchar jTimeout)
{
	char* pcBuff = NULL;
	
	jbyte* bBuff = env->GetByteArrayElements(jBuff, 0);
	int len = env->GetArrayLength(jBuff);

	pcBuff = new char[len + 1];
	memset(pcBuff, 0x0, len + 1);
	memcpy(pcBuff, bBuff, len);
	pcBuff[len] = '\0';

	unsigned char* ucbuf = (unsigned char*)pcBuff;

	pos.Write(ucbuf, (size_t)jCnt, (long)jTimeout);

	env->ReleaseByteArrayElements(jBuff, bBuff, 0);
	return 0;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_Read
 * Signature: ([BCC)I
 */
JNIEXPORT jint JNICALL Java_printer_PrinterLibs_Port_1Read
(JNIEnv* env, jclass, jbyteArray jBuff, jchar jCnt, jchar jTimeout)
{
	jint res = 0;

	jbyte* bBuff = env->GetByteArrayElements(jBuff, 0);

	unsigned char* ucbuf = (unsigned char*)bBuff;
	res = pos.Read(ucbuf, jCnt, jTimeout);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_Close
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_printer_PrinterLibs_Port_1Close
(JNIEnv*, jclass)
{
	pos.Close();
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_EnumCom
 * Signature: ([BILprinter/PrinterRefInteger;Lprinter/PrinterRefInteger;)V
 */
JNIEXPORT void JNICALL Java_printer_PrinterLibs_Port_1EnumCom
(JNIEnv* env, jclass obj, jbyteArray byte_arr_buff, jint nBuff, jobject out_obj_needed, jobject out_obj_ret)
{
	jboolean isCopy = JNI_FALSE;

	size_t required_size;
	if (byte_arr_buff != NULL) {
		jbyte* jbyte_buff = env->GetByteArrayElements(byte_arr_buff, &isCopy);
		jsize jbuff_size = env->GetArrayLength(byte_arr_buff);

		size_t len = (int)nBuff;
		char* buff = new char[len];

		memset(buff, 0x0, len);
		if ((buff) != NULL) {
			nzComIOEnumerator.Enumerate(buff, len, &required_size);
			for (int j = 0; j < jbuff_size; j++) {
				jbyte_buff[j] = buff[j];
			}

			jclass obj_needed = env->GetObjectClass(out_obj_needed);
			jfieldID fid_needed = env->GetFieldID(obj_needed, "refValue", "I");
			env->SetIntField(out_obj_needed, fid_needed, required_size);
		}
		delete []buff;
		env->ReleaseByteArrayElements(byte_arr_buff, jbyte_buff, 0);
	}
	else {
		char* buff = NULL;

		nzComIOEnumerator.Enumerate(buff, nBuff, &required_size);

		jclass obj_needed = env->GetObjectClass(out_obj_needed);
		jfieldID fid_needed = env->GetFieldID(obj_needed, "refValue", "I");
		env->SetIntField(out_obj_needed, fid_needed, required_size);
	}

	jint jint_returned = required_size;
	jclass obj_returned = env->GetObjectClass(out_obj_ret);
	jfieldID fid_returned = env->GetFieldID(obj_returned, "refValue", "I");
	env->SetIntField(out_obj_ret, fid_returned, jint_returned);

	return;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_EnumLpt
 * Signature: ([BILprinter/PrinterRefInteger;Lprinter/PrinterRefInteger;)V
 */
JNIEXPORT void JNICALL Java_printer_PrinterLibs_Port_1EnumLpt
(JNIEnv* env, jclass obj, jbyteArray byte_arr_buff, jint nBuff, jobject out_obj_needed, jobject out_obj_ret)
{
	jboolean isCopy = JNI_FALSE;

	size_t required_size;
	if (byte_arr_buff != NULL) {
		jbyte* jbyte_buff = env->GetByteArrayElements(byte_arr_buff, &isCopy);
		jsize jbuff_size = env->GetArrayLength(byte_arr_buff);

		size_t len = (int)nBuff;
		char* buff = new char[len];

		memset(buff, 0x0, len);
		if ((buff) != NULL) {
			nzLptIOEnumerator.Enumerate(buff, len, &required_size);
			for (int j = 0; j < jbuff_size; j++) {
				jbyte_buff[j] = buff[j];
			}

			jclass obj_needed = env->GetObjectClass(out_obj_needed);
			jfieldID fid_needed = env->GetFieldID(obj_needed, "refValue", "I");
			env->SetIntField(out_obj_needed, fid_needed, required_size);
		}
		delete []buff;
		env->ReleaseByteArrayElements(byte_arr_buff, jbyte_buff, 0);
	}
	else {
		char* buff = NULL;

		nzLptIOEnumerator.Enumerate(buff, nBuff, &required_size);

		jclass obj_needed = env->GetObjectClass(out_obj_needed);
		jfieldID fid_needed = env->GetFieldID(obj_needed, "refValue", "I");
		env->SetIntField(out_obj_needed, fid_needed, required_size);
	}

	jint jint_returned = required_size;
	jclass obj_returned = env->GetObjectClass(out_obj_ret);
	jfieldID fid_returned = env->GetFieldID(obj_returned, "refValue", "I");
	env->SetIntField(out_obj_ret, fid_returned, jint_returned);

	return;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    Port_EnumUSB
 * Signature: ([BILprinter/PrinterRefInteger;Lprinter/PrinterRefInteger;)V
 */
JNIEXPORT void JNICALL Java_printer_PrinterLibs_Port_1EnumUSB
(JNIEnv* env, jclass obj, jbyteArray byte_arr_buff, jint nBuff, jobject out_obj_needed, jobject out_obj_ret)
{
	jboolean isCopy = JNI_FALSE;

	size_t required_size;
	if (byte_arr_buff != NULL) {
		jbyte* jbyte_buff = env->GetByteArrayElements(byte_arr_buff, &isCopy);
		jsize jbuff_size = env->GetArrayLength(byte_arr_buff);

		size_t len = (int)nBuff;
		char* buff = new char[len];

		memset(buff, 0x0, len);
		if ((buff) != NULL) {
			nzUsbIOEnumerator.Enumerate(buff, len, &required_size);
			for (int j = 0; j < jbuff_size; j++) {
				jbyte_buff[j] = buff[j];
			}

			jclass obj_needed = env->GetObjectClass(out_obj_needed);
			jfieldID fid_needed = env->GetFieldID(obj_needed, "refValue", "I");
			env->SetIntField(out_obj_needed, fid_needed, required_size);
		}
		delete []buff;
		env->ReleaseByteArrayElements(byte_arr_buff, jbyte_buff, 0);
	}
	else {
		char* buff = NULL;

		nzUsbIOEnumerator.Enumerate(buff, nBuff, &required_size);

		jclass obj_needed = env->GetObjectClass(out_obj_needed);
		jfieldID fid_needed = env->GetFieldID(obj_needed, "refValue", "I");
		env->SetIntField(out_obj_needed, fid_needed, required_size);
	}

	jint jint_returned = required_size;
	jclass obj_returned = env->GetObjectClass(out_obj_ret);
	jfieldID fid_returned = env->GetFieldID(obj_returned, "refValue", "I");
	env->SetIntField(out_obj_ret, fid_returned, jint_returned);

	return;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_TextOut
 * Signature: (Ljava/lang/String;IIIIII)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1TextOut
(JNIEnv* env, jclass, jstring print_str, jint x, jint w_scale, jint h_scale, jint font_type, jint font_style, jint encoding)
{
	jboolean res = JNI_FALSE;

	jboolean isCopy;
	std::cout << "lib->POS_TextOut print_str:[" << print_str << "]" << endl;
	const char *utf_str = env->GetStringUTFChars(print_str, &isCopy);
	std::cout << "lib->POS_TextOut utf_str:[" << utf_str << "]" << endl;

	// 1B 40
	pos.POS_Reset();

	if (x >= 0) {
		pos.POS_SetHorizontalAbsolutePrintPosition(x);
	}
	else {
		PosAlignment align;

		switch (x) {
		default:
		case -1:
			align = PosAlignment_Left;
			break;
		case -2:
			align = PosAlignment_HCenter;
			break;
		case -3:
			align = PosAlignment_Right;
			break;
		}
		pos.POS_SetAlignment(align);
	}
	

	pos.POS_SetTextScale((unsigned char)w_scale, (unsigned char)h_scale);

	AsciiTextFontType ascii_font_type = (AsciiTextFontType)font_type;
	pos.POS_SetAsciiTextFontType(ascii_font_type);
	
	(void)font_style;
	
	// 1C 26
	pos.POS_SetMultiByteMode();

	MultiByteModeEncoding nEncoding = (MultiByteModeEncoding)encoding;
	// 1B 39 01
	pos.POS_SetMultiByteEncoding(nEncoding);

	res = pos.POS_PrintString(utf_str) ? JNI_TRUE : JNI_FALSE;

	env->ReleaseStringUTFChars(print_str, utf_str);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_SetBarcode
 * Signature: (Ljava/lang/String;IIIIII)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1SetBarcode
(JNIEnv* env, jclass obj, jstring barcode_content,
	jint x, jint barcode_unit_width, jint barcode_height,
	jint hri_font_type, jint hri_font_pos, jint jtype)
{
	jboolean res;

	jboolean isCopy;
	const char* pcBarcode = env->GetStringUTFChars(barcode_content, &isCopy);

	pos.POS_SetBarcodeReadableTextPosition(x);

	pos.POS_SetBarcodeUnitWidth(barcode_unit_width);

	pos.POS_SetBarcodeHeight(barcode_height);

	pos.POS_SetBarcodeReadableTextFontType(hri_font_type);

	pos.POS_SetBarcodeReadableTextPosition(hri_font_pos);

	res = pos.POS_PrintBarcode(jtype, pcBarcode) ? JNI_TRUE : JNI_FALSE;

	env->ReleaseStringUTFChars(barcode_content, pcBarcode);

	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_SetQRCode
 * Signature: (Ljava/lang/String;IIII)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1SetQRCode
(JNIEnv* env, jclass, jstring str_content, jint x, jint QRCode_unit_width, jint version, jint ecc_level)
{
	jboolean res;

	jboolean isCopy;
	const char* QRCode_str = env->GetStringUTFChars(str_content, &isCopy);

#ifdef DEBUG
	res = pos.POS_PrintQRCode(version, ecc_level, QRCode_str);
#else
	res = pos.POS_EPSON_PrintQRCode(QRCode_unit_width, ecc_level, QRCode_str);
#endif

	env->ReleaseStringUTFChars(str_content, QRCode_str);
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_PrintPicture
 * Signature: (Ljava/lang/String;IIIII)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1PrintPicture
(JNIEnv* env, jclass, jstring file_name, jint x, jint dst_w, jint dst_h, jint binary_algorithm, jint compress_method)
{
	jboolean res = JNI_FALSE;

	// no impl
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_SelfTest
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1SelfTest
(JNIEnv*, jclass)
{
	jboolean res = pos.POS_PrintSelfTestPage() ? JNI_TRUE : JNI_FALSE;
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_FeedLine
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1FeedLine
(JNIEnv*, jclass)
{
	return pos.POS_FeedLines(1) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_FeedNLine
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1FeedNLine
(JNIEnv*, jclass, jint jlineNum)
{
	return pos.POS_FeedLines(jlineNum) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_FeedNDot
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1FeedNDot
(JNIEnv*, jclass, jint dot_num)
{
	return pos.POS_FeedDots(dot_num);
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_QueryStatus
 * Signature: (ILprinter/PrinterRefLong;C)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1QueryStatus
(JNIEnv* env, jclass, jint type, jobject out_status, jchar timeout)
{
	jboolean res = true;

	(void)type; // Fixed 1
	
	jlong lStatus = pos.POS_QueryPrinterStatus(timeout);

	jclass obj_status = env->GetObjectClass(out_status);
	jfieldID fid_status = env->GetFieldID(obj_status, "refValue", "J");
	env->SetLongField(out_status, fid_status, lStatus);

	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_RTQueryStatus
 * Signature: (ILprinter/PrinterRefInteger;C)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1RTQueryStatus
(JNIEnv* env, jclass, jint type, jobject out_status, jchar timeout)
{
	jboolean res = true;

	jint IStatus = pos.POS_RTQueryStatus(type, timeout);

	jclass obj_status = env->GetObjectClass(out_status);
	jfieldID fid_status = env->GetFieldID(obj_status, "refValue", "I");
	env->SetIntField(out_status, fid_status, IStatus);

	res = (IStatus > 0) ? JNI_TRUE : JNI_FALSE;
	return res;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_TicketSucceed
 * Signature: (IJ)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1TicketSucceed
(JNIEnv* env, jclass, jint index, jlong timeout)
{
	(void)index; // Fixed 0
	return pos.POS_QueryPrintResult(0, timeout) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_SetMotionUnit
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1SetMotionUnit
(JNIEnv* env, jclass, jint nHorizontal, jint nVertical)
{
	return pos.POS_SetMovementUnit(nHorizontal, nVertical) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_SetLineHeight
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1SetLineHeight
(JNIEnv* env, jclass, jint nHeight)
{
	return pos.POS_SetTextLineHeight(nHeight) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_SetRightSpacing
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1SetRightSpacing
(JNIEnv* env, jclass, jint nDistance)
{
	return pos.POS_SetAsciiTextCharRightSpacing(nDistance) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_Reset
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1Reset
(JNIEnv*, jclass)
{
	return pos.POS_Reset() ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_KickOutDrawer
 * Signature: (III)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1KickOutDrawer
(JNIEnv* env, jclass, jint nID, jint nHighLevelTime, jint nLowLevelTime)
{
	return pos.POS_KickDrawer(nID, nHighLevelTime, nLowLevelTime) ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_FeedAndCutPaper
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1FeedAndCutPaper
(JNIEnv*, jclass)
{
	return pos.POS_FeedAndCutPaper() ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_FullCutPaper
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1FullCutPaper
(JNIEnv*, jclass)
{
	return pos.POS_FullCutPaper() ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_HalfCutPaper
 * Signature: ()Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1HalfCutPaper
(JNIEnv*, jclass)
{
	return pos.POS_HalfCutPaper() ? JNI_TRUE : JNI_FALSE;
}

/*
 * Class:     printer_PrinterLibs
 * Method:    POS_Beep
 * Signature: (II)Z
 */
JNIEXPORT jboolean JNICALL Java_printer_PrinterLibs_POS_1Beep
(JNIEnv* env, jclass, jint nBeepCount, jint nBeepMillis)
{
	return pos.POS_Beep(nBeepCount, nBeepMillis) ? JNI_TRUE : JNI_FALSE;
}
