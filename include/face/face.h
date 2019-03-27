
#ifndef _CSTD_FACE_H__
#define _CSTD_FACE_H__

struct HAARCASCADE;
struct HAARCASCADE* cas_load(const char* fn);

int objdetect(const void* cas, int height, int width, const unsigned char* data, int datastep,
	const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
	double thd, int mincnt, int* xywh, int xywh_len, int is_trans);

int face_detect(int height, int width, const unsigned char* data, int datastep,
	const unsigned char* mask, int maskstep, double sc, double ssmin, double ssmax, double ss, int stepxy,
	double thd, int mincnt, int* xywh, int xywh_len, int is_trans);


#endif // _CSTD_FACE_H__
