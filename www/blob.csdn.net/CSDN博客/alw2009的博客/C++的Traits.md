# C++的Traits - alw2009的博客 - CSDN博客





2015年11月12日 17:44:27[traveler_zero](https://me.csdn.net/alw2009)阅读数：169标签：[C++																[Traits](https://so.csdn.net/so/search/s.do?q=Traits&t=blog)](https://so.csdn.net/so/search/s.do?q=C++&t=blog)
个人分类：[技术](https://blog.csdn.net/alw2009/article/category/5947555)








1. Think of a trait as a small object whose main purpose is to carry information used by another object or algorithm to determine "policy" or "implementation
 details"  ------ Bjarne Stroustrup





2. The
 traits class is used in template code to reflect properties (traits) of the actual template argument.





- template <typename T>  
- struct TraitsHelper {  
- typedef T ret_type;  
- typedef T par_type;  
- };  
- template <>  
- struct TraitsHelper<int> {  
- typedefint ret_type;  
- typedefint par_type;  
- };  
- template <>  
- struct TraitsHelper<float> {  
- typedeffloat ret_type;  
- typedefint par_type;  
- };  





- template <typename T>  
- class Test {  
- public:  
-      TraitsHelper<T>::ret_type Compute(TraitsHelper<T>::par_type d);  
- private:  
-      T mData;  
- };  







