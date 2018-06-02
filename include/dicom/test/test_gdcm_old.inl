

#include "lib/gdcm/libgdcm.inl"

#if 0

typedef enum { aaaa, bbbb} aaa_t;

typedef struct {
  int group;
  int element;
  const char* owner;
  bool ret;
  aaa_t AA;
  const char* name;
} DICT_ENTRY;

static const DICT_ENTRY DICOMV3DataDict [] = {
  {0x0857, 0x0000, "Nautilus Medical", false, aaaa, "" },
  {0x0857, 0x0000, "Nautilus Medical", false, aaaa, "" },
};
#endif

int test_gdcm()
{
  return 0;
}
