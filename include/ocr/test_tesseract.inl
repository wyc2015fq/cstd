
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>
#include <tesseract/capi.h>
#include <direct.h>
#include <tesseract/gettimeofday.cpp>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
#pragma comment(lib,"leptonica-1.77.0.lib")
#pragma comment(lib,"tesseract40.lib")

void die1(const char *errstr) {
    fputs(errstr, stderr);
    exit(1);
}
// 当type为CP_ACP时，GBK转化为UNICODE；当type为CP_UTF8时，UTF8转化为UNICODE
wchar_t* m2w(const char * ch, int type = CP_ACP) {
	int len = MultiByteToWideChar(type, 0, ch, -1, nullptr, 0);
	wchar_t *str = new wchar_t[len + 1];
	wmemset(str, 0, len + 1);
	MultiByteToWideChar(type, 0, ch, -1, str, len);
	return str;
}
// 当type为CP_ACP时，UNICODE转化为GBK；当type为CP_UTF8时，UNICODE转化为UTF8
char* w2m(const wchar_t * wch, int type = CP_ACP) {
	int len = WideCharToMultiByte(type, 0, wch, -1, nullptr, 0, nullptr, nullptr);
	char *str = new char[len + 1];
	memset(str, 0, len + 1);
	WideCharToMultiByte(type, 0, wch, -1, str, len, nullptr, nullptr);
	return str;
}

struct testdata_t {
	int n;
	int idx[20];
public:
};


int LoadTextFile(const string& folder, const string& testfile, std::vector<testdata_t>& testdata, std::vector<string>& imgs) {
	FILE* input = NULL;
	input = fopen(testfile.c_str(), "rb");
	if (input) {
		char buf[1024];
		char fname[256];
		for (; fgets(buf, 1024, input) > 0; ) {
			sscanf(buf, "%s", fname);
			char* p = buf;
			testdata_t td = { 0 };
			for (; p = strchr(p, ' '); ) {
				while (' ' == *p) ++p;
				int k = atoi(p);
				if (k>0) {
					td.idx[td.n++] = k;
				}
			}
			testdata.push_back(td);
			imgs.push_back(folder + fname);
		}
		fclose(input);
	}
	return testdata.size();
}

string GetPredictString(const testdata_t* fm, const vector<string>& labels)
{
	string str;
	for (size_t t = 0; t < fm->n; t++)
	{
		int label = fm->idx[t];
		str += labels[label];
	}
	return str;
}

int test_recognition_rate() {


	TessBaseAPI *handle;
	handle = TessBaseAPICreate();
	if (TessBaseAPIInit3(handle, NULL, "chi_sim") != 0) {
		printf("Error initialising tesseract\n");
		return 0;
	}



	string imgfolder = "C:\\OCR_Line\\images\\";
	string modelfolder = "C:\\OCR_Line\\model\\densenet-no-blstm\\";
	string outfile = "C:\\OCR_Line\\out.txt";
	string testfile = "C:\\OCR_Line\\test.txt";

	vector<string> imgs;
	vector<string> labels;
	std::vector<int> label_idx;
	std::vector<testdata_t> testdata;
	LoadTextFile(imgfolder, testfile, testdata, imgs);
	const string label_file = modelfolder + "label.txt";

	std::ifstream labels_f(label_file.c_str());
	//CHECK(labels_f) << "Unable to open labels file " << label_file;
	string line;
	while (std::getline(labels_f, line))
		labels.push_back(string(line));

	int errch = 0;
	int errcnt = 0;
	for (int i = 0; i < imgs.size(); ++i)
	{
		PIX *img;
		if ((img = pixRead(imgs[i].c_str())) == NULL)
			continue;
		TessBaseAPISetImage2(handle, img);
		if (TessBaseAPIRecognize(handle, NULL) != 0) {
			printf("Error in Tesseract recognition\n");
			continue;
		}
		char* text;
		if ((text = TessBaseAPIGetUTF8Text(handle)) == NULL)
			continue;

		wchar_t* wtext = m2w(text, CP_UTF8);
		char* mtext = w2m(wtext, CP_ACP);
		pixDestroy(&img);
		string strpredict0 = GetPredictString(&testdata[i], labels);
		errcnt += 0 != strcmp(mtext, strpredict0.c_str());
		printf("%5.4f \r", errcnt*1./(i+1));
		delete[] wtext;
		delete[] mtext;
		TessDeleteText(text);
	}

	TessBaseAPIEnd(handle);
	TessBaseAPIDelete(handle);
	return 0;
}
int test_tesseract(int argc, char *argv[]) {
	//test_recognition_rate();
    TessBaseAPI *handle;
    PIX *img;
    char *text;
	_chdir("D:/code/bin/tesseract/bin/bin");
	//setenv("TESSDATA_PREFIX", "D:\\code\\bin", 1);
	//system("set TESSDATA_PREFIX=D:\\code\\bin");
    //原图像的绝对路径 
    if ((img = pixRead("20436218_1024524228.jpg")) == NULL)   {
        printf("Error reading image\n");
		return 0;
	}

	handle = TessBaseAPICreate();
	if (TessBaseAPIInit3(handle, NULL, "chi_sim") != 0) {
		printf("Error initialising tesseract\n");
		return 0;
	}

	TessBaseAPISetImage2(handle, img);
	if (TessBaseAPIRecognize(handle, NULL) != 0) {
		printf("Error in Tesseract recognition\n");
		return 0;
	}

	if ((text = TessBaseAPIGetUTF8Text(handle)) == NULL) {
		printf("Error getting text\n");
		return 0;
	}

	wchar_t* wtext = m2w(text, CP_UTF8);
	char* mtext = w2m(wtext, CP_ACP);
    fputs(mtext, stdout); //输出OCR识别的文本信息
	delete[] wtext;
	delete[] mtext;
    system("pause");
    TessDeleteText(text);
    TessBaseAPIEnd(handle);
    TessBaseAPIDelete(handle);
    pixDestroy(&img);

    return 0;
}