
#ifndef _CHECK_INL_
#define _CHECK_INL_

#include "cstd.h"
#include "cfile.h"

#ifndef _IS_CHECK
bool is_check = 0;
#else
bool is_check = 1;
#endif

#if 1
#define GETNAME(NAME) NAME ## _id
#define CHECK_OR_PUT(NAME)        static int GETNAME(NAME) = 0; check_or_put(__FILE__, __LINE__, #NAME, GETNAME(NAME) ++, NAME)
#define CHECK_OR_PUT2(NAME, len)  static int GETNAME(NAME) = 0; check_or_put(__FILE__, __LINE__, #NAME, GETNAME(NAME) ++, NAME, len)
#define CHECK_OR_PUT_ARR(NAME)        static int GETNAME(NAME) = 0; check_or_put_arr(__FILE__, __LINE__, #NAME, GETNAME(NAME) ++, NAME, sizeof(NAME))
#define CHECK_OR_PUT_ARR2(NAME, len)        static int GETNAME(NAME) = 0; check_or_put_arr(__FILE__, __LINE__, #NAME, GETNAME(NAME) ++, NAME, len*sizeof(*NAME))
#else
#define CHECK_OR_PUT(NAME)
#define CHECK_OR_PUT2(NAME, len)
#endif

char* get_file_name(const char* file, int line, const char* name, int id) {
	static char buf[256];
	//_snprintf(buf, countof(buf), "D:\\tmp\\mtcnn\\%d_%s_%d.dat", line, name, id);
	_snprintf(buf, countof(buf), "D:\\tmp\\mtcnn\\%s_%d.dat", name, id);
	return buf;
}

int check_or_put(const char* file1, int line, const char* name, int id, const img_t* mat) {
	char* filename = get_file_name(file1, line, name, id);
	if (is_check) {
		buf_t bf[1] = { 0 };
		buf_load(filename, bf);
		int* p = (int*)bf->data;
		if (bf->len != (4 * 3 + mat->step * mat->rows)) {
			int asdf = 0;
		}
		assert(bf->len==(4*3+ mat->step * mat->rows));
		int h = *p++;
		int w = *p++;
		int t = *p++;
		assert(h == mat->rows && w == mat->cols && t == mat->tid);
		//img_t img1 = cvMat2(h, w, mat->tid, CC_MAT_CN(mat), p);
#if 0
		double dis = cvNorm(mat, &img1)/(h*w);
		double esp = 0.001;
		if (dis > esp) {
			int asdf = 0;
			double *d0 = (double*)mat->data;
			double *d1 = (double*)p;
			for (int i = 0; i < h*w; ++i) {
				if (d0[i] != d1[i]) {
					int asdf = 0;
				}
			}
		}
		assert(dis<esp);
#endif
    if (0 != memcmp(mat->data, p, mat->step * mat->rows)) {
      if (1) {
        int asdf = 0;
        typedef float type;
        type *d0 = (type*)mat->data;
        type *d1 = (type*)p;
        for (int i = 0; i < h*w; ++i) {
          if (d0[i] != d1[i]) {
            int asdf = 0;
          }
        }
      }
      int asdf = 0;
    }
		ASSERT(0==memcmp(mat->data, p, mat->step * mat->rows));
		bffree(bf);
	}
	else {
		int p[] = { mat->rows,mat->cols, mat->tid };
		buf_t bf[] = { sizeof(p), (uchar*)p, mat->step * mat->rows, (uchar*)mat->data };
		savebufs(filename, bf, countof(bf));
	}
	return 0;
}

int check_or_put(const char* file, int line, const char* name, int id, const img_t& mat) {
	check_or_put(file, line, name, id, &mat);
	return 0;
}
template <typename T>
int check_or_put(const char* file, int line, const char* name, int id, const T* arr, int len) {
	img_t img = cvMat2(1, len, (TypeId)TypeIdTrais<T>::id, 1);
	check_or_put(file, line, name, id, &img);
	return 0;
}
int check_or_put_arr(const char* file, int line, const char* name, int id, const void* arr1, int len) {
  uchar* arr = (uchar*)arr1;
	img_t img = cvMat2(1, len, (TypeId)TypeIdTrais<uchar>::id, 1);
	check_or_put(file, line, name, id, &img);
	return 0;
}
#if 0
int check_or_put(const char* file, int line, const char* name, int id, const Mat& mat) {
	Mat mat2 = mat.clone();
	img_t img = mi(mat2);
	check_or_put(file, line, name, id, &img);
	return 0;
}
int check_or_put(const char* file, int line, const char* name, int id, const Matx22d& mat) {
	Mat mat2(mat);
	img_t img = mi(mat2);
	check_or_put(file, line, name, id, &img);
	return 0;
}
int check_or_put(const char* file, int line, const char* name, int id, const Vec6d& mat) {
	Mat mat2(mat);
	img_t img = mi(mat2);
	check_or_put(file, line, name, id, &img);
	return 0;
}
#endif

#endif // _CHECK_INL_
