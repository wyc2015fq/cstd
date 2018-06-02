
#include "cstd.h"
#include "cfile.h"

static void vstr_print(vstr_t* sv) {
  int i;
  for (i=0; i<sv->n; ++i) {
    printf("%s\n", sv->v[i].s);
  }
}

static int cstr_splitpath(const char* s, int l, str_t* driver, str_t* path, str_t* fn, str_t* driverpath, str_t* pathfn) {
  str_t ss[5] = {0};
  int i = 0, i1;
  l = l<0?strlen(s):l;
  driver = driver ? driver : (ss+i++);
  path = path ? path : (ss+i++);
  fn = fn ? fn : (ss+i++);
  driverpath = driverpath ? driverpath : (ss+i++);
  pathfn = pathfn ? pathfn : (ss+i++);
  i = cstr_findchr(s, 0, 2, ':');
  if (i>=0) {
    driverpath->s = driver->s = (char*)s;
    driver->l = i;
  } else {
    i = 0;
  }
  i1 = cstr_rfindchrs(s, i, l, "/\\", 0);
  if (i<0) {
    i1 = i;
  }
  driverpath->l = i1;
  fn->s = (char*)s + i;
  fn->l = l-i1;
  path->s = (char*)s + i;
  *pathfn = *path = _cstrtrim_c(s+i, i1-i, "/\\");
  pathfn->l = l-(pathfn->s-s);
  return 0;
}

typedef struct hashnode_t_ hashnode_t;
struct hashnode_t_ {
  hashnode_t* next;
  uint32 hash;
  char s[1];
};
static hashnode_t* hash_str(hashnode_t** table, int n, const char* s, int l0, bool binsert) {
  int l = l0<0?strlen(s):l0;
  uint32 hash = RSHash(s, l);
  int i = hash%n;
  hashnode_t* node = table[i];
  if (node) {
    for (;node; node = node->next) {
      if (node->hash==hash && 0==cstr_nicmp_(node->s, -1, s, l, -1, 0, 0)) {
        return node;
      }
    }
  }
  if (binsert) {
    int sz = sizeof(hashnode_t)+l+1;
    char* p = (char*)pmalloc(sz);
    memset(p, 0, sz);
    node = (hashnode_t*)p;
    memcpy(node->s, s, l);
    node->hash = hash;
    node->s[l] = 0;
    node->next = table[i];
    table[i] = node;
  }
  return NULL;
}
static int hash_frees(hashnode_t** table, int n) {
  int i;
  for (i=0; i<n; ++i) {
    pfree(table[i]);
    table[i] = NULL;
  }
  return 0;
}
static int hash_print(hashnode_t** table, int n) {
  int i;
  for (i=0; i<n; ++i) {
    hashnode_t* node = table[i];
    for (;node; node = node->next) {
      printf("%s\n", node->s);
    }
  }
  return 0;
}
static int hashstrset_count(hashnode_t** table, int n) {
  int i, cnt=0;
  for (i=0; i<n; ++i) {
    hashnode_t* node = table[i];
    for (;node; node = node->next) {
      ++cnt;
    }
  }
  return cnt;
}
static int hashstrset_to_strv(hashnode_t** table, int n, vstr_t* sv) {
  int i, j=0, cnt = hashstrset_count(table, n);
  vstr_setsize(sv, cnt);
  for (i=0; i<n; ++i) {
    hashnode_t* node = table[i];
    for (;node; node = node->next) {
      str_setstr(sv->v+j++, node->s, -1);
    }
  }
  return 0;
}
static 
int code_include_list(const char* rootpath, const char* fn, hashnode_t** table, int n) {
  char buf[256];
  int i, j=0, len, i2=0, l0;
  vstr_t sv[1] = {0};
  str_t s[1] = {0};
  char* s0 = NULL;
  const char* ss = "#include \"";
  if (file_exist_in_path(rootpath, NULL, fn, buf, countof(buf), NULL) && str_load(buf, s)) {
    char pathbuf[256];
    char fullpath[256];
    char pathfn[256];
    str_t path[1] = {0};
    str_t name[1] = {0};
    cstr_normalization(buf, -1, "\\/", '/', 0);
    cstr_splitpath(fn, -1, NULL, path, name, NULL, NULL);
    len = strlen(ss);
    for (i=0; i<s->l; i = i2+1) {
      i2 = cstr_findchr(s->s, i, s->l, '\n');
      i2 = (i2<0) ? s->l : i2;
      l0 = i2-i;
      s0 = s->s+i;
      if (*s0==*ss && cstr_icmp(s0, MIN(len, l0), ss, len, 0)==0) {
        int i0 = cstr_findchr(s0, 0, l0, '\"');
        int i1 = cstr_rfindchr(s0, 0, l0, '\"');
        if (i0>0 && i1>(i0+2)) {
          vstr_push_cstr(sv, s0+i0+1, i1-i0-2);
        }
      }
    }
    str_free(s);
    cstr_cpy(pathbuf, 256, path->s, path->l);
    j=0;
    for (i=0; i<sv->n; ++i) {
      if (file_exist_in_path(rootpath, NULL, sv->v[i].s, fullpath, countof(fullpath), pathfn) ||
        file_exist_in_path(rootpath, pathbuf, sv->v[i].s, fullpath, countof(fullpath), pathfn) ) {
        //if (strstr(pathfn, "..")) {          int asdf=0;          path_normalization(pathfn, -1);        }
        path_normalization(pathfn, -1);
        if (!hash_str(table, n, pathfn, -1, 1)) {
          code_include_list(rootpath, pathfn, table, n);
        }
      }
    }
    vstr_free(sv);
  }
  return 0;
}
static 
int get_code_include_list(const char* rootpath, const char*const* fns, int nfn, vstr_t* sv) {
  hashnode_t* table[256] = {0};
  int i, rootlen, n = countof(table);
  char fn[256];
  char rootpath1[256];
  strcpy(rootpath1, rootpath);
  path_normalization(rootpath1, -1);
  rootlen = strlen(rootpath1);
  for (i=0; i<nfn; ++i) {
    strcpy(fn, fns[i]);
    path_normalization(fn, -1);
    if (cstr_icmp(rootpath1, rootlen, fn, rootlen, 1)==0) {
      strcpy(fn, fn+rootlen+1);
    }
    code_include_list(rootpath, fn, table, n);
    hash_str(table, n, fn, -1, 1);
  }
  hashstrset_to_strv(table, n, sv);
  return 0;
}

static int str_cmp1(str_t s1, str_t s2, int ignore_case) {
  int n = MIN(s1.l, s2.l);
  ++n;
  return mem_icmp(s1.s, s2.s, n, ignore_case);
}
static int vstr_sort(vstr_t* sv, int ignore_case) {
//#define STR_LESS(a, b)   ((a).l!=(b).l ? ((a).l<(b).l):str_cmp(a, b, ignore_case)<0)
#define STR_LESS(a, b)   (str_cmp1(a, b, ignore_case)<0)
  QSORT(sv->v, sv->v+sv->n, STR_LESS, str_t);
#undef STR_LESS
  return 0;
}
static int copyfilelist(const char* frompath, const vstr_t* sv, const char* topath) {
  int i, tolen, n=0;
  char sFrom[256];
  char sTo[256];
  mkdirs(topath);
  for (i=0; i<sv->n; ++i) {
    _snprintf(sFrom, 256, "%s/%s", frompath, sv->v[i].s);
    _snprintf(sTo, 256, "%s/%s", topath, sv->v[i].s);
    tolen = strrchr(sTo, '/') - sTo;
    if (tolen>0) {
      mkdirs2(sTo, tolen);
      printf("%s\n", sv->v[i].s);
      sys_cpfile(sFrom, sTo, 0);
      ++n;
    }
  }
  return n;
}
static int cpcode(const char* rootpath, const char* topath, const char*const* fns, int nfn) {
  vstr_t sv[1] = {0};
  get_code_include_list(rootpath, fns, nfn, sv);
  vstr_sort(sv, 1);
  copyfilelist(rootpath, sv, topath);
  vstr_print(sv);
  vstr_free(sv);
  return 0;
}

#include "str/test_path_normalization.inl"

#define CSTD_INCLUDE "C://code//cstd//include//"
#define CPCODE  test_cpcode1(__FILE__, "Z:/yiwei/git/rongyi_term_driver/android/cstd")
#define CPCODETO(TOPATH)  test_cpcode1(__FILE__, TOPATH)
static int test_cpcode1(const char* file, const char* topath) {
  char ch = 0;//getchar();
  if (1||ch=='\n') {
    const char* rootpath = CSTD_INCLUDE;
    //topath = __FILE__;CPM_FaceDetect
    const char *fns[] = {file};
    //const char* topath = "D:/code/out/test_volume_rendering/";
    //topath = "Z:/yiwei/git/rongyi_term_driver/android/cstd";
    cpcode(rootpath, topath, fns, countof(fns));
  }
  return 0;
}
#define CPCODEN(fns)  test_cpcoden(fns, countof(fns))
int test_cpcoden(const char*const* fns, int n) {
  char ch = 0;//getchar();
  if (1||ch=='\n') {
    const char* rootpath = CSTD_INCLUDE;
    //topath = __FILE__;CPM_FaceDetect
    const char* topath = "D:/code/out/test_volume_rendering/";
    topath = "Z:/yiwei/git/rongyi_term_driver/android/cstd";
    cpcode(rootpath, topath, fns, n);
  }
  return 0;
}
int test_cpcode() {
  //test_path_normalization();
  char aa[] = {0x47, 0x50, 0x4a, 0x4d, 0};
  char fn1[256] = "easydraw/adsf/.././mycode_face.inl";
  char fn[256] = "AGC//AGC_MeanPwr_Test_25K_ToTw.inl";
  const char* rootpath = "E://code//cstd//include//";
  //topath = __FILE__;CPM_FaceDetect
  const char *fns[] = {
#if 1
    "cap/test_cap_tcp.inl",
      "CLM_cpp/test_CLM.inl",
      "CLM_cpp/drawkonhon.inl",
      "cap/cap.inl",
      "cap/test_cap.inl",
      "sys/test_parallel_for.inl",
      "facelib/mtcnn/test_mtcnn.inl",
      "facelib/mtcnn_old/test_mtcnn.inl",
      "net/test_httpd.inl",
      "cv1/test/test_ocl.inl",
      "facelib/face_detect_n_track/test_face_detect_n_track.inl",
      "facelib/facedetect/facedetect.inl",
      "facedetect/test_rongyinet.inl",
      "sys.inl",
#else
      "test/test_arm.inl",
#endif
  };
  const char* topath = "D:/code/out/test_volume_rendering/";
  topath = "Z:/yiwei/git/rongyi_term_driver/android/facedetect/code";
  topath = "Z:/yiwei/git/rongyi_term_driver/android/cstd";
  
  cpcode(rootpath, topath, fns, countof(fns));
  return 0;
}
