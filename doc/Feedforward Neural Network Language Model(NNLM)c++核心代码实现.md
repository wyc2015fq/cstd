# Feedforward Neural Network Language Model(NNLM)c++核心代码实现

置顶 2015年03月29日 20:29:05 [a635661820](https://me.csdn.net/a635661820) 阅读数：3855



# *本文来自CSDN博客，转载请注明出处：http://blog.csdn.net/a635661820/article/details/44730507*



# 参考文献: A Neural Probabilistic Language Model





   参照我另一篇[NNLM学习介绍](http://blog.csdn.net/a635661820/article/details/44130285)的博客, 这一篇是对NNLM的简要实现, 自己简化了一些，输入层到输出层没有连接(加上直连边的真在原论文中没有明显的提高)，并且没有并行算法。下面贴上自己的一些核心代码。总体来说，我用了c++面向对象来设计该算法，大概分为6个类，如下： 



1. CLex类：用来处理文本的一个类
2. CInput类：输入层类, 包含相关量
3. CHidden类：隐层类，包含相关量
4. COutput类：输出层类，包含相关变量
5. CAlgothrim类：算法类，涉及前向算法以及反向更新算法，将前面3个类串起来
6. CForeCast类：测试类，训练好模型后对模型进行测试的相关操作



关于网络中的各个手动设定的参数在macroDefinition.h，包括隐层神经元个数、特征向量维度等。这里的附带的代码只贴出了核心相关代码，即CInput, CHidden, COutput, CAlgothrim的核心代码



网络手动设定的参数在macroDefinition.h里面，定义为宏，初始设置是按照论文中设置的：

```cpp
//以下是各个变量的宏定义，留做接口，方便调试  
# define M 100       //一个单词对应的向量的维度数为M  
# define N 3     //输入层的单词个数,第N+1个是预测单词  
# define HN 60       //Hidden Number隐层神经元的个数   
# define EPSILON 0.001       //神经网络的学习率  
# define CONVERGE_THRESHOLD -4       //手动设定的累加对数概率收敛值  
# define FOREWORD_NUM 5  //模型预测时的输出概率最高的前5个词语
```

在Input.h文件里面，类的结构：

```cpp


//输入层的结构定义  
class CInput    
{  
public:  
    bool LoadCorpusfile(char *corpusfile);      //将语料库文件读入  
    bool Computex(int k);                       //计算一个句子输入单词的x向量  
    string GetWordByID(unsigned long id);       //根据ID返回单词  
    unsigned long GetIDByWord(string word);     //根据单词返回其ID   
    bool NextSentence(int i);           //从语料库文件中读入下一个句子  
    CInput();                       //生成词典、二维矩阵  
    virtual ~CInput();          //释放单词映射矩阵,释放词典  

​    CLex *pChnLex;      //指向中文词典的指针  
​    float vec;        //输入单词对应的映射矩阵  
​    vector<string> sentence;      //一行句子  
​    vector<string> corpus;        //语料库  
​    float *x;       //保存输入层单词的特征向量  
​    long unsigned expectedID;       //训练句子时下一个输出单词ID  
};


类的实现在Input.cpp里面：





CInput::CInput()  
{  
    //构造函数  
    //生成词典、二维矩阵  
  
    pChnLex = new CLex;     //动态分配词典对象  
    if (NULL == pChnLex)        //分配失败  
    {  
        printf("动态分配词典失败！\n");  
        exit(1);          
    }  
              
    if (!pChnLex->LoadLexicon_Null("output.voc"))        //将指向中文词典的指针与词典文本库连接起来  
    {  
        printf("载入中文词典失败！\n");      //载入失败输出错误信息到屏幕  
    }  
  
    unsigned long sizeV = pChnLex->ulVocSizeC;       //记录词典的大小  
    srand(time(NULL));                                  //随机数种子  
  
    vec = new float *[sizeV];       //开始分配单词映射的二维矩阵  
    if (NULL == vec)        //分配失败  
    {  
        printf("分配单词映射矩阵失败！\n");  
        exit(1);  
    }  
    for (unsigned long i=1; i<sizeV; i++)        //初始化二维矩阵  
    {  
        vec[i] = new float[M];      //分配满足每个单词对应M维  
        if (NULL == vec[i])       
        {  
            printf("分配单词映射向量失败！\n");  
            exit(1);  
        }  
        for (int j=0; j<M; j++)          //赋值-0.5-0.5的小数  
        {  
            vec[i][j] = (float)(((rand()/32767.0)*2-1)/2);  
        }  
    }  
      
    x = new float[M*N];     //输入层单词的特征向量  
    if (NULL == x)  
    {  
        cerr << "输入层单词的特征向量分配失败" << endl;  
        exit(1);  
    }  
    memset(x, 0, M*N*sizeof(float));        //将x向量清0  
                                                                  
}  
  
CInput::~CInput()  
{  
    //析构函数  
    //释放单词映射矩阵  
    //释放中文词典  
  
    unsigned long sizeV = pChnLex->ulVocSizeC;       //记录词典的大小  
    for (unsigned long i=1; i<sizeV; i++)            //释放单词映射矩阵  
    {  
        delete [] vec[i];  
        vec[i] = NULL;  
    }  
    delete []vec;  
    vec = NULL;  
  
    delete pChnLex;     //释放中文词典  
    pChnLex = NULL;       
  
    delete [] x;        //释放x  
    x = NULL;  
  
}  
  
bool CInput::NextSentence(int i)  
{  
    //从预料中读取第i个句子  
    //并且将该句子中的单词以空格分隔存入容器  
  
    string line;          
    string word;  
      
    if (i >= corpus.size())      //错误  
    {  
        cerr << "!!!!!error 下标出界" << endl;  
        return false;  
    }  
    line = corpus[i];       //第i个句子  
  
    stringstream instring(line);        //字符流与该句子关联  
    sentence.clear();       //清空之前的句子  
      
    while (instring >> word)  
    {  
        sentence.push_back(word);       //把句子中的单词以空格隔开存入sentence  
    }  
      
    return true;  
}  
  
unsigned long CInput::GetIDByWord(string word)  
{  
    //根据单词返回其ID  
    return pChnLex->findword(word);  
}  
  
string CInput::GetWordByID(unsigned long id)  
{  
    //根据ID返回单词  
    return pChnLex->GetLexiconByID(id);  
}  
  
bool CInput::Computex(int k)  
{  
    //计算一个句子输入单词的x向量  
    //k用来控制多次训练一个句子  
  
    long unsigned id;  
    long unsigned Vsize = pChnLex->ulVocSizeC;       //词典大小  
    int i, j, t;  
    for (i=k,t=0; i<N+k; i++,t++)  
    {     
        if (i >= sentence.size())        //该句子训练完毕  
        {  
            return false;  
        }  
        id = GetIDByWord(sentence[i]);      //得到输入层第i个单词id    
        if (id >= Vsize)  
        {  
            cerr << "输入句子有误(可能是由于当前训练语料库过小,词库太少导致)" << endl;  
            exit(1);  
        }  
        for (j=0; j<M; j++)  
        {  
            x[M*t+j] = vec[id][j];            
        }  
    }  
  
    if (k+N >= sentence.size())      //该句子训练完毕  
    {  
        expectedID = 0;     //不会出现的ID号码标注  
        return false;  
    }  
    expectedID = GetIDByWord(sentence[k+N]);        //得到训练句子的输出单词ID  
}  
  
bool CInput::LoadCorpusfile(char *corpusfile)  
{  
    //将语料库文件读入内存  
  
    ifstream infile(corpusfile);        //关联欲读入的文件  
    if (!infile)        //读入失败  
    {  
        return false;  
    }  
  
    string line;          
    string word;  
  
    while (getline(infile, line))       //从语料库读入一行句子到容器  
    {  
        corpus.push_back(line);  
    }  
  
    infile.close();     //关闭文件    
      
    return true;  
      
}








//隐层的结构定义  
class CHidden    
{  
public:  
    CHidden();                  //生成输入层到隐层的权值矩阵,隐层神经元的输出向量,偏置向量  
    virtual ~CHidden();         //释放权值矩阵H、向量a,d  
    void OutHidden(float *x);       //计算隐层的输出  
  
    float H;      //由输入层到隐层的权值矩阵(变量名按照论文中，方便对照)  
    float *a;       //隐层神经元的输出向量  
    float *d;       //隐层的偏置向量  
  
};




隐层的结构定义在Hidden.h里面：



//隐层的结构定义  
class CHidden    
{  
public:  
    CHidden();                  //生成输入层到隐层的权值矩阵,隐层神经元的输出向量,偏置向量  
    virtual ~CHidden();         //释放权值矩阵H、向量a,d  
    void OutHidden(float *x);       //计算隐层的输出  
  
    float H;      //由输入层到隐层的权值矩阵(变量名按照论文中，方便对照)  
    float *a;       //隐层神经元的输出向量  
    float *d;       //隐层的偏置向量  
  
};


隐层的实现在Hidden.cpp：



CHidden::CHidden()  
{  
    //构造函数  
    //生成输入层到隐层的权值矩阵  
    //生成隐层神经元的输出向量  
    //生成隐层的偏置向量  
  
    H = new float *[HN];        //输入层到隐层的权值矩阵H  
    if (NULL == H)  
    {  
        cerr << "输入层到隐层的权值矩阵分配失败！" << endl;  
        exit(0);  
    }  
    int i, j;  
    for (i=0; i<HN; i++)  
    {  
        H[i] = new float [M*N];  
        if (NULL == H[i])  
        {  
            cerr << "输入层到隐层的权值矩阵分配失败！" << endl;  
            exit(0);  
        }  
    }     
  
    for (i=0; i<HN; i++)     //初始化矩阵H  
    {  
        for (j=0; j<M*N; j++)          
        {  
            H[i][j] = (float)(((rand()/32767.0)*2-1)/2);        //赋值-0.5-0.5的小数  
        }  
    }  
  
    d = new float[HN];      //隐层偏置向量  
    if (NULL == d)  
    {  
        cerr << "隐层偏置向量分配失败" << endl;  
        exit(1);  
    }  
    a = new float[HN];      //隐层输出向量  
    if (NULL == a)  
    {  
        cerr << "隐层输出向量分配失败" << endl;  
        exit(1);  
    }  
    for (i=0; i<HN; i++)  
    {  
        d[i] = (float)(((rand()/32767.0)*2-1)/2);       //赋值-0.5-0.5的小数  
        a[i] = 0;  
    }  
  
}  
  
CHidden::~CHidden()  
{  
    //释放权值矩阵H、向量a,d  
  
    int i;    
    for (i=0; i<HN; i++)     //释放H  
    {  
        delete [] H[i];  
        H[i] = NULL;  
    }  
    delete []H;  
    H = NULL;  
  
    delete []d;     //释放d  
    delete []a;     //释放a  
    d = NULL;  
    a = NULL;  
}  
  
void CHidden::OutHidden(float *x)  
{  
    //计算隐层输出  
    //x是由输入层传过来的输入层特征向量  
  
    int i, j;  
    float zigma, o;  
    for (i=0; i<HN; i++)     //根据o<-d + Hx  a<-tanh(o)计算a  
    {  
        zigma = 0.0;  
        for (j=0; j<M*N; j++)  
        {  
            zigma += x[j] * H[i][j];  
        }  
        o = d[i] + zigma;  
        a[i] = tanh(o);  
    }  
  
}


```

输出层的结构定义在Output.h：

```cpp
//输出层的结构定义  
class COutput    
{  
public:   
    void Initialize(long unsigned lexiconSize);     //生成隐层到输出层的权值矩阵,偏置向量、输出向量、输出单词概率向量  
    void SoftMax(void);                             //SotfMax回归层，进行归一化，计算概率  
    void Output(float *a, long unsigned expectedID);        //计算输出层的输出  
    COutput();                                        
    virtual ~COutput();             //释放权值矩阵U、向量b,y,p  
      
    float U;      //隐层到输出层的权值矩阵  
    float *y;       //输出层的输出  
    float *b;       //输出层神经元的偏置向量  
    float *p;       //输出单词概率向量  
    long unsigned VSize;        //语料库词典的大小  
    float L;        //累加的对数概率  
};


//输出层的实现在Output.cpp文件里面：

COutput::COutput()  
{     
    //do nothing  
}  
  
COutput::~COutput()  
{  
    //释放权值矩阵U、向量b,y,p  
      
    long unsigned i;      
    for (i=1; i<VSize; i++)      //释放H  
    {  
        delete [] U[i];  
        U[i] = NULL;  
    }  
    delete []U;  
    U = NULL;  
      
    delete []b;     //释放d  
    delete []y;     //释放a  
    delete []p;     //释放p  
    b = NULL;  
    y = NULL;  
    p = NULL;  
}  
  
void COutput::Output(float *a, long unsigned expectedID)  
{  
    //计算输出层的输出  
    //a,expectedID分别是隐层传的输出向量，句子的输出单词ID  
      
    long unsigned i;  
    int j;  
    float zigma;  
    for (i=1; i<VSize; i++)      //根据y<-b + Ua    
    {  
        zigma = 0.0;  
        for (j=0; j<HN; j++)  
        {  
            zigma += a[j] * U[i][j];  
        }  
        y[i] = b[i] + zigma;  
        p[i] = exp(y[i]);       //即计算e^yi         
    }  
  
    SoftMax();      //SotfMax回归计算概率  
  
    L += log(p[expectedID]);        //返回以e为底的对数值  
}  
  
void COutput::SoftMax()  
{  
    //SotfMax回归层，进行归一化，计算概率  
  
    float sum = 0;  
    long unsigned i;  
    for (i=1; i<VSize; i++)      //计算SoftMax分母  
    {  
        sum += p[i];  
    }  
    for (i=1; i<VSize; i++)      //计算每个单词的概率  
    {  
        p[i] /= sum;  
    }  
}  
  
void COutput::Initialize(unsigned long lexiconSize)  
{  
    //初始化输出层函数,为了程序构架好看方便，没放在构造函数里面  
    //生成隐层到输出层的权值矩阵  
    //输出层偏置向量、输出向量、输出单词概率向量  
      
    VSize = lexiconSize;        //保存语料库词典大小  
    U = new float *[lexiconSize];       //隐层到输出层的权值矩阵U,让其下标从1开始与词典ID对应  
    if (NULL == U)  
    {  
        cerr << "隐层到输出层的权值矩阵U分配失败！" << endl;  
        exit(0);  
    }  
    long unsigned int i, j;     //i尽量为lVsizeg型,与词典类型大小对应  
    for (i=1; i<lexiconSize; i++)  
    {  
        U[i] = new float[HN];  
        if (NULL == U[i])  
        {  
            cerr << "隐层到输出层的权值矩阵U分配失败！" << endl;  
            exit(0);  
        }  
    }  
      
    for (i=1; i<lexiconSize; i++)        //初始化矩阵U  
    {  
        for (j=0; j<HN; j++)       
        {  
            U[i][j] = (float)(((rand()/32767.0)*2-1)/2);        //赋值-0.5-0.5的小数  
        }  
    }  
      
    b = new float[lexiconSize];     //输出层偏置向量，让其下标从1开始与词典ID对应  
    if (NULL == b)  
    {  
        cerr << "输出层偏置向量分配失败" << endl;  
        exit(1);  
    }  
    y = new float[lexiconSize];     //输出层输出向量,让其下标从1开始与词典ID对应  
    if (NULL == y)  
    {  
        cerr << "输出层输出向量分配失败" << endl;  
        exit(1);  
    }  
    p = new float[lexiconSize];     //输出层输出向量,让其下标从1开始与词典ID对应  
    if (NULL == p)  
    {  
        cerr << "输出层概率向量分配失败" << endl;  
        exit(1);  
    }  
    for (i=1; i<lexiconSize; i++)        //初始化输出矩阵偏置向量、输出向量、概率向量  
    {  
        b[i] = (float)(((rand()/32767.0)*2-1)/2);       //赋值-0.5-0.5的小数  
        p[i] = 0;  
        y[i] = 0;  
    }  
      
    p[0] = 0;       //对后续排序起作用  
    L = 0;      //累加对数概率清0  
}


```



算法类的结构定义在Algothrim.h：

```cpp
//算法层的结构定义  
class CAlgothrim    
{  
public:  
    bool SaveParameters();      //保存网络全部参数到文件weight.txt中  
    void PrintOverInfo(long unsigned count, int sentNum);       //提示训练结束  
    void PrintStartInfo(char *corpusfile);      //打印该神经网络版本训练的相关信息  
    bool WriteResult();     //将输出层的概率向量结果输出到文件  
    bool Run(char *corpusFile);     //模型训练框架函数  
    void Initialize();      //生成中间梯度向量  
    void UpdateAll();       //反向算法更新网络所有参数  
    void PrintParametersAfterUpate();       //在反向更新参数后打印网络所有参数  
    void UpdateInput();     //更新输入层vec  
    void UpdateHidden();        //更新隐层的H,d  
    void PrintParametersBeforeUpdate();     //在反向更新之前输出整个网络的所有参数  
    void UpdateOut();       //更新输出层的参数U,b  
    CAlgothrim(CInput *pIn, CHidden *pHi, COutput *pOu);        //3个参数分别表示输入层、隐层、输出层的指针   
    virtual ~CAlgothrim();      //释放偏导向量  
  
    CInput *pInput;     //指向输入层的指针  
    CHidden *pHidden;       //指向隐层的指针  
    COutput *pOut;      //指向输出层的指针  
    float *Lpa;     //L对a的偏导向量  
    float *Lpx;     //L对x的偏导向量  
    float *Lpy;     //L对y的偏导向量  
  
};
```


算法类的实现在Algothrim.cpp，这里是程序的核心部分：



```cpp
CAlgothrim::CAlgothrim(CInput *pIn, CHidden *pHi, COutput *pOu)  
{  
    //构造函数  
    //3个参数分别表示输入层、隐层、输出层的指针   
  
    //获得各层指针  
    pInput = pIn;  
    pHidden = pHi;  
    pOut = pOu;   
  
}  
  
CAlgothrim::~CAlgothrim()  
{  
    //释放偏导向量  
  
    **delete** []Lpa;       //释放Lpa  
    Lpa = NULL;  
  
    **delete** []Lpx;       //释放Lpx  
    Lpx = NULL;  
  
    **delete** []Lpy;       //释放Lpy  
    Lpy = NULL;  
}  
  
void** CAlgothrim::UpdateOut()  
{  
    //更新输出层的参数U,b  
  
    //计算Lpy  
    **long** unsigned Vsize = pInput->pChnLex->ulVocSizeC;        //词典大小  
    **long** unsigned j;  
    **long** unsigned wt = pInput->expectedID;       //期望单词ID  
    **int** k;  
  
    memset(Lpa, 0, HN***sizeof**(**float**));       //Lpa置0  
  
    **for** (j=1; j<Vsize; j++)      //进行更新    
    {  
        **if** (j == wt)        //根据Lpy<-1(j==wt) - pj 计算Lpy  
        {  
            Lpy[j] = 1 - pOut->p[j];  
        }  
        **else**  
        {  
            Lpy[j] = 0 - pOut->p[j];  
        }  
  
        pOut->b[j] += EPSILON*Lpy[j];        //根据bj <- bj + ε*Lpy更新b  
          
        **for** (k=0; k<HN; k++)     //计算Lpa(累加),根据Lpa<-Lpa+Uj*Lpyj  
        {  
            Lpa[k] += Lpy[j]*pOut->U[j][k];  
        }  
  
        **for** (k=0; k<HN; k++)     //根据Uj<-Uj+ε*Lpy*a更新U  
        {  
            pOut->U[j][k] += EPSILON*Lpy[j]*pHidden->a[k];  
        }  
    }  
          
}  
**void** CAlgothrim::UpdateHidden()  
{  
    //更新隐层的H,d  
  
    **float** Lpo[HN] = {0.0};      //L对o的偏导向量  
    **int** k;  
    **for** (k=0; k<HN; k++)     //计算Lpo 根据lpo <- (1-ak2)lpak  
    {  
        Lpo[k] = (1 - pHidden->a[k]*pHidden->a[k]) * Lpa[k];  
    }  
  
    //累加计算Lpx  
    memset(Lpx, 0, N*M***sizeof**(**float**));      //置0  
    **int** i;  
    **float** temp;  
    **for** (i=0; i<M*N; i++)        //根据Lpx -< Lpx + H(转置)*Lpo 累加计算Lpx  
    {  
        temp = 0.0;  
        **for** (k=0; k<HN; k++)  
        {  
            temp += pHidden->H[k][i] * Lpo[k];  
        }  
        Lpx[i] = temp;  
    }  
  
    //更新d  
    **for** (k=0; k<HN; k++)     //根据d <- d + ε*Lpo更新d  
    {  
        pHidden->d[k] += EPSILON * Lpo[k];  
    }  
  
    //更新H  
    **for** (i=0; i<HN; i++)     //根据H <- H + ε*Lpo*x(转置)  
    {  
        **for** (k=0; k<M*N; k++)  
        {             
            pHidden->H[i][k] += EPSILON*Lpo[i]*pInput->x[k];  
        }  
    }  
  
}  
  
**void** CAlgothrim::UpdateInput()  
{  
    //更新输入层vec  
  
    **int** k, i;  
    **long** unsigned id;  
  
    **for** (i=0; i<N; i++)  
    {  
        id = pInput->GetIDByWord(pInput->sentence[i]);        //得到训练句子的第i个单词ID  
        **for** (k=0; k<M; k++)  
        {         
            pInput->vec[id][k] += EPSILON * Lpx[i*M+k];  
        }  
    }  
}  
**void** CAlgothrim::UpdateAll()  
{  
    //反向算法更新网络所有参数  
  
    UpdateOut();        //更新输出层  
    UpdateHidden();     //更新隐层  
    UpdateInput();      //更新输入层  
}  
  
**void** CAlgothrim::Initialize()  
{  
    //生成中间梯度向量  
      
    **long** unsigned Vsize = pInput->pChnLex->ulVocSizeC;        //得到词典大小  
      
    Lpa = **new** **float**[HN];        //L对a的偏导向量  
    **if** (NULL == Lpa)  
    {  
        cerr << "L对a的偏导向量分配失败！" << endl;  
        exit(1);  
    }  
    memset(Lpa, 0, HN***sizeof**(**float**));       //置0  
      
    Lpx = **new** **float**[N*M];       //L对x的偏导向量  
    **if** (NULL == Lpx)  
    {  
        cerr << "L对x的偏导向量分配失败！" << endl;  
        exit(1);  
    }  
    memset(Lpx, 0, N*M***sizeof**(**float**));      //置0  
      
    Lpy = **new** **float**[Vsize];     //L对y的偏导向量，下标从1开始与id对应  
    **if** (NULL == Lpy)  
    {  
        cerr << "L对y的偏导向量分配失败！" << endl;  
        exit(1);  
    }  
    memset(Lpy, 0, (Vsize)***sizeof**(**float**));      //置0  
}  
  
```


15/5/6补充：原来的函数run成如下：



```cpp
bool CAlgothrim::Train(char *corpusFile)
{
	//模型训练框架函数
	//参数为语料库文件名
	int i = 0;		//控制语料库句子的循环训练
	int k = 0;		//控制一个句子中循环训练
	unsigned int t = 0;		//控制整个语料库的训练次数
	int len;		//记录要训练句子的长度
	long unsigned count = 0;		//记录网络更新次数				
	pInput->LoadCorpusfile(corpusFile);		//载入语料库文件	
	PrintStartInfo(corpusFile);		//打印网络功能信息
	pOut->Initialize(pInput->pChnLex->ulVocSizeC);		//初始化输出层
	Initialize();		//生成中间梯度向量
	while (true)		
	{
		t++;		//记录对语料库训练次数
		pOut->L = 0;		//log10(Pwt) L是累加的对数概率
		for (i=0; i<pInput->corpus.size(); i++)		//遍历语料库的句子进行训练
		{
			//神经网络前馈运算
			pInput->NextSentence(i);		//从语料库中读取第i个句子做训练
			len = pInput->sentence.size();		//读入的句子长度
			for (k=0; k+N<len; k++)		//对一个句子反复训练N-len次
			{
				pInput->Computex(k);		//计算输入层的x的特征向量
				pHidden->OutHidden(pInput->x);		//计算隐层输出
				pOut->Output(pHidden->a, pInput->expectedID);		//计算输出层
				//	PrintParametersBeforeUpdate();		//打印未更新前的参数				
				//反向算法更新参数
				UpdateOut();		//更新输出层参数	
				UpdateHidden();		//更新隐层参数
				UpdateInput();		//更新输入层参数
				//	PrintParametersAfterUpate();		//打印更新后的参数								
				count++;
			}		
		}
		double PPL = pow(10, -pOut->L / pInput->runningWords);
		cout << t << "次训练，累加对数和概率: " << pOut->L << "训练集上的困惑度为" << PPL << endl;
		if (t % 5 == 0)		//每5次保存一下
		{
			SaveParameters();		//保存参数
		}
		if (MAX_TRAIN_EPOCH == t)		//等于最大训练遍数，停止训练
		{
			break;
		}
	}
	SaveParameters();		//保存参数	
	PrintOverInfo(count, pInput->corpus.size());
	return true;
}
```

15/5/6补充：

增加了测试PPL的函数：



```cpp
double CAlgothrim::TestPPL(char *testFile)
{
	//在测试集上计算PPL,返回PPL
	pInput->LoadCorpusfile(testFile);
	pOut->L = 0;		//log10(Pwt) L是累加的对数概率
	for (int i=0; i<pInput->corpus.size(); i++)		//遍历语料库的句子
	{
		//神经网络前馈运算
		pInput->NextSentence(i);		//从语料库中读取第i个句子做测试
		int len = pInput->sentence.size();		//读入的句子长度
		for (int k=0; k+N<len; k++)		//对一个句子N-len次测试
		{
			pInput->Computex(k);		//计算输入层的x的特征向量
			pHidden->OutHidden(pInput->x);		//计算隐层输出
			pOut->Output(pHidden->a, pInput->expectedID);		//计算输出层
			//cout << "prob: " << pOut->L << endl;
		}		
	}
	double PPL = pow(10, -pOut->L / pInput->runningWords);
	cout  << "在测试集上的累加对数和概率: " << pOut->L << "测试集上的困惑度为" << PPL << endl;
	return PPL;
}
```

为了方便，我把这个源码打包了，可以在下面的地址下载：

<http://download.csdn.net/detail/a635661820/8853783>