# C++中的显式构造函数 - 数据之美的博客 - CSDN博客
2017年03月08日 15:13:45[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：169
有如下一个简单的复数类：
class ClxComplex
{
public:
    ClxComplex(double dReal =0.0, double dImage =0.0) { m_dReal = dReal; dImage = dImage; }
double GetReal() const { return m_dReal; }
double GetImage() const { return m_dImage; }
private:
double m_dReal;
double m_dImage;
};
    我们知道，下面的3行代码是等价的：
ClxComplex lxTest =2.0;
ClxComplex lxTest = ClxComplex(2.0);
ClxComplex lxTest = ClxComplex(2.0, 0.0);
    其实，对于前两行来说，编译器都是把它们转换成第3行的代码来实现的。因为我们写了构造函数，编译器就按照我们的构造函数来进行隐式转换，直接把一个double数值隐式转换成了一个ClxComplex的对象。可是，有些时候，我们不希望进行隐式转换，或者隐式转换会造成错误。比如下面的一个简化的字符串类：
class ClxString
{
public:
    ClxString(int iLength);
    ClxString(constchar*pString);
~ClxString();
private:
char*m_pString;
};
ClxString::ClxString(int iLength)
{
if (iLength >0)
        m_pString =newchar[iLength];
}
ClxString::ClxString(constchar*pString)
{
    m_pString =newchar[strlen(pString)];
    strcpy(m_pString, pString);
}
ClxString::~ClxString()
{
if (m_pString != NULL)
delete m_pString;
}
    我们可以用字符串的长度来初始化一个ClxString的对象，但是我们却不希望看到下面的代码：
ClxString lxTest =13;  // 等同于ClxString lxTest = ClxString(13);
    这会给阅读代码造成不必要的歧义。
    还有，我们知道下面的代码是用字符串A来初始化一个ClxString的对象：
ClxString lxTest ="A";  // 等同于ClxString lxTest = ClxString("A");
    可是，如果有人写成：
ClxString lxTest ='A';  // 等同于ClxString lxTest = ClxString(65);
    那上面的代码就会初始化一个长度为65（字母A的ASCII码值，在C和C++中，字符是以ASCII值存储的）的字符串。
    当然，上面的情况都不是我们希望看到的。在这个时候我们就要用到显示构造函数了。
    将构造函数声明成**explicit**就可以防止隐式转换。
    下面是使用显示构造函数的ClxString：
class ClxString
{
public:
explicit ClxString(int iLength);
    ClxString(constchar*pString);
~ClxString();
private:
char*m_pString;
};
    在这种情况下，要想用字符串的长度来初始化一个ClxString对象，那就必须显示的调用构造函数：
ClxString lxTest = ClxString(13);
    而下面这些代码将不能通过编译。
ClxString lxTest =13; 
ClxString lxTest ='A';
