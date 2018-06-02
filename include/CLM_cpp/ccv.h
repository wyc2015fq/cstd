
#include "cstd.h"
#include "cfile.h"
#include "cv/cv.inl"

// OpenCV stuff
enum {
	CV_8U = CC_8U,
	CV_16S = CC_16S,
	CV_32F = CC_32F,
	CV_32S = CC_32S,
	CV_64F = CC_64F,
	CV_32FC1 = CC_32FC1,
	CV_32SC1 = CC_32SC1,
	CV_64FC1 = CC_64FC1,
	CV_8UC1 = CC_8UC1,
};


enum {
	CV_TM_SQDIFF = CC_TM_SQDIFF,
	CV_TM_SQDIFF_NORMED = CC_TM_SQDIFF_NORMED,
	CV_TM_CCORR = CC_TM_CCORR,
	CV_TM_CCORR_NORMED = CC_TM_CCORR_NORMED,
	CV_TM_CCOEFF = CC_TM_CCOEFF,
	CV_TM_CCOEFF_NORMED = CC_TM_CCOEFF_NORMED
};

typedef Img Mat;
#define Point_ CPoint_
#define Size_ CSize_
#define Rect_ CRect_
typedef CPoint2i Point;
typedef CPoint2f Point2f;
typedef CPoint2d Point2d;
typedef CPoint3f Point3f;
typedef CPoint3d Point3d;
//typedef CRect_<int> Rect;
#define MatIterator_ ImgIterator_
#define MatConstIterator_ ImgConstIterator_
#define getOptimalDFTSize cGetOptimalDFTSize
typedef vec2d Vec2d;
typedef vec3d Vec3d;
typedef vec4d Vec4d;
typedef vec6d Vec6d;
typedef CSize Size;
typedef CScalar Scalar;
typedef matx22f Matx22f;
typedef matx33d Matx33d;
typedef matx22d Matx22d;
typedef matx23d Matx23d;
typedef matx44d Matx44d;
typedef Svd SVD;
#define cvScalarAll cScalarAll
#define cvRealScalar cRealScalar
#define cScalar1 cScalar

template<class _Elem, class _Traits>
std::basic_fstream<_Elem, _Traits>& operator<<(std::basic_fstream<_Elem, _Traits>& o, const Img& img) {
	return o;
}


vec3d cVec3d(double x, double y=0, double z=0) {
  vec3d v(x, y, z);
	return v;
}

vec6d cVec6d(double v0, double v1=0, double v2 = 0, double v3 = 0, double v4 = 0, double v5 = 0) {
	vec6d v(v0, v1, v2, v3, v4, v5);
	return v;
}

Img mi(const Img& x, CRect rc) {
	return x(rc);
}

void convertTo(const Img& a, Img& b, int x, int y, int w, int h, int type) {
	a.convertTo(b(cRect(x, y, w, h)), type);
}

void copyTo(const Mat& a, Mat& b, int x, int y, int w, int h) {
	if (a.tid != b.tid) {
		a.convertTo(b(cRect(x, y, w, h)), b.tid);
	}
	else {
		a.copyTo(b(cRect(x, y, w, h)));
	}
}

img_t mi1(const Mat& x) {
	return x;
}

Mat ma(const img_t* im) {
	return Mat(*im);
}

CC_INLINE CPoint2d cPoint2d(double x, double y)
{
	CPoint2d pt;
	pt.x = x, pt.y = y;
	return pt;
}
Point3f cPoint3f(double x, double y, double z) {
	return fPOINT3(x, y, z);
}
double norm(CPoint3f pt) {
	return sqrt(pt.x*pt.x + pt.y*pt.y + pt.z*pt.z);
}
Point3f cPoint3f(const Point3d a) {
	Point3f c;
	c.x = a.x;
	c.y = a.y;
	c.z = a.z;
	return c;
}
Point3d cPoint3d(const Point3f a) {
	Point3d c;
	c.x = a.x;
	c.y = a.y;
	c.z = a.z;
	return c;
}

Point3f cPoint3f(const vec3d a) {
	Point3f c;
	c.x = a.val[0];
	c.y = a.val[1];
	c.z = a.val[2];
	return c;
}
Point3f cPoint3f(const Mat& mat) {
	Point3f pt;
	ASSERT(mat.rows*mat.cols==3);
	mat.convertTo(&pt, CC_32F);
	return pt;
}
Point3f operator /(const Point3f& a, double b) {
	Point3f c;
	c.x = a.x / b;
	c.y = a.y / b;
	c.z = a.z / b;
	return c;
}
vec3d operator *(const matx33d& a, vec3d b) {
	vec3d c;
	matvec3(c.val, a.val, b.val);
	return c;
}

Mat convertTo(const Mat& mat, int type) {
	Mat MatTemp2 = mat;
	mat.convertTo(MatTemp2, type);
	return MatTemp2;;
}


#ifndef _USE_MYCV
bool is_check = 0;
#else
bool is_check = 1;
#endif

#if 0
#define CHECK_OR_PUT(NAME)  static int NAME ## _id = 0; check_or_put(__FILE__, __LINE__, #NAME, NAME ## _id ++, NAME)
#define CHECK_OR_PUT2(NAME, len)  static int NAME ## _id = 0; check_or_put(__FILE__, __LINE__, #NAME, NAME ## _id ++, NAME, len)
#else
#define CHECK_OR_PUT(NAME)
#define CHECK_OR_PUT2(NAME, len)
#endif

char* get_file_name(const char* file, int line, const char* name, int id) {
	static char buf[256];
	_snprintf(buf, countof(buf), "D:\\tmp\\clm\\%d_%s_%d.dat", line, name, id);
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
		img_t img1 = cvMat2(h, w, mat->tid, CC_MAT_CN(mat), p);
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
		if (0 != memcmp(mat->data, p, mat->step * mat->rows)) {
			int asdf = 0;
		}
		//assert(0==memcmp(mat->data, p, mat->step * mat->rows));
		bffree(bf);
	}
	else {
		int p[] = { mat->rows,mat->cols, mat->tid };
		buf_t bf[] = { sizeof(p), (uchar*)p, mat->step * mat->rows, (uchar*)mat->data };
		savebufs(filename, bf, countof(bf));
	}
	return 0;
}

int check_or_put(const char* file, int line, const char* name, int id, const Mat& mat) {
	Mat mat2 = mat.clone();
	check_or_put(file, line, name, id, &mat2);
	return 0;
}
int check_or_put(const char* file, int line, const char* name, int id, const img_t& mat) {
	check_or_put(file, line, name, id, &mat);
	return 0;
}
template <typename T>
int check_or_put(const char* file, int line, const char* name, int id, const T* arr, int len) {
	img_t img = cvMat(1, len, (TypeId)TypeIdTrais<T>::id, 1);
	check_or_put(file, line, name, id, &img);
	return 0;
}
int check_or_put(const char* file, int line, const char* name, int id, const Matx22d& mat) {
	Mat mat2(mat);
	check_or_put(file, line, name, id, &mat2);
	return 0;
}
int check_or_put(const char* file, int line, const char* name, int id, const Vec6d& mat) {
	Mat mat2(mat);
	check_or_put(file, line, name, id, &mat2);
	return 0;
}
