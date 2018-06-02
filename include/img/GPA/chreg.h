
#include "Pjsecure.h"
#include "mars.h"
#include "gost.h"
#include "findfile.h"

/*
  chreg_regsave("test");
  assert(chreg_regload("test"));
#define REG_SAVE()  chreg_regsave("GPAdll.dll")
#define REG_CHECK() chreg_regload("GPAdll.dll")
REG_SAVE();
REG_CHECK();
*/

#define REGFILE  "reg.txt"
#define SNFILE   ".sn.txt"

#define SAVESTR(_NAME, _STR, _MODE)  {FILE* pf = fopen(_NAME, _MODE); if (pf) {fprintf(pf, "%s", _STR); fclose(pf);} }
#define LOADSTR(_NAME, _STR, _LEN)  {FILE* pf = fopen(_NAME, "Rb"); if (pf) {fgets(_STR, _LEN, pf); fclose(pf);} }

#define DATASAVE(_STR, _SN, _LEN) datasave(strcat_s(_STR, SNFILE), "mars", _SN, _LEN)
#define DATALOAD(_STR, _SN, _LEN) dataload(strcat_s(_STR, SNFILE), "mars", _SN, _LEN)

char* strcat_s(const char* a, const char* b) {
  static char s[256];
  memset(s, 0, 256);
  strcpy(s, a);
  strcat(s, b);
  return s;
}

int datasave(const char* fname, const char* head, const unsigned char* data, int len) {
  FILE* fpout = fopen(fname, "wb");
  if (fpout) {
    int i;
		fprintf( fpout, "%s:", head );
	  for ( i = 0; i < len; ++i ) {
		  fprintf( fpout, "%02x", data[ i ] );
	  }
    fclose(fpout);
  }
  return 0;
}

int dataload(const char* fname, const char* head, unsigned char* data, int len) {
  int err=0;
  FILE* fpout = fopen(fname, "rb");
  if (fpout) {
    int i, ch;
	  for ( i=0; i<(int)strlen(head) && 0==feof(fpout); ++i) {
      ch = fgetc(fpout);
      if (ch!=head[i]) {
        err=1;
        break;
      }
    }
    ch = fgetc(fpout);
    if (':'!=ch) {
      err=1;
    }
    if (0==err) {
      for ( i = 0; i < len; ++i ) {
        if (0!=feof(fpout)) {
          err=1;
          break;
        }
        fscanf( fpout, "%02x", data+i );
      }
    }
    fclose(fpout);
  }
  return !err;
}

int chreg_get_hard_info(unsigned char* buf, int len) {
  GetCPUNum(buf, 256);
  return 0;
}

int chreg_enc(const char* data, const char* key, unsigned char* sn, int snlen) {
  int maxkeylen=5, len;
  char key0[256];
  len = strlen(key);
  memset(sn, 0, snlen);
  memset(key0, 0, 256);
  strcpy(key0, key);
  strcpy(sn, data);
  mars_enc((ULONG32*)sn, maxkeylen, (ULONG32*)key0,(len)/4);
  return maxkeylen*sizeof(ULONG32);
}

int chreg_regnum(const char* key, unsigned char* sn, int snlen) {
  char data[256] = {0};
  memset(data, 0, 256);
  memset(sn, 0, snlen);
  chreg_get_hard_info(data, 256);
  //printf("%s\n", data);
  //GetNicAddress(buf, 256); ??每次都不一样
  return chreg_enc(data, key, sn, snlen);
}

int chreg_regsave(const char* str) {
  char sn[256];
  int len;
  memset(sn, 0, 256);
  len = chreg_regnum(str, sn, 256);
  DATASAVE(str, sn, len);
  return 0;
}

int chreg_regload(const char* str) {
  unsigned char sn[256] = {0};
  unsigned char sn2[256] = {0};
  int len, ok;
  memset(sn, 0, 256);
  memset(sn2, 0, 256);
  len = chreg_regnum(str, sn, 256);
  ok = DATALOAD(str, sn2, len);
  //printf("%d\n", ok);
  if (ok) {
    ok = !memcmp(sn, sn2, len);
  }
  //printf("%d\n", ok);
  return ok;
}

int chreg_check(const char* str) {
  if (!chreg_regload(str)) {
    char buf[256];
    char buf2[256];
    memset(buf, 0, 256);
    memset(buf2, 0, 256);
    chreg_get_hard_info(buf, 256);
    _snprintf(buf2, 256, "%s:%s\n", buf, str);
    SAVESTR(REGFILE, buf2, "a");
    //MessageBox(NULL,buf, "注册", MB_OK | MB_DEFBUTTON3 | MB_ICONQUESTION |MB_SETFOREGROUND | MB_TOPMOST );
    MessageBox(NULL,"注册文件已保存，请将其发给软件提供商，进行注册。", "注册", MB_OK | MB_DEFBUTTON3 | MB_ICONQUESTION |MB_SETFOREGROUND | MB_TOPMOST );
    return 0;
  }
  return 1;
}

int chreg_reg_save_from_regfile(const char* str) {
  char data[256];
  char key[256];
  char sn[256];
  char* key0;
  int maxkeylen=5, len;
  memset(data, 0, 256);
  memset(key, 0, 256);
  memset(sn, 0, 256);
  len = strlen(str);
  if (strlen(data)==0) return 0;
  key0 = strchr(str, ':');
  if (NULL==key0) return 0;
  memset(data, 0, 256);
  memset(key, 0, 256);
  strncpy(data, str, key0-str);
  strcpy(key, key0+1);
  len = strlen(key);
  while(len>0 && ('\n'==key[len-1]||'\r'==key[len-1]||' '==key[len-1]||'\t'==key[len-1])) {
    key[len-1] = 0;
    --len;
  }
  len = chreg_enc(data, key, sn, 256);
  DATASAVE(key, sn, len);
  return 1;
}

int chreg_reg() {
  char buf[256];
  memset(buf, 0, 256);
  if (file_exist(REGFILE)) {
    FILE* pf = fopen(REGFILE, "rb");
    if (pf) {
      for (; fgets(buf, 256, pf)>0; ) {
        chreg_reg_save_from_regfile(buf);
      }
      fclose(pf);
    }
  }
  return 0;
}
