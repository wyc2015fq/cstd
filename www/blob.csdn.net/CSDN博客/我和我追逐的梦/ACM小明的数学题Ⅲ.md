
# ACM小明的数学题Ⅲ - 我和我追逐的梦~~~ - CSDN博客


置顶2011年11月04日 20:01:39[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：1320标签：[string																](https://so.csdn.net/so/search/s.do?q=string&t=blog)[basic																](https://so.csdn.net/so/search/s.do?q=basic&t=blog)[stdstring																](https://so.csdn.net/so/search/s.do?q=stdstring&t=blog)[测试																](https://so.csdn.net/so/search/s.do?q=测试&t=blog)[output																](https://so.csdn.net/so/search/s.do?q=output&t=blog)[input																](https://so.csdn.net/so/search/s.do?q=input&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=output&t=blog)[
																					](https://so.csdn.net/so/search/s.do?q=测试&t=blog)个人分类：[C、C++开发																](https://blog.csdn.net/heyongluoyao8/article/category/908444)[算法设计与分析																](https://blog.csdn.net/heyongluoyao8/article/category/961150)[
							](https://blog.csdn.net/heyongluoyao8/article/category/908444)
[
				](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
			](https://so.csdn.net/so/search/s.do?q=测试&t=blog)
[
		](https://so.csdn.net/so/search/s.do?q=stdstring&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=basic&t=blog)
[
	](https://so.csdn.net/so/search/s.do?q=string&t=blog)
小明的数学题Ⅲ
Description
小明是个小学五年级的学生，为了早点去看自己爱看的卡通，他想快点把作业做完。可是可恶的数学老师今天却布置了一道难题，小明想了很久也不知道该怎么做。你的任务就是帮小明解决掉这道数学题。
题目是这样子的，有两个实数a，b，计算a/b，要求保留小数点后面n 位（0<=n<=100，四舍五入）。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例，每行有三个数a,b,n。a,b都是形如1.02或者2的数，不采用科学计数法表示，也不会有.5或者2.之类的方法表示。
输出：
每行输出一个测试用例的结果
Sample Input
2
5.0 2.0 4
23.1 4.7 10
Sample Output
2.5000
4.9148936170
\#include<iostream>
\#include<string>
using namespace std;
string addition( string A, string B );
string subtraction( string A, string B );
void eraser( string &in ); //去除多余的0 ( 除法中使用 )
long max_decimal=100;      //求商时小数位最大长度，-1表示没有限制
class BigDecimal
{
bool negative;  //标记数的正负性 正数0 负数1
string BDstr;   //记录标准化的大数字符串
long point;     //记录小数点的位置
public:
BigDecimal(){ negative=0;BDstr="";point=0; }
BigDecimal( string s ){ initBD( s ); }
void initBD( string s )
{
long i,lenI;
string In=s;
BDstr="";
if(In[0]=='-') { negative=1;In.erase(In.begin()); }
else if(In[0]=='+') { negative=0;In.erase(In.begin()); }
else { negative=0; }
lenI=In.size();
for(i=0;i<lenI;i++)
{
if(In[i]=='.'){ i++;break;}
BDstr.insert(BDstr.end(),In[i]);
}
point=lenI-i;
for(;i<lenI;i++) BDstr.insert(BDstr.end(),In[i]);
standard();//标准化
}
void standard()//去BDstr前后多余的0
{
long i,mid=BDstr.size()-point;
for(i=0;i<mid;i++)
{
if(BDstr[0]=='0') BDstr.erase(BDstr.begin());
else break;
}
for(i=0;i<point;i++)
{
long lenBD=BDstr.size();
if(BDstr[lenBD-1]=='0') BDstr.erase(BDstr.begin()+lenBD-1);
else break;
}
point-=i;
if(BDstr==""){ negative=0;point=0; }
}
void print()
{
standard();
// 输出前的准备
if( max_decimal ){
while( point <= max_decimal ){
BDstr += "0";
point++;
}
if( BDstr[BDstr.size()-1]>='5' ){
BDstr.erase(BDstr.end()-1);
BDstr = addition(BDstr,"1");
} else {
BDstr.erase(BDstr.end()-1);
}
point--;
}
if( point==BDstr.size() ){
BDstr.insert(BDstr.begin(),'0');
}
//cout << point << endl;
long i,lenBD=BDstr.size(),mid=lenBD-point;
if(BDstr==""){ putchar('0');return; }
if(negative==1) putchar('-');
//if(point==lenBD) putchar('0');
for(i=0;i<mid;i++) putchar(BDstr[i]);
if(i<lenBD)putchar('.');
for(;i<lenBD;i++) putchar(BDstr[i]);
}
//友元函数
friend BigDecimal BD_add( BigDecimal a, BigDecimal b );
friend BigDecimal BD_sub( BigDecimal a, BigDecimal b );
friend BigDecimal BD_multiply( BigDecimal a, BigDecimal b );
friend BigDecimal BD_divide( BigDecimal a, BigDecimal b );
//操作符重载 = + - * / (Simple)
};
BigDecimal BD_sub( BigDecimal a, BigDecimal b )
{
BigDecimal tmp=b;
tmp.negative=!tmp.negative;
return BD_add( a, tmp );
}
BigDecimal BD_add( BigDecimal a, BigDecimal b )
{
BigDecimal Res;
if( a.point>b.point ) while( a.point!=b.point ){ b.BDstr.append("0");b.point++; }//对齐小数部分
if( a.point<b.point ) while( a.point!=b.point ){ a.BDstr.append("0");a.point++; }
if( (a.negative==0&&b.negative==0) || (a.negative==1&&b.negative==1) )//同正或同负
{
Res.negative=a.negative;
Res.point=a.point;
Res.BDstr=addition(a.BDstr,b.BDstr);
}
else  //一正一负
{
if( a.BDstr.size()>b.BDstr.size() ||(a.BDstr.size()==b.BDstr.size()&&a.BDstr>b.BDstr) )
{
Res.negative=a.negative;
Res.point=a.point;
Res.BDstr=subtraction(a.BDstr,b.BDstr);
}
if( a.BDstr.size()<b.BDstr.size() ||(a.BDstr.size()==b.BDstr.size()&&a.BDstr<b.BDstr) )
{
Res.negative=b.negative;
Res.point=b.point;
Res.BDstr=subtraction(b.BDstr,a.BDstr);
}
}
Res.standard();
return Res;
}
BigDecimal BD_multiply( BigDecimal a, BigDecimal b )
{
BigDecimal Res;
Res.point=a.point+b.point;
if(a.negative==b.negative) Res.negative=false; //同号为正
else Res.negative=true;                        //异号为负
string basic[10];
basic[1]=a.BDstr;
int i,j,pos_basic=1,lenb=b.BDstr.size();
for(i=0;i<lenb;i++)
{
int digit=b.BDstr[lenb-1-i]-'0';
if(digit==0) continue;
if(digit>pos_basic)
for(j=pos_basic+1;j<=digit;j++) basic[j]=addition(basic[j-1],a.BDstr);
basic[0]=basic[digit];
for(j=0;j<i;j++) basic[0].append("0");
Res.BDstr=addition(Res.BDstr,basic[0]);
}
j=Res.point-Res.BDstr.size();
for(i=0;i<j;i++) Res.BDstr.insert(Res.BDstr.begin(),'0');
Res.standard();
return Res;
}
BigDecimal BD_divide( BigDecimal a, BigDecimal b )
{
BigDecimal Res;
if(a.negative==b.negative) Res.negative=false; //同号为正
else Res.negative=true;                        //异号为负
string tmp_a=a.BDstr,tmp_b=b.BDstr,divisor,tmp;
if(tmp_b=="") { cout << "Divisor is zero!" << endl; return Res; }
long point_a=a.point,point_b=b.point,i,j;
if(point_a>point_b) while(point_a!=point_b) { tmp_b.append("0");point_b++; }//对齐小数位
if(point_a<point_b) while(point_a!=point_b) { tmp_a.append("0");point_a++; }
eraser(tmp_a);eraser(tmp_b);
long append=tmp_a.size()-tmp_b.size();
j=append;
while( append>0 || ( append==0 && tmp_a>=tmp_b ) )//计算整数部分
{
divisor=tmp_b;
for(i=0;i<j;i++) divisor.append("0");
for(i=0;(tmp=subtraction(tmp_a,divisor))!="-";i++) {tmp_a=tmp;eraser(tmp_a);}
Res.BDstr.insert(Res.BDstr.end(),i+'0');
append=tmp_a.size()-tmp_b.size();
j--;
}
for(;j>=0;j--) Res.BDstr.append("0");
eraser(tmp_a);
divisor=tmp_b;
while( tmp_a!="0" && (Res.point<=max_decimal||max_decimal==-1) )//计算小数部分
{
tmp_a.append("0");
for(i=0;(tmp=subtraction(tmp_a,divisor))!="-";i++) {tmp_a=tmp;eraser(tmp_a);}
Res.BDstr.insert(Res.BDstr.end(),i+'0');
Res.point++;
}
Res.standard();
return Res;
}
string addition( string A, string B )
{
string Res;
int i,tmp=0,lenA=A.size(),lenB=B.size();
for(i=0;i<lenA||i<lenB;i++)
{
int a=0,b=0,add;
if(i<lenA) a=A[lenA-1-i]-'0';
if(i<lenB) b=B[lenB-1-i]-'0';
add=a+b+tmp;
tmp=add/10;
add%=10;
Res.insert(Res.begin(),add+'0');
}
if(tmp!=0) Res.insert(Res.begin(),tmp+'0');
return Res;
}
string subtraction( string A, string B )// TICE: numA >= numB ! 否则返回 "-"负号
{
string Res;
int i,tmp=0,lenA=A.size(),lenB=B.size();
if(lenA<lenB||(lenA==lenB&&A<B)) return "-";//返回 "-"负号
for(i=0;i<lenA||i<lenB;i++)
{
int a=0,b=0,sub;
if(i<lenA) a=A[lenA-1-i]-'0';
if(i<lenB) b=B[lenB-1-i]-'0';
sub=a+tmp-b;
if( sub >= 0 ){ Res.insert(Res.begin(),sub+'0');tmp=0; }
else { Res.insert(Res.begin(),sub+10+'0');tmp=-1; }
}
return Res;
}
BigDecimal BD_pow( BigDecimal BD, int n )
{
if(n==1) return BD;
return BD_multiply(BD_pow(BD,n-1),BD);
}
void eraser( string &in )
{
int i,len=in.size();
for(i=0;i<len-1;i++)
if(in[0]=='0') in.erase( in.begin() );
else break;
}
int main()
{
string a,b;
int n;
cin >> n;
BigDecimal BD1,BD2;
while( n-- )
{
cin >> a >> b >> max_decimal;
BD1.initBD(a);
BD2.initBD(b);
BD1 = BD_divide(BD1,BD2);
BD1.print();
cout << endl;
}
return 0;
}

[
](https://so.csdn.net/so/search/s.do?q=string&t=blog)
