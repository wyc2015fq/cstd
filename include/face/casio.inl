
int fgetd(FILE* pf, int defd) {
	int d = defd;
	int ret = fscanf(pf, "%d", &d);
	return d;
}
double fgetf1(FILE* pf, double deff) {
	double d = deff;
	int ret = fscanf(pf, "%lf", &d);
	return d;
}
int fgetf_i6(FILE* pf, double deff) {
	double d = deff;
	int ret = fscanf(pf, "%lf", &d);
	return _SHIFT_TOINT(d);
}

struct HAARCASCADE* cas_load(const char* fn)
{
	FILE* pf = fopen(fn, "rb");
	if (NULL == pf) return 0;
	int i, ret, stagelen, weaklen, cx, cy, is_nesting;
	size_t weaksize, featsize;
	char* weakptr, *featptr;
	HAARCASCADE* ca = NULL;
	HAARCASCADE* ptr = NULL;
	iStage* stage = NULL;
	iHaar* haar = NULL;
	//_SHIFT_TOINT();
	ret = fscanf(pf,  "%d %d %d %d %d" ENDL, &stagelen, &weaklen, &cx, &cy, &is_nesting);
	ca = (HAARCASCADE*)malloc(sizeof(HAARCASCADE)+ stagelen*sizeof(fStage)+ weaklen*sizeof(fHaar));


	memset(ca, 0, sizeof(HAARCASCADE));
	ptr = ca;
	ptr->stagelen = stagelen;
	ptr->ihlen = weaklen;
	ptr->w = cx;
	ptr->h = cy;
	//ptr->is_nesting = is_nesting;

	stage = (iStage *)(ca + 1);
	haar = (iHaar *)(stage + stagelen);

	for (i = 0, weaklen = 0; i < ptr->stagelen; ++i) {
		stage[i].len = fgetd(pf, 0);
		stage[i].thd = fgetf_i6(pf, 0);
		weaklen += stage[i].len;
	}

	ASSERT(weaklen <= ptr->ihlen);

	for (i = 0; i < weaklen; ++i) {
		int id = 0;
		double d;
		fscanf(pf, "%d %d %d %d %d %d %d %d %d %d %d" ENDL, &id,
			&haar[i].ft0.x, &haar[i].ft0.y, &haar[i].ft0.w, &haar[i].ft0.h, &haar[i].ft0.d,
			&haar[i].ft1.x, &haar[i].ft1.y, &haar[i].ft1.w, &haar[i].ft1.h, &haar[i].ft1.d
		);
		haar[i].minval = fgetf_i6(pf, 0);
		haar[i].binval = fgetf_i6(pf, 0);
		for (int j = 0; j < LUT_BIN; ++j) {
			haar[i].val[j] = fgetf_i6(pf, 0);
		}
		int asdf = 90;
	}

	ptr->ihlen = weaklen;
#if 0
#endif
	return ca;
}
