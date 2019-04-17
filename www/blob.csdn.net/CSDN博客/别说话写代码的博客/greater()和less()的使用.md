# greater()和less()的使用 - 别说话写代码的博客 - CSDN博客





2017年08月04日 10:02:34[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：560








# greater和less


greater和less是头文件<xfunctional>中定义的两个结构。下面看它们 的定义，greater和less都重载了操作符()。



**[cpp]**[view
 plain](http://blog.csdn.net/chijianxingfeng/article/details/8886399#)[copy](http://blog.csdn.net/chijianxingfeng/article/details/8886399#)



- // TEMPLATE STRUCT greater
- emplate<class _Ty>  
- struct greater  
-     : public binary_function<_Ty, _Ty, bool>  
- {   // functor for operator>
- bool operator()(const _Ty& _Left, const _Ty& _Right) const
-     {   // apply operator> to operands
- return (_Left > _Right);  
-     }  
- };  
- 
- // TEMPLATE STRUCT less
- emplate<class _Ty>  
- struct less  
-     : public binary_function<_Ty, _Ty, bool>  
- {   // functor for operator<
- bool operator()(const _Ty& _Left, const _Ty& _Right) const
-     {   // apply operator< to operands
- return (_Left < _Right);  
-     }  
- };  



在sort()函数中使用greater<int>()和less<int>(),



**[cpp]**[view
 plain](http://blog.csdn.net/chijianxingfeng/article/details/8886399#)[copy](http://blog.csdn.net/chijianxingfeng/article/details/8886399#)



- #include<iostream>
- #include<algorithm>//因为用了sort()函数
- #include<functional>//因为用了greater<int>()
- usingnamespace std;  
- 
- void main()  
- {  
- int a[]={3,1,4,2,5};  
- int i;  
- int len=sizeof(a)/sizeof(int);//这里切记要除以sizeof(int)!
- 
-     sort(a ,a + len, greater<int>());//内置类型的由大到小排序
- for(i=0;i<len;i++)  
-         cout<<a[i]<<" ";  
-     cout<<"\n";  
-     sort(a, a + len, less<int>());        //内置类型的由小到大排序
- for(i=0;i<len;i++)  
-         cout<<a[i]<<" ";  
- }  




