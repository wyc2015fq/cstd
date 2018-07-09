
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tesseract/capi.h>
#include <direct.h>
#include <tesseract/gettimeofday.cpp>

#pragma comment(lib,"leptonica-1.77.0.lib")
#pragma comment(lib,"tesseract40.lib")

void die(const char *errstr) {
    fputs(errstr, stderr);
    exit(1);
}

int test_tesseract(int argc, char *argv[]) {
    TessBaseAPI *handle;
    PIX *img;
    char *text;
	_chdir("D:/code/bin/tesseract/bin/bin");
	//setenv("TESSDATA_PREFIX", "D:\\code\\bin", 1);
	//system("set TESSDATA_PREFIX=D:\\code\\bin");
    //原图像的绝对路径 
    if ((img = pixRead("D:\\code\\data\\ocr\\number.jpg")) == NULL)   
        die("Error reading image\n");

    handle = TessBaseAPICreate();
    if (TessBaseAPIInit3(handle, NULL, "eng") != 0)
        die("Error initialising tesseract\n");

    TessBaseAPISetImage2(handle, img);
    if (TessBaseAPIRecognize(handle, NULL) != 0)
        die("Error in Tesseract recognition\n");

    if ((text = TessBaseAPIGetUTF8Text(handle)) == NULL)
        die("Error getting text\n");

    fputs(text, stdout); //输出OCR识别的文本信息

    system("pause");
    TessDeleteText(text);
    TessBaseAPIEnd(handle);
    TessBaseAPIDelete(handle);
    pixDestroy(&img);

    return 0;
}