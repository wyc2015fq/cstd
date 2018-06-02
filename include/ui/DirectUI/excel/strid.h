

typedef UINT  StrID_t;

char** g_strtable=0;
unsigned int g_strtableused=0;
unsigned int g_strtablesize=0;

void clear_strtable(void) {
  unsigned int i;
  if (g_strtable) {
    for (i=0; i<g_strtableused; ++i) {
      free(g_strtable[i]);
      g_strtable[i]=0;
    }
    free(g_strtable);
    g_strtableused=0;
    g_strtablesize=0;
    g_strtable=0;
  }
}
char* idtostr(unsigned int id) {
  if (id<g_strtableused && NULL!=g_strtable) {
    return g_strtable[id];
  }
  return NULL;
}
StrID_t strtoid(const char* s) {
  unsigned int i;
  static int inited=0;
  if (0==inited) {inited=1;atexit(clear_strtable);}
  if (0==g_strtable) {
    g_strtableused = 0;
    g_strtablesize = 32;
    g_strtable = realloc(g_strtable, sizeof(char*)*g_strtablesize);
    g_strtable[g_strtableused++] = strdup("");
  }
  for (i=0; i<g_strtableused; ++i) {
    if (0==strcmp(g_strtable[i], s)) {
      return i;
    }
  }
  if (g_strtableused>=g_strtablesize) {
    g_strtablesize = g_strtableused*2;
    g_strtable = realloc(g_strtable, sizeof(char*)*g_strtablesize);
  }
  g_strtable[g_strtableused++] = strdup(s);
  return g_strtableused-1;
}

char* randstr(int maxlen) {
  int i, len;
  static char buf[256];
  static int inited=0;
  //const char* chset="abcdefghijklmnopqrstuvwxyz0123456789";
  const char* chset="01";
  int chsetlen = strlen(chset);
  if (inited==0) {inited=1;srand(100);}
  if (maxlen>256) {maxlen = 256;}
  len = rand()%maxlen;
  buf[len]=0;
  for (i=0; i<len; ++i) {
    int d = rand()%chsetlen;
    buf[i] = chset[d];
  }
  return buf;
}

int test_strid() {
  int i;
  for (i=0; i<100; ++i) {
    char* s = randstr(10);
    int d = strtoid(s);
    const char* s2 = idtostr(d);
    printf("%s\n", s2);
    //Sleep(20);
  }
  printf("strtable used=%d\n", g_strtableused);
  clear_strtable();
  return 0;
}
