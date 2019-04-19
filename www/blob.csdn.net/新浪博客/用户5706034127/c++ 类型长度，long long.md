# c++ 类型长度，long long_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
int 4
long long 8
#include  
#include  
#include  
using namespace std;
int sizeoftype()
{
cout << "type: \t\t" <<
"************size**************" << endl;
cout << "bool: \t\t" <<
"所占字节数：" << sizeof(bool);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "char: \t\t" <<
"所占字节数：" << sizeof(char);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "signed char: \t"
<< "所占字节数：" << sizeof(signed char);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "unsigned char: \t"
<< "所占字节数：" << sizeof(unsigned char);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "wchar_t: \t"
<< "所占字节数：" << sizeof(wchar_t);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "short: \t\t"
<< "所占字节数：" << sizeof(short);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "int: \t\t" <<
"所占字节数：" << sizeof(int);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "unsigned: \t"
<< "所占字节数：" << sizeof(unsigned);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "long: \t\t" <<
"所占字节数：" << sizeof(long);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "unsigned long: \t"
<< "所占字节数：" << sizeof(unsigned long);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "double: \t" <<
"所占字节数：" << sizeof(double);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "long double: \t"
<< "所占字节数：" << sizeof(long double);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "float: \t\t"
<< "所占字节数：" << sizeof(float);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "size_t: \t" <<
"所占字节数：" << sizeof(size_t);
cout << "\t最大值：" <<
(numeric_limits::max)();
cout << "\t最小值：" <<
(numeric_limits::min)() << endl;
cout << "string: \t" <<
"所占字节数：" << sizeof(string) << endl;
// << "\t最大值：" <<
(numeric_limits::max)() << "\t最小值：" <<
(numeric_limits::min)() << endl;  
cout << "type: \t\t" <<
"************size**************" << endl;
cout << "long long: \t\t"
<< "所占字节数：" << sizeof(long long) << endl;;
cout << "long long: \t\t"
<< "所占字节数：" << sizeof(long int) << endl;;
return 0;
}
- #include 
- #include 
- #include 
- #include 
- 
usingnamespacestd; 
- 
- #define MAXN 9999
- #define MAXSIZE 10
- #define DLEN 4
- 
- 
classBigNum 
- 
{ 
- 
public: 
- inta[500]; //可以控制大数的位数 
- intlen; //大数长度
- 
public: 
- BigNum(){ len = 1;memset(a,0,sizeof(a)); } //构造函数
- BigNum(constint); //将一个int类型的变量转化为大数
- BigNum(constchar*); //将一个字符串类型的变量转化为大数
- BigNum(constBigNum &); //拷贝构造函数
- BigNum &operator=(constBigNum &); //重载赋值运算符，大数之间进行赋值运算
- 
- friendistream& operator>>(istream&, BigNum&); //重载输入运算符
- friendostream& operator<<(ostream&, BigNum&); //重载输出运算符
- 
- BigNum operator+(constBigNum &) const; //重载加法运算符，两个大数之间的相加运算 
- BigNum operator-(constBigNum &) const; //重载减法运算符，两个大数之间的相减运算 
- BigNum operator*(constBigNum &) const; //重载乘法运算符，两个大数之间的相乘运算 
- BigNum operator/(constint&) const; //重载除法运算符，大数对一个整数进行相除运算
- 
- BigNum operator^(constint&) const; //大数的n次方运算
- intoperator%(constint&) const; //大数对一个int类型的变量进行取模运算 
- booloperator>(constBigNum & T)const; //大数和另一个大数的大小比较
- booloperator>(constint& t)const; //大数和一个int类型的变量的大小比较
- 
- voidprint(); //输出大数
- 
}; 
- 
BigNum::BigNum(constintb) //将一个int类型的变量转化为大数
- 
{ 
- intc,d = b; 
- len = 0; 
- memset(a,0,sizeof(a)); 
- while(d > MAXN) 
- { 
- c = d - (d / (MAXN + 1)) * (MAXN + 1); 
- d = d / (MAXN + 1); 
- a[len++] = c; 
- } 
- a[len++] = d; 
- 
} 
- 
BigNum::BigNum(constchar*s) //将一个字符串类型的变量转化为大数
- 
{ 
- intt,k,index,l,i; 
- memset(a,0,sizeof(a)); 
- l=strlen(s); 
- len=l/DLEN; 
- if(l%DLEN) 
- len++; 
- index=0; 
- for(i=l-1;i>=0;i-=DLEN) 
- { 
- t=0; 
- k=i-DLEN+1; 
- if(k<0) 
- k=0; 
- for(intj=k;j<=i;j++) 
- t=t*10+s[j]-'0'; 
- a[index++]=t; 
- } 
- 
} 
- 
BigNum::BigNum(constBigNum & T) : len(T.len) //拷贝构造函数
- 
{ 
- inti; 
- memset(a,0,sizeof(a)); 
- for(i = 0 ; i < len ; i++) 
- a[i] = T.a[i]; 
- 
} 
- 
BigNum & BigNum::operator=(constBigNum & n) //重载赋值运算符，大数之间进行赋值运算
- 
{ 
- inti; 
- len = n.len; 
- memset(a,0,sizeof(a)); 
- for(i = 0 ; i < len ; i++) 
- a[i] = n.a[i]; 
- return*this; 
- 
} 
- 
istream& operator>>(istream & in, BigNum & b) //重载输入运算符
- 
{ 
- charch[MAXSIZE*4]; 
- inti = -1; 
- in>>ch; 
- intl=strlen(ch); 
- intcount=0,sum=0; 
- for(i=l-1;i>=0;) 
- { 
- sum = 0; 
- intt=1; 
- for(intj=0;j<4&&i>=0;j++,i--,t*=10) 
- { 
- sum+=(ch[i]-'0')*t; 
- } 
- b.a[count]=sum; 
- count++; 
- } 
- b.len =count++; 
- returnin; 
- 
- 
} 
- 
ostream& operator<<(ostream& out, BigNum& b) //重载输出运算符
- 
{ 
- inti; 
- cout << b.a[b.len - 1]; 
- for(i = b.len - 2 ; i >= 0 ; i--) 
- { 
- cout.width(DLEN); 
- cout.fill('0'); 
- cout << b.a[i]; 
- } 
- returnout; 
- 
} 
- 
- 
BigNum BigNum::operator+(constBigNum & T) const//两个大数之间的相加运算
- 
{ 
- BigNum t(*this); 
- inti,big; //位数 
- big = T.len > len ? T.len : len; 
- for(i = 0 ; i < big ; i++) 
- { 
- t.a[i] +=T.a[i]; 
- if(t.a[i] > MAXN) 
- { 
- t.a[i + 1]++; 
- t.a[i] -=MAXN+1; 
- } 
- } 
- if(t.a[big] != 0) 
- t.len = big + 1; 
- else
- t.len = big; 
- returnt; 
- 
} 
- 
BigNum BigNum::operator-(constBigNum & T) const//两个大数之间的相减运算 
- 
{ 
- inti,j,big; 
- boolflag; 
- BigNum t1,t2; 
- if(*this>T) 
- { 
- t1=*this; 
- t2=T; 
- flag=0; 
- } 
- else
- { 
- t1=T; 
- t2=*this; 
- flag=1; 
- } 
- big=t1.len; 
- for(i = 0 ; i < big ; i++) 
- { 
- if(t1.a[i] < t2.a[i]) 
- { 
- j = i + 1; 
- while(t1.a[j] == 0) 
- j++; 
- t1.a[j--]--; 
- while(j > i) 
- t1.a[j--] += MAXN; 
- t1.a[i] += MAXN + 1 - t2.a[i]; 
- } 
- else
- t1.a[i] -= t2.a[i]; 
- } 
- t1.len = big; 
- while(t1.a[len - 1] == 0 && t1.len > 1) 
- { 
- t1.len--; 
- big--; 
- } 
- if(flag) 
- t1.a[big-1]=0-t1.a[big-1]; 
- returnt1; 
- 
} 
- 
- 
BigNum BigNum::operator*(constBigNum & T) const//两个大数之间的相乘运算 
- 
{ 
- BigNum ret; 
- inti,j,up; 
- inttemp,temp1; 
- for(i = 0 ; i < len ; i++) 
- { 
- up = 0; 
- for(j = 0 ; j < T.len ; j++) 
- { 
- temp = a[i] * T.a[j] + ret.a[i + j] + up; 
- if(temp > MAXN) 
- { 
- temp1 = temp - temp / (MAXN + 1) * (MAXN + 1); 
- up = temp / (MAXN + 1); 
- ret.a[i + j] = temp1; 
- } 
- else
- { 
- up = 0; 
- ret.a[i + j] = temp; 
- } 
- } 
- if(up != 0) 
- ret.a[i + j] = up; 
- } 
- ret.len = i + j; 
- while(ret.a[ret.len - 1] == 0 && ret.len > 1) 
- ret.len--; 
- returnret; 
- 
} 
- 
BigNum BigNum::operator/(constint& b) const//大数对一个整数进行相除运算
- 
{ 
- BigNum ret; 
- inti,down = 0; 
- for(i = len - 1 ; i >= 0 ; i--) 
- { 
- ret.a[i] = (a[i] + down * (MAXN + 1)) / b; 
- down = a[i] + down * (MAXN + 1) - ret.a[i] * b; 
- } 
- ret.len = len; 
- while(ret.a[ret.len - 1] == 0 && ret.len > 1) 
- ret.len--; 
- returnret; 
- 
} 
- 
intBigNum::operator %(constint& b) const//大数对一个int类型的变量进行取模运算 
- 
{ 
- inti,d=0; 
- for(i = len-1; i>=0; i--) 
- { 
- d = ((d * (MAXN+1))% b + a[i])% b; 
- } 
- returnd; 
- 
} 
- 
BigNum BigNum::operator^(constint& n) const//大数的n次方运算
- 
{ 
- BigNum t,ret(1); 
- inti; 
- if(n<0) 
- exit(-1); 
- if(n==0) 
- return1; 
- if(n==1) 
- return*this; 
- intm=n; 
- while(m>1) 
- { 
- t=*this; 
- for( i=1;i<<1<=m;i<<=1) 
- { 
- t=t*t; 
- } 
- m-=i; 
- ret=ret*t; 
- if(m==1) 
- ret=ret*(*this); 
- } 
- returnret; 
- 
} 
- 
boolBigNum::operator>(constBigNum & T) const//大数和另一个大数的大小比较
- 
{ 
- intln; 
- if(len > T.len) 
- returntrue; 
- elseif(len == T.len) 
- { 
- ln = len - 1; 
- while(a[ln] == T.a[ln] && ln >= 0) 
- ln--; 
- if(ln >= 0 && a[ln] > T.a[ln]) 
- returntrue; 
- else
- returnfalse; 
- } 
- else
- returnfalse; 
- 
} 
- 
boolBigNum::operator >(constint& t) const//大数和一个int类型的变量的大小比较
- 
{ 
- BigNum b(t); 
- return*this>b; 
- 
} 
- 
- 
voidBigNum::print() //输出大数
- 
{ 
- inti; 
- cout << a[len - 1]; 
- for(i = len - 2 ; i >= 0 ; i--) 
- { 
- cout.width(DLEN); 
- cout.fill('0'); 
- cout << a[i]; 
- } 
- cout << endl; 
- 
} 
- 
intmain(void) 
- 
{ 
- inti,n; 
- BigNum x[101]; //定义大数的对象数组
- x[0]=1; 
- for(i=1;i<101;i++) 
- x[i]=x[i-1]*(4*i-2)/(i+1); 
- cin>>n; 
- x[n].print(); 
- x[n+1].print(); 
- (x[n]+x[n+1]).print(); 
- (x[n]-x[n+1]).print(); 
- (x[n]*x[n+1]).print(); 
- (x[n]/100).print(); 
- (x[n]^20).print(); 
- cout<<x[n]0<<endl; 
- return0; 
- 
}  
