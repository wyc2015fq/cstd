# 四道经典C语言指针试题 - 2019跟着小虎玩着去软考 - CSDN博客
2011年05月25日 14:32:00[littletigerat](https://me.csdn.net/littletigerat)阅读数：13521
**四道经典****C****语言指针试题**
# 试题一：
void GetMemory( char *p )
{
p = (char *) malloc( 100 );
}
void Test( void ) 
{
char *str = NULL;
GetMemory( str ); 
strcpy( str, "hello world" );
printf( str );
} 
## 解答：
本题中传入中GetMemory( char *p )**函数的形参为字符串指针**，在函数内部修改形参并不能真正的改变传入形参的值，执行完
char *str = NULL;
GetMemory( str );
后的str仍然为NULL；
存在内存泄露，打印为空
# 试题二：
char *GetMemory( void )
{ 
char p[] = "hello world"; 
return p; 
}
void Test( void )
{ 
char *str = NULL; 
str = GetMemory(); 
printf( str ); 
} 
## 解答：
char p[] = "hello world"; 
return p;
p[]数组为函数内的局部自动变量，在函数返回后，内存已经被释放。这是许多程序员常犯的错误，其根源在于不理解变量的生存期。
# 试题三：
void GetMemory( char **p, int num )
{
*p = (char *) malloc( num );
}
void Test( void )
{
char *str = NULL;
GetMemory( &str, 100 );
strcpy( str, "hello" ); 
printf( str ); 
} 
## 解答：
存在2处问题：
本题中的Test函数中**未对****malloc****的内存进行释放**。
本题中的GetMemory避免了试题一的问题，传入GetMemory的参数为字符串指针的指针，但是在GetMemory中执行申请内存及赋值语句
*p = (char *) malloc( num ); 
**后未判断内存是否申请成功**，应加上：
if ( *p == NULL )
{
...//进行申请内存失败处理
}
# 试题四：
void Test( void )
{
char *str = (char *) malloc( 100 );
strcpy( str, "hello" );
free( str ); 
... //省略的其它语句
} 
## 解答：
存在2处问题：
试题四存在与试题三同样的问题，在执行char *str = (char *) malloc(100); 
**后未进行内存是否申请成功的判断**；
另外，**在****free(str)****后未置str****为空，导致可能变成一个“野”指针**，应加上：
str = NULL;
