# c++二维数组，vector初始化_用户5706034127_新浪博客
||分类：[cPlusPlus](http://blog.sina.com.cn/s/articlelist_5706034127_1_1.html)|
vector>
二维数组， 
vector::iterator
int_ite;
vector::iterator
string_ite;
//vector
v(n,i)形式，v包含n 个值为 i 的元素
vector
ivec(10,0);
//vector
v(v1)形式，v是v1 的一个副本
vector
ivec1(ivec);
//vector
v(n)形式，v包含n 个值初始化的元素
vector
ivec2(10);
//vector
v(n)形式，v包含n 个值初始化的元素
vector
svec(10);
//数组初始化vector
int
iarray[]={1,2,3,4,5,6,7,8,9,0};
//count：
iarray数组个数
size_t
count=sizeof(iarray)/sizeof(int);
//int数组初始化
ivec3
vector
ivec3(iarray,iarray+count);
//string数组初始化
svec1
string
word[]={"ab","bc","cd","de","ef","fe"};
//s_count：
word数组个数
size_t
s_count=sizeof(word)/sizeof(string);
//string数组初始化
svec1
vector
svec1(word,word+s_count);
//用
back_inserter 函数
vector
ivec4; //空对象
fill_n(back_inserter(ivec4),10,3);//10个3
填充ivec4.
}
C++
Reference对于memset的定义为：
void* memset ( void* ptr, intvalue, size_t num );
对int类型的内存区域进行初始化，ptr的类型是int*，举例说明一下:
1.  
将内存初始化为0
memset(ptr,0,sizeof(ptr));
2.
 将内存初始化为-1
memset(ptr,0xff,sizeof(ptr));
因为按照字节进行填充，那么value值为0xff，将会把内存的每个字节都填充为0xff，则int值的区域为全1，那么int值就是-1。
3.
 将内存初始化为极大值
memset(ptr,0x3f,sizeof(ptr));
这段初始化代码初始出来的int值大小是1061109567
使用memset初始化vector
vector
vec(10,1);
memset(vec.data(),0,vec.size()*sizeof(int));
