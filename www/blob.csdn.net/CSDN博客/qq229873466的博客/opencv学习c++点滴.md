# opencv学习c++点滴 - qq229873466的博客 - CSDN博客

2016年10月03日 14:08:42[qq229873466](https://me.csdn.net/qq229873466)阅读数：221
个人分类：[VC应用																[C/C++](https://blog.csdn.net/qq229873466/article/category/6778966)](https://blog.csdn.net/qq229873466/article/category/5990495)


1.宏定义__BEGIN__、__END__、EXIT实现go exit跳转。

2.字符串连接。

```cpp
CvIntHaarClassifier* icvLoadTreeCascadeClassifier( const char* filename, int step,int* splits )
{
    ...
    sprintf( stage_name, "%s/", filename );
    suffix = stage_name + strlen( stage_name );
    for( i = 0; ; i++ )
    {
        sprintf( suffix, "%d/%s", i, CV_STAGE_CART_FILE_NAME );
        f = fopen( stage_name, "r" );
        if( !f ) break;
        fclose( f );
    }
    ...
}
```

3.选择性注释。

```cpp
#if 0
    ...
#else
    ...
#endif
```

4.加一指针跳转到结构体第二个元素的位置

```cpp
static CvIntHaarFeatures* icvCreateIntHaarFeatures( CvSize winsize,int mode, int symmetric )
{
   ...
   features = (CvIntHaarFeatures*) cvAlloc( sizeof( CvIntHaarFeatures ) +
        ( sizeof( CvTHaarFeature ) + sizeof( CvFastHaarFeature ) ) * seq->total );
   features->feature = (CvTHaarFeature*) (features + 1);
   ...
}
```

5.刷新输出缓冲区，立即输出

```cpp
fflush( stdout );
```






