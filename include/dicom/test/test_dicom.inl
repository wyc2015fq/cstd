

#include "cstd.h"
#include "str.h"
#include "cfile.h"

#include "Converter.inl"


int test_dicom()
{
  dirlist_t sd[1] = {0};
  const char* path = "D:/CTdata/CENOVIXupcompress";
  int i;
  CConverter cc;
  sys_chdir(path);
  dirlist(sd, path, "*.dcm", 0);

  for (i = 0; i < sd->n; ++i) {
    cc.OpenDICM(sd->v[i].name, DIRECT);
    //dicom_transForm(sd->v[i].name);
  }

  dirlist_free(sd);
  return 0;
}
