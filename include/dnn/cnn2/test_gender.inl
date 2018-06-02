
#include "test_mnist.inl"
#include "img/imgio.inl"
#include "util/db_lmdb.inl"
//#include "tools/convert_imageset.inl"

const char* age_tag[] = {
  "region1_5",
    "region6_10",
    "region11_15",
    "region16_20",
    "region21_25",
    "region26_30",
    "region31_35",
    "region36_40",
    "region41_45",
    "region46_50",
    "region51_55",
    "region56_60",
    "region61_65",
    "region66_70",
    "region71_75",
    "region76_80",
};
const char* gender_tag[] = {
  "Female",
  "Male",
};
int str_to_label(const char* str, const char** strs, int len, int delims) {
  int i;
  for (i=0; i<len; ++i) {
    const char* p = str;
    const char const * s = strs[i];
    int l = strlen(s);
    for (;*p;++p) {
      if (delims==*p && delims==p[l+1] && 0==memicmp(p+1, s, l)) {
        return i;
      }
    }
  }
    printf("error : %s unkown label\n", str);
  if (1) {
    int asdf=0;
  }
  return -1;
}
int test_gender() {
  const char* list_file = "list.txt";
  char out_file[256] = "gender";
  FILE* pout;
  sys_chdir("D:/code/cnn/gender");
  vstr_t sv[1] = {0};
  int i;
  img_t im[1] = {0};
  DataSize size;
  int delims = '\\';
  vstr_load(list_file, sv);
  if (0) {
    for (i=0; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      //int age_id = str_to_label(sv->v[i].s, age_tag, countof(age_tag), delims);
    }
  }
  size = dataSize(sv->n, 1, 64, 64);
  _snprintf(out_file, 256, "gender_%dx%dx%d.dat", size.c, size.h, size.w);
  pout = fopen(out_file, "wb");
  if (pout) {
    //fwrite(&size, sizeof(size), 1, pout);
    for (i=0; i<sv->n; ++i) {
      int gender_id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
      if (gender_id>=0 && imread(sv->v[i].s, size.c, 1, im)) {
        int id = str_to_label(sv->v[i].s, gender_tag, countof(gender_tag), delims);
        int bytesize = Data_3DSize(size);
        imresize(im, size.h, size.w, im);
        fwrite(&gender_id, sizeof(gender_id), 1, pout);
        fwrite(im->data, bytesize, 1, pout);
        printf("%7d/%-7d %2d %2d%%\n", i, sv->n, gender_id, i*100/sv->n);
        //imshow_(im);WaitKey(-1);
      }
    }
  }
  imfree(im);
  vstr_free(sv);
  return 0;
}
