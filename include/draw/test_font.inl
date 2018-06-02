
#define FONTTESTDEF_DEF(FONTTESTDEF) \
  FONTTESTDEF( test_font1, NULL) \
  FONTTESTDEF( test_font1, 列举出系统中的所有字体)

void test_font1(gc_t* g) {
#if 0
  gcSolidBrush(g, ColorBlack);
FontFamily fontfamily("Arial");
Font font(&fontfamily,8,FontStyleRegular,UnitPoint);  

int count=0;  
int found=0;  

WCHAR familyName[100];//这里为了简化程序，分配足够大的空间  
WCHAR *familyList=NULL;  
FontFamily pFontFamily[500];//同样，分配足够大的空间  

CRect rect;  
this->GetClientRect(&rect);  
RectF r(0,0,rect.Width(),rect.Height());  

InstalledFontCollection installedFontCollection;  
count=installedFontCollection.GetFamilyCount();  
installedFontCollection.GetFamilies(count,pFontFamily,&found);  

familyList=new WCHAR[count*sizeof(familyName)];  
wcscpy(familyList,L"");//先清空familyList，wcscpy实现对宽字节的复制操作  
for(int j=0;j<count;j++){  
  pFontFamily[j].GetFamilyName(familyName);  
  wcscat(familyList,familyName);//把familyName添加到familyList的最后  
  wcscat(familyList,L",");//wcscat实现对宽字节字符的添加操作  
}  
graphics.DrawString(familyList,-1,&font,r,NULL,&solidbrush);  

delete[] familyList;
#endif
}

int test_font(ctrl_t* c) {
  SYSIO;
  IRECT rc = c->rc, rcLeft;
  int i, left_w = 250;
  static int testid = 1;
  static int inited = 0;
  static double t1 = 0;
  static double times[20];
  static int ntime=0;
  static int itime=0;
  double t2 = 0;
  typedef void (*test_font_fun_t)(gc_t* g);
  const test_font_fun_t test_font_fun[] = {
#define FONTTESTDEF(FUN, NAME)   FUN,
    FONTTESTDEF_DEF(FONTTESTDEF)
#undef FONTTESTDEF
  };
  iRectCutR(&rc, -left_w, 0, &rcLeft);
  if (!inited) {
    inited = 1;
  }
  {
    utime_start(time1);
    test_font_fun[testid](g);
    t1 = utime_elapsed(time1);
    QUEUE_PUSH(times, ntime, itime, t1);
    QUEUE_SUM(times, ntime, t2);
    t2/=ntime;
  }

  gcPenAlignment(g, PenAlignmentInset);
  {
    static ctrl_t cc[10] = {0};
    const char* s_test = "|"
#define FONTTESTDEF(FUN, NAME)   #NAME "|"
      FONTTESTDEF_DEF(FONTTESTDEF)
#undef FONTTESTDEF
      ;
    
    i = 0;
    flexboxlay(rcLeft, countof(cc), cc, 2, flex_column);
    fmtlabel_ctrl(cc + i++, "time1 %5.2fms", t2 * 1000);
    rbox_ctrl(cc + i++, "字体测试", s_test, &testid);
  }
  return 0;
}
