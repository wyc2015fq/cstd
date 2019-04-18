# 北大天网搜索引擎TSE分析及完全注释[2]路过查询处理程序 - nosmatch的专栏 - CSDN博客
2012年02月24日 10:10:13[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：1027
    由上一篇文章[原]自顶向下学搜索引擎——北大天网搜索引擎TSE分析及完全注释[1]寻找搜索引擎入口 我们可以知道整个程序是从TSESearch.cpp 中的main函数开始的我们重点一下这段代码
//TSESearch.cpp CQuery iQuery;
iQuery.GetInputs();  //具体程序开始执行
 // current query & result page number
 iQuery.SetQuery();
 iQuery.SetStart();
 // begin to search
 //开始具体搜索程序
 gettimeofday(&begin_tv,&tz); //开始计时获取程序运行时间差
 iQuery.GetInvLists(mapBuckets);  //将所有字符集存入映射变量中 瓶颈所在
 iQuery.GetDocIdx(vecDocIdx);  //将倒排索引存入向量中  瓶颈所在
 CHzSeg iHzSeg;  //include ChSeg/HzSeg.h
 iQuery.m_sSegQuery = iHzSeg.SegmentSentenceMM(iDict, iQuery.m_sQuery); //将get到的查询变量分词分成 "我/  爱/  你们/ 的/  格式"
 vector vecTerm;
 iQuery.ParseQuery(vecTerm);  //将以"/"划分开的关键字一一顺序放入一个向量容器中
 set setRelevantRst; 
 iQuery.GetRelevantRst(vecTerm, mapBuckets, setRelevantRst); 
 gettimeofday(&end_tv,&tz);
 // search end
 //搜索完毕按照顺序我们首先深入进iQuery对象的类CQuery   
//Query.cpp
1、GetInputs
这个方法的功能是将前台get过来的变量转换到HtmlInputs结构体数组中如下例子和代码：
//假设前台查询的关键字是"1"着HtmlInputs中内容输出如下  //HtmlInputs[0].Name word  //HtmlInputs[0].Value 1  //HtmlInputs[1].Name www  //HtmlInputs[1].Value 搜索  //HtmlInputs[2].Name cdtype  //HtmlInputs[2].Value GB
/* 
 * Get form information throught environment varible.
 * return 0 if succeed, otherwise exit.
 */
/**
 * 程序翻译说明
 * 处理GET过来的表单
 *
 * @access  public
 * @return  string 0
 */
int CQuery::GetInputs()
{
    int i,j;
 char *mode = getenv("REQUEST_METHOD"); //返回环境变量的值 这里环境变量 REQUEST_METHOD 为 get 方法
    char *tempstr; //GET变量字符串或POST字符串内容
 char *in_line; 
 int length;  //GET变量串长度或POST内容长度
 cout << "Content-type: text/html\n\n";
 //cout << "Cache-Control: no-cache\n";
 //cout << "Expires: Tue, 08 Apr 1997 17:20:00 GMT\n";
 //cout << "Expires: 0\n";
 //cout << "Pragma: no-cache\n\n";
 cout << "\n";
 cout << "\n";
 //cout << "\n";
 //cout << "\n";
 //cout << "\n";
 cout << "\n";
 cout.flush(); //释放输出缓冲区 输出头部head和之前的html标签内容
 //cout <<"" << endl;
 if (mode==NULL) return 1;
 if (strcmp(mode, "POST") == 0) 
 {
  length = atoi(getenv("CONTENT_LENGTH")); //如果是POST方法着获得环境变量CONTENT_LENGTH的整型值
  if (length==0 || length>=256)
   return 1;
  in_line = (char*)malloc(length + 1);
  read(STDIN_FILENO, in_line, length);
  in_line[length]='\0';
 } 
 else if (strcmp(mode, "GET") == 0) 
 {
  char* inputstr = getenv("QUERY_STRING"); //如果是GET方法着获得环境变量QUERY_STRING的字符串值
  length = strlen(inputstr);
  if (inputstr==0 || length>=256)
   return 1;
  //获取get内容长度并把get ？后面的参数赋值给变量in_line
  in_line = (char*)malloc(length + 1);
  strcpy(in_line, inputstr); //小心溢出攻击
 }
 tempstr = (char*)malloc(length + 1); //获取post内容或get内容长度
 if(tempstr == NULL)
 {
  printf("\n");
  printf("\n");
  printf("Major failure #1;please notify the webmaster\n");
  printf("\n");
  fflush(stdout); //输出缓冲区
  exit(2); //错误返回
 }
 j=0;
 for (i=0; i char
   strcpy(HtmlInputs[HtmlInputCount].Name,tempstr);
   if (i == length - 1)
   {
    strcpy(HtmlInputs[HtmlInputCount].Value,"");
    HtmlInputCount++;
   }
   j=0;
  }
  else if ((in_line[i] == '&') || (i==length-1))
  {
   if (i==length-1)
   {
    if(in_line[i] == '+')tempstr[j]=' ';
    else tempstr[j] = in_line[i];
    j++;
   }
   tempstr[j]='\0';
   CStrFun::Translate(tempstr); //将URL编码形式的参数转换成字符型 %** -> char
   strcpy(HtmlInputs[HtmlInputCount].Value,tempstr);
   HtmlInputCount++;
   j=0;
  } 
  else if (in_line[i] == '+') 
  {
   tempstr[j]=' ';
   j++;
  }
  else 
  {
   tempstr[j]=in_line[i]; //组合get中的变量如word www cdtype
   j++;
  }
  //cout<";
  //cout<";
  //cout.flush();
 }
 /*
 for (int kk = 0; kk < HtmlInputCount ; ++kk )
 {
  cout<<"Name="<";
  cout<<"Value="<";
 }
 //假设前台查询的关键字是"1"输出如下
 //Name=word
 //Value=1
 //Name=www
 //Value= 搜索
 //Name=cdtype
 //Value=GB
 */
 if(in_line) free(in_line);
 if(tempstr) free(tempstr);
 return 0;
}
2、SetQuery
//Query.cpp
void CQuery::SetQuery()
{
 string q = HtmlInputs[0].Value;
 CStrFun::Str2Lower(q,q.size()); //大写变小写
 m_sQuery = q;  //准备查询关键字
}
3、SetStart
void CQuery::SetQuery()
{
 string q = HtmlInputs[0].Value;
 CStrFun::Str2Lower(q,q.size()); //大写变小写word变量里的值
 m_sQuery = q;  //设置查询关键字
}
4、GetInvLists
 bool CQuery::GetInvLists(map<string, string> &mapBuckets) const
{
 ifstream ifsInvInfo(INF_INFO_NAME.c_str(), ios::binary); //以二进制形式打开一个文件的输入流缓冲，INF_INFO_NAME在头文件Comm.h中定义了的， const string INF_INFO_NAME("./Data/sun.iidx");
 //倒排索引文件索引字和文档好之间有一个制表符"\t"
 //朱德  14383 16151 16151 16151 1683 207 6302 7889 8218 8218 8637
 //朱古力  1085 1222
 if (!ifsInvInfo) {
  cerr << "Cannot open " << INF_INFO_NAME << " for input\n";
  return false;
 }
 string strLine, strWord, strDocNum;
 //以行读取输入流缓冲到字符串对象strLine中并做处理
 while (getline(ifsInvInfo, strLine)) {
  string::size_type idx;
  string tmp;
  idx = strLine.find("\t");
  strWord = strLine.substr(0,idx);
  strDocNum = strLine.substr(idx+1);
  mapBuckets.insert(map<string,string>::value_type (strWord, strDocNum)); //倒排表二项二维表存入映射中
  /*
  map<string, string>::iterator iter;
  int kkk = 0;
  for (iter = mapBuckets.begin(); kkk != 10; ++iter)
  {
   cout<<iter->first<<"  "<<iter->second<<"<br>";
   ++kkk;
  }
  cout.flush();
  */
 }
 return true;
}
5、GetDocIdx
bool CQuery::GetDocIdx(vector &vecDocIdx) const
{
 ifstream ifs(DOC_IDX_NAME.c_str(), ios::binary); 
 //0  0  bc9ce846d7987c4534f53d423380ba70
 //1  76760 4f47a3cad91f7d35f4bb6b2a638420e5
 //2  141624 d019433008538f65329ae8e39b86026c
 if (!ifs) { 
  cerr << "Cannot open " << DOC_IDX_NAME << " for input\n"; //以二进制形式打开一个文件的输入流缓冲，DOC_IDX_NAME在头文件Comm.h中定义了的， const string INF_INFO_NAME("./Data/Doc.idx"); 
  return false; 
 } 
 string strLine, strDocid, strUrl; 
 while (getline(ifs,strLine)){
  DocIdx di;
  sscanf( strLine.c_str(), "%d%d", &di.docid, &di.offset ); //只保留了前面两项文档号和偏移量
  vecDocIdx.push_back(di); //导入结构体向量中
 }
 return true;
}
[复制](#)[搜索](#)
[复制](#)[搜索](#)
