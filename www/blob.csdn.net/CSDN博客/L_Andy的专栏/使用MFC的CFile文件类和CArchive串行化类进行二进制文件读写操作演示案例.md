# 使用MFC的CFile文件类和CArchive串行化类进行二进制文件读写操作演示案例 - L_Andy的专栏 - CSDN博客

2014年04月22日 11:42:18[卡哥](https://me.csdn.net/L_Andy)阅读数：4760


                
一  编程案例介绍

   本案例基于控制台工程，创建专门的演示类CDemo，实现二进制文件的读写串行化操作，具体代码如下：

1 CDemo类定义：

class CDemo 

{

public:

    void CreateFile();

    CDemo();

    virtual ~CDemo();

    void Serialize(CArchive &ar);

    void WriteFile();

    void ReadFile();

    void Initlize();//初始化

private:

    //需要读写的演示数据：

    //基本类型数据

    int    nYear;

    char   chMonth;

    short  shDay;

    float  fSalary;

    //字符串数据

    CString m_strUserName;

    CString m_strPassWord;

    //文件相关的数据：

    CString  m_strFileName;//文件名

};

2 CDemo类接口实现定义：

CDemo::CDemo()

{

    //读写的文件名称

    m_strFileName = "data.dat";

}

CDemo::~CDemo()

{

}

//文件读写串行化函数

void CDemo::Serialize(CArchive &ar)

{

    if (ar.IsStoring())

    {

        //写变量的数据到文件

        ar<<nYear;

        ar<<chMonth;

        ar<<shDay;

        ar<<fSalary;

        ar<<m_strUserName;

        ar<<m_strPassWord;

    }

    else

    {

        //读文件内容并保存到变量：

        ar>>nYear;

        ar>>chMonth;

        ar>>shDay;

        ar>>fSalary;

        ar>>m_strUserName;

        ar>>m_strPassWord;

    }

}

//数据写文件

void CDemo::WriteFile()

{

    CFile file;

    if(file.Open(m_strFileName, CFile::modeWrite, NULL))//创建磁盘文件并打开

    {

        CArchive ar(&file, CArchive::store);//根据打开的文件，创建文件串行化对象

        Serialize(ar); //写文件内容

        //结束后关闭对象

        ar.Close();

        file.Close();

    }

}

//从文件上读数据

void CDemo::ReadFile()

{

    CFile file;

    if(file.Open(m_strFileName, CFile::modeRead, NULL))//打开磁盘文件

    {

        CArchive ar(&file, CArchive::load);//根据打开的文件，创建文件串行化对象

        Serialize(ar); //读文件内容

        //结束后关闭对象

        ar.Close();

        file.Close();

    }

}

//数据初始化：

void CDemo::Initlize()//初始化

{

    //初始化演示数据

    nYear = 2010;

    chMonth = 12;

    shDay  = 31 ;

    fSalary  = 10000.0;

     m_strUserName = "张三";

     m_strPassWord = "12345";

}

//创建二进制文件

void CDemo::CreateFile()

{

    CFile file;

    if(file.Open(m_strFileName, CFile::modeCreate, NULL))//创建磁盘文件并打开

    {

        file.Close();

    }

}

二 读写二进制文件的演示代码

  注：需要在VC++6.0环境下生成支持MFC的控制台工程！

下面蓝色部分是演示代码，经过调试测试，可以正确运行：

  int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])

{

    int nRetCode = 0;

    if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))

    {

        nRetCode = 1;

    }

    else

    {

        //演示文件读写：

        CDemo  demo1;//对象构造

        demo1.Initlize();//初始化数据

        //demo1.CreateFile();  //创建文件--第一次调用创建文件，后续可以把这行代码注释掉都可以。

        demo1.WriteFile();  //写文件内容

        CDemo  demo2;//对象构造

        demo2.ReadFile(); //从文件读数据，初始化对象Demo2的成员数据       

    }

    return nRetCode;

}
            

