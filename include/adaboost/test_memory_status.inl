
int test_memory_status()
{
  char* divisor = "M";
  MEMORYSTATUS stat;
  enum { DIV = (1024 * 1024), WIDTH = 7 };

  GlobalMemoryStatus(&stat);

  printf("The MemoryStatus structure is %ld bytes long.\n", stat.dwLength);
  printf("It should be %d.\n", sizeof(stat));
  printf("%ld percent of memory is in use.\n", stat.dwMemoryLoad);
  printf("There are %*ld total %sbytes of physical memory.\n", WIDTH, stat.dwTotalPhys / DIV, divisor);
  printf("There are %*ld free %sbytes of physical memory.\n", WIDTH, stat.dwAvailPhys / DIV, divisor);
  printf("There are %*ld total %sbytes of paging file.\n", WIDTH, stat.dwTotalPageFile / DIV, divisor);
  printf("There are %*ld free %sbytes of paging file.\n", WIDTH, stat.dwAvailPageFile / DIV, divisor);
  printf("There are %*lx total %sbytes of virtual memory.\n", WIDTH, stat.dwTotalVirtual / DIV, divisor);
  printf("There are %*lx free %sbytes of virtual memory.\n", WIDTH, stat.dwAvailVirtual / DIV, divisor);
  //divisor = malloc(stat.dwAvailPhys * 0.9);
  return 0;
}

//#include "test_cascade.inl"
//#include "test_samppair.inl"

int fill_randn(int h, int w, double* arr, int al, double me)
{
  int i, j;

  for (i = 0; i < h; ++i, arr += al) {
    for (j = 0; j < w; ++j) {
      arr[j] = randn(i + me, 1);
      //printf("%f\n", arr[j]);
    }

    //printf("\n");
  }

  return 0;
}

// 生成纯数据特征
int test_makedat()
{
  enum {hh = 1000, ww = 1000};
  double* data = MALLOC(double, 2 * hh * ww);
  const char* fn = "ss.dat";
  srand(12);
  fill_randn(hh, ww, data, 2 * ww, 0);
  fill_randn(hh, ww, data + ww, 2 * ww, 1);
  _chdir("D:/pub/bin/adaboost/data");
  savefile(fn, data, sizeof(double)*hh * ww);
  FREE(data);
  ininame(INIFILENAME);
  iniappname(ADABOOSTSECT);
  inisetint("特征数目", hh);
  inisetint("正样本数目", ww);
  inisetint("负样本数目", ww);
  inisetstr("样本文件", fn);
  return 0;
}
