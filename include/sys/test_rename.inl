
// 
#include "cstd.h"
#include "cfile.h"

//dir一个目录中所有文件功能
// FindFile.cpp : Defines the entry point for the console application.
//

const int MAXLEN = 1024;                //定义最大目录长度
unsigned long FILECOUNT = 0;            //记录文件数量

int listdir(const char* dirpath, fileinfo_t** finfo)
{
  return 0;
}

int test_findfile() {
  dirlist_t dir[1] = {0};
  int i;
  double tt = time_before(0, 0, 1,0,0,0);
  sys_chdir("E:/book");
  dirlist(dir, "E:/book", "*.pdf", FF_SUBDIR);
  for (i=0; i<dir->n; ++i) {
    char* name = GetFileNameExt(dir->v[i].name);
    printf("%10.f %20s\n", timetof64(dir->v[i].wtime), name);
    if (0==cstr_icmp(name, 4, "view", 4, 1)) {
      rename(dir->v[i].name, name+4);
    }
  }
  dirlist_free(dir);
  return 0;
}

int test_rename() {
  dirlist_t dir[1] = {0};
  int i;
  double tt = time_before(0, 0, 1,0,0,0);
  sys_chdir("E:/www/mv/vip.aqdyh.net/htm_rmvb_have");
  dirlist(dir, "E:/www/mv/vip.aqdyh.net/htm_rmvb_have", "*.html", FF_SUBDIR);
  for (i=0; i<dir->n; ++i) {
    char* name = GetFileNameExt(dir->v[i].name);
    printf("%10.f %20s\n", timetof64(dir->v[i].wtime), name);
    if (0==cstr_icmp(name, 4, "view", 4, 1)) {
      rename(dir->v[i].name, name+4);
    }
  }
  dirlist_free(dir);
  return 0;
}

int strv_include1(const vstr_t* sv, vstr_t* sv2, const char* incstr, int l_incstr, int ignore_case) {
  int i, j=0, pos;
  strv_setsize(sv2, sv->n);
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  for (i=0; i<sv->n; ++i) {
    pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    if (pos>=0) {
      str_t* s1 = sv2->v + j++;
      str_setstr(s1, sv->v[i].s, sv->v[i].l);
    }
  }
  strv_setsize(sv2, j);
  return j;
}
#include "str\strstr.inl"
int strv_include(const vstr_t* sv, vstr_t* sv2, const char* incstr, int l_incstr, int ignore_case) {
  int i, j=0, pos;
  int charStep[MAX_CHAR_SIZE] = {0};
  strv_setsize(sv2, sv->n);
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  setCharStep(incstr, l_incstr, charStep);
  for (i=0; i<sv->n; ++i) {
    //pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    pos = strstr_sunday_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, charStep);
    if (pos>=0) {
      str_t* s1 = sv2->v + j++;
      str_setstr(s1, sv->v[i].s, sv->v[i].l);
    }
  }
  strv_setsize(sv2, j);
  return j;
}
int strv_includes(const vstr_t* sv, vstr_t* sv2, const char* incstr, int l_incstr, int ignore_case) {
  int i, j=0, pos, k;
  vstr_t sv3[1] = {0};
  strv_setsize(sv2, sv->n);
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  strv_split_str(sv3, STR2(incstr, l_incstr), " ", " ", 2);
  for (i=0; i<sv->n; ++i) {
    //pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    pos = -1;
    for (k=0; k<sv3->n; ++k) {
      pos = cstr_find(sv->v[i].s, sv->v[i].l, sv3->v[k].s, sv3->v[k].l, 0, ignore_case);
      if (pos>=0) {
        str_t* s1 = sv2->v + j++;
        str_setstr(s1, sv->v[i].s, sv->v[i].l);
        break;
      }
    }
  }
  strv_setsize(sv2, j);
  strv_free(sv3);
  return j;
}
int strs_include_find(const str_t* s, int n, const char* incstr, int l_incstr, int i, int ignore_case) {
  int j=0, pos;
  int charStep[MAX_CHAR_SIZE] = {0};
  l_incstr = l_incstr<0 ? strlen(incstr) : l_incstr;
  setCharStep(incstr, l_incstr, charStep);
  for (; i<n; ++i) {
    //pos = cstr_find(sv->v[i].s, sv->v[i].l, incstr, l_incstr, 0, ignore_case);
    pos = strstr_sunday_find(s[i].s, s[i].l, incstr, l_incstr, charStep);
    if (pos>=0) {
      return i;
    }
  }
  return -1;
}
int strv_uniq_filename(const vstr_t* sv, vstr_t* sv2, int ignore_case) {
  int i, j=0, pos;
  strv_setsize(sv2, sv->n);
  for (i=0; i<sv->n; ++i) {
    char* name = GetFileNameExt(sv->v[i].s);
    pos = strs_include_find(sv->v, sv->n, name, -1, i+1, ignore_case);
    if (pos<0) {
      str_t* s1 = sv2->v + j++;
      str_setstr(s1, sv->v[i].s, sv->v[i].l);
    }
  }
  strv_setsize(sv2, j);
  return j;
}
int strv_tostr(const vstr_t* sv, str_t* s, const char* sp) {
  int i, n = 0, spl = strlen(sp);
  for (i=0; i<sv->n; ++i) {
    n += sv->v[i].l;
  }
  n += sv->n * spl;
  str_setsize(s, n);
  n = 0;
  for (i=0; i<sv->n; ++i) {
    memcpy(s->s + n, sv->v[i].s, sv->v[i].l);
    memcpy(s->s + n + sv->v[i].l, sp, spl);
    n += sv->v[i].l + spl;
  }
  return 0;
}
int str_include_split(vstr_t* sv, str_t s, str_t incstr, int ignore_case, const char* trimstr) {
  int i=0;
  str_t s2[1] = {0};
  strv_setsize(sv, 0);
  for (;(i = STRsplit_find(s, incstr, i, ignore_case, "\n", trimstr, s2))>=0;) {
    strv_push_str(sv, (*s2));
  }
  return 0;
}
int test_have_rmvb() {
  dirlist_t dir[1] = {0};
  int i, j;
  vstr_t sv[5] = {0};
  //str_t s[5] = {0};
  vstr_t* sv1 = sv+1;
  vstr_t* sv2 = sv+2;
  double tt = time_before(0, 0, 1,0,0,0);
  sys_chdir("E:/www/mv/vip.aqdys.net");
  //test_strstr();
  vstr_load("info.txt", sv);
  if (0) {
    {
      utime_start(_start_time);
      strv_include(sv, sv2, "西条沙罗", -1, 1);
      printf("%f\n", utime_elapsed(_start_time));
    }
    {
      utime_start(_start_time);
      strv_include1(sv, sv2, "西条沙罗", -1, 1);
      printf("%f\n", utime_elapsed(_start_time));
    }
  }
  //strv_tostr(sv, s, "\n");
  //str_include_split(sv2, *s, STR1("MIMK"), 1, " ");
  strv_include(sv, sv, "亚莉西", -1, 1);
  // 亚莉西 园田美樱 织田真子 肉弾 松嶋葵 上原亚衣 MMND PURIN 菅野松雪 OHO CEAD DASD 巨乳 星美梨香 KTDS 青叶优香 高岛恭子 香山美樱 佳澄果穗 宇都宫紫苑 澁谷果步 七草千岁 白石真琴 梦乃爱华 雾岛樱 希咲艾玛 风间由美 宫野ゆかな 福咲れん HEYZO NITR NATR 绫濑南 爆乳 成宫美琴 EKDV SPRD 河西希 吉川爱美 KAORI 中山理莉 杏堂夏 当真由纪 星野娜美 木南日菜 松本美 宇佐木桃 木下和津实 ERIKA 藤本紫媛 杏美月 若槻水菜 北山柑菜 小西悠 春菜花 ひばり结羽 穗高悠希 水元恵梨香 西村ニーナ 舞咲美娜 本间丽花 叶月美音 三喜本 芹野莉奈 铃木心春 HND 吹石玲奈 尾上若叶 黒人 长瀬麻美 澁谷果步 僕 ゆいの 上城里绪菜 田中真树 双叶由奈 折原ほのか 甘良滴 BOBB 西篠可怜 橘优花 西野绘里香 雾岛樱 高桥美绪 小泉真希 茜杏珠 桐原绘里香 赤井美月 素人性欲 佐佐木绘里
  dirlist(dir, "H:/MV", "*.jpg", FF_SUBDIR);
  for (j=0; j<sv->n; ++j) {
    char* p;
    int len = 10;
    strv_split_str(sv2, sv->v[j], "*", "\" \n", 0);
    p = sv2->v[3].s;
    len = MIN(len, sv2->v[3].l);
    if (len<10) {
      continue;
    }
    for (i=0; i<dir->n; ++i) {
      char* name = GetFileNameExt(dir->v[i].name);
      int len2 = len;
      int len1 = strlen(name);
      len2 = MIN(len2, len1);
      if (0==cstr_icmp(p, len2, name, len2, 1)) {
        break;
      }
    }
    if (i==dir->n) {
      strv_split_str_add(sv1, sv2->v[1], "#", " \n\"", 5);
    }
    //printf("%d %s\n", (i==dir->n), p);
  }
  strv_include(sv1, sv1, "rmvb", -1, 1);
  strv_uniq_filename(sv1, sv1, 1);
  for (j=0; j<sv1->n; ++j) {
    logprintf("%s\n", sv1->v[j].s);
  }
  strv_frees(sv, countof(sv));
  dirlist_free(dir);
  return 0;
}

