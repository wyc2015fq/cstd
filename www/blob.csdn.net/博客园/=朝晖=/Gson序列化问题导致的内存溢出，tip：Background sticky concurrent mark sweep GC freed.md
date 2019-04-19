# Gson序列化问题导致的内存溢出，tip：Background sticky concurrent mark sweep GC freed - =朝晖= - 博客园
# [Gson序列化问题导致的内存溢出，tip：Background sticky concurrent mark sweep GC freed](https://www.cnblogs.com/dhcn/p/7130805.html)
      问题原因，如果在json model里面放了非可序列化的对象就会导致这中问题，可序列化的就是那些基础数据类型和集合类型，如果在里面放个Android的Activity或者adapter这类类型字段，变量声明前面一定要加  
**transient**
否则就是长期GC提示。

