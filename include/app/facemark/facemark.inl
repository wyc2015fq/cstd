

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#ifdef _WIN64
#pragma comment(lib,"opencv_world320.lib")
#pragma comment(lib,"opencv_ts320.lib")
#pragma comment(lib,"strmiids.lib")
#else
#ifdef _DEBUG
#pragma comment(lib,"opencv_world300d.lib")
#else
#pragma comment(lib,"opencv_world300.lib")
#endif
#endif



#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <direct.h>


// 性别
enum CC_GENDER { CC_Unknown=0, CC_Female,  CC_Male };

typedef struct facemark_t {
	char name[256];
	CC_GENDER gender; //
	int age;  // <=0 表示 Unknown
} facemark_t;


int facemark() {
#ifdef _DEBUG
	_chdir("D:/code/facemark");
#endif
	const char* fname = "facemarkout.txt";
	FILE* pf = fopen(fname, "rb");
	facemark_t* pm = NULL;
	if (NULL == pf) {
		return 0;
	}
	char buf[1024];
	int pic_count = 0, i;
	fseek(pf, 0, SEEK_SET);
	for (; fgets(buf, 1024, pf); pic_count++);
	fseek(pf, 0, SEEK_SET);
	i = 0;
	pm = (facemark_t*)malloc(pic_count * sizeof(facemark_t));
	if (pm) {
		fseek(pf, 0, SEEK_SET);
		for (; fgets(buf, 1024, pf); ++i) {
			int c=0, d=0;
			int  len = strlen(buf);
			while (len > 0 && ('\r' == buf[len - 1] || '\n' == buf[len - 1])) { --len; }
			buf[len] = 0;
			sscanf(buf, "%s %c %d", pm[i].name, &c, &d);
			c = tolower(c);
			pm[i].gender = 'f'==c ? CC_Female : 'm'==c ? CC_Male : CC_Unknown;
			pm[i].age = d;
		}

		if (pic_count>0) {
			using namespace cv;
			int ch = 0;
			Mat image, show_image;
			int cur_i = -1;
			i = 0;
			for (; 1;) { //
				if (cur_i!=i) {
					if (i < 0) {
						i = 0;
						printf("i = %d", i);
					}
					if (i >= pic_count) {
						i = pic_count-1;
						printf("i = %d", i);
					}
				  image = imread(pm[i].name);
				  cur_i = i;
				}
				{
					show_image = image.clone();
					int c = CC_Female == pm[i].gender ? 'F' : CC_Male == pm[i].gender ? 'M' : '_';
					snprintf(buf, 256, "%c %d", c, pm[i].age);
					cv::putText(show_image, buf, cv::Point(50, 50), 0, 1, cv::Scalar(0, 0, 255), 2);
					imshow("result", show_image);
				}
				int ch = waitKey(10);
				ch = tolower(ch);
				if ('[' == ch) { --i; }
				if (']' == ch) { ++i; }
				if ('f' == ch) { pm[i].gender = CC_Female; }
				if ('m' == ch) { pm[i].gender = CC_Male; }
				if ('0' <= ch && ch<='9') { pm[i].age = (ch - '0')*10+5; }
				if ('s' == ch || 'q' == ch) {
					FILE* pf = fopen(fname, "wb");
					for (int j = 0; j < pic_count; ++j) {
						int c = CC_Female == pm[j].gender ? 'F' : CC_Male == pm[j].gender ? 'M' : '_';
						fprintf(pf, "%s %c %d\n", pm[j].name, c, pm[j].age);
					}
					fclose(pf);
				}
				//printf("%c\n", ch);
			}
		}
		free(pm);
	}
	fclose(pf);
	return 0;
}
