# mwArray与C++接口 - wishchinYang的专栏 - CSDN博客
2014年07月11日 13:27:17[wishchin](https://me.csdn.net/wishchin)阅读数：3681
**1.Matlab调用C++**：[http://blog.csdn.net/zouxy09/article/details/20553007](http://blog.csdn.net/zouxy09/article/details/20553007)
摘录下效果图：
![](https://img-blog.csdn.net/20140305162005921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvem91eHkwOQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2.mwArray类操作**：[http://blog.sina.com.cn/s/blog_9460ff790101fujz.html](http://blog.sina.com.cn/s/blog_9460ff790101fujz.html)
2.1. 复数数组的初始化（由C到matlab）
    double rdata[4] = {1.0, 2.0, 3.0, 4.0};
    double idata[4] = {10.0, 20.0, 30.0, 40.0};
    mwArray a(2, 2, mxDOUBLE_CLASS, mxCOMPLEX);
    a.Real().SetData(rdata, 4);
    a.Imag().SetData(idata, 4);
   对应从mwArray中获取元素
    a.Real().GetData(buffer,len);
    a.Imag().GetData(buffer,len);
2.2. cell元组阵列的获取
    由于mwArray索引运算符()直接返回mwArray类型，因此可以直接声明通过索引获取元组内的矩阵值。
    eg: a={[1],[2],[1 2;3 4];[1 3;2 4],[1],[2]};
    mwArray b=a(2); //b=[1 3;2 4];
    mwArray c=a(5); //c=[1 2;3 4];
    PS:mwArray中数组为按列排，因此[1 3;2 4]的索引号为2
2.3. mwArray成员函数使用
    int NumberOfDimensions        返回矩阵维数
    int NumberOfElements            返回矩阵元素个数
    mwArray GetDimensions         返回一维矩阵，表示矩阵各维大小
    bool IsComplex                        判断是否复数矩阵
    PS：使用GetData前可先使用NumberOfElements确定元素个数，避免越界。
2.4. 字符串转换为mwArray
    char str[]="inFile.mat";
    mwArray inFile(str);
    则inFile则可以直接作为函数的输入参数进行传递。
更详细的说明见：[http://blog.sina.com.cn/s/blog_b3facf740101e3c7.html](http://blog.sina.com.cn/s/blog_b3facf740101e3c7.html)
**intNumberOfFields() const** 返回结构体域个数
**intElementSize() const** 返回mwArray阵列元素大小
**intCompareTo(const mwArray& arr)const**对比两个mwArray阵列
**boolIsSparse()const** 判断是否Sparse阵列
**boolIsNumeric()const** 判断是否是数值阵列
**boolIsEmpty()const** 判断是否是空阵列
**boolIsComplex()const** 判断是否复型阵列
**boolEquals(const mwArray& arr)const** 判断两个阵列是否相同
**3.我使用的代码**
//输入，文件名，用于载入文件
std::stringSFilename;
constmwArrayFilename(SFilename.c_str());mwArraytopic_numM(1,5,mxINT8_CLASS);doubleb[]={5，4,3,6,8};topic_numM.SetData(b,5);
//输出，用于获取数组
intOutNorNum=530;intOutabNorNum=33;mwArrayNormal;mwArrayx_Abnorm_index;
函数....................................
double*Ptrue=newdouble[OutNorNum];int*Indexfalse=newint[OutabNorNum];double*Pfalse=newdouble[OutNorNum];
Normal.GetData(Ptrue,OutNorNum);//此句导致程序崩溃！越界x_Abnorm_index.GetData(Indexfalse,OutabNorNum);//此句导致程序崩溃！超出下标
