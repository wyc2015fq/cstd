
#include "ICTCLAS.inl"


int test_ICTCLAS()
{
  double  m_nTime;
  int m_nResultNum;
  char m_sSource[1024] = "中文分词是中文文本处理的一个基础性工作，结巴分词利用进行中文分词";
  char sResult[1024];
  sys_chdir("E:/pub/bin/ICTCLAS/");

  CResult m_ICTCLAS;
  // TODO: Add extra initialization here
  m_nTime = 0;
  m_nResultNum = 1;

  if (!IsDataExists()) {
    printf("错误：Data文件夹不存在或者缺少数据文件！\n");
  }

  m_ICTCLAS.m_nOutputFormat = 0;
  m_ICTCLAS.m_nOperateType = 2;
  clock_t start, finish;

  if (m_nResultNum == 1) {
    start = clock();

    if (!m_ICTCLAS.ParagraphProcessing((char*)m_sSource, sResult)) {
      printf("错误：程序初始化异常！");
    }
    else {
      printf("%s\n", sResult);
    }

    finish = clock();
    m_nTime = (finish - start) * 1000. / CLOCKS_PER_SEC;
    printf("%5.1fms %5.3f\n", m_nTime, m_ICTCLAS.m_dResultPossibility[0]);
    //write  log file
  }
  else {
    if (strlen(m_sSource) > 1024) {
      printf("多个结果处理的文本不能超过1K!");
      return 0;
    }
    else if (m_nResultNum > 10) {
      printf("结果数不能超过10!");
      return 0;
    }

    start = clock();
    char* sSource = new char[strlen(m_sSource) + 20];
    strcpy(sSource, SENTENCE_BEGIN);
    strcat(sSource, m_sSource);
    strcat(sSource, SENTENCE_END);

    if (!m_ICTCLAS.Processing(sSource, m_nResultNum)) {
      printf("错误：程序初始化异常！");
    }
    else {
      finish = clock();
      m_nTime = 1000.*(finish - start) / CLOCKS_PER_SEC;

      for (int i = 0; i < m_ICTCLAS.m_nResultCount; i++) {
        printf("Result %d(Score=%f):\r\n", i + 1, m_ICTCLAS.m_dResultPossibility[i]);
        m_ICTCLAS.Output(m_ICTCLAS.m_pResult[i], sResult, true);
      }
    }

    delete [] sSource;
  }

  return 0;
}
