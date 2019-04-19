# Android Parcel理解 - xqhrs232的专栏 - CSDN博客
2011年12月11日 22:34:56[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：606标签：[android																[存储																[64bit																[byte																[float																[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=float&t=blog)](https://so.csdn.net/so/search/s.do?q=byte&t=blog)](https://so.csdn.net/so/search/s.do?q=64bit&t=blog)](https://so.csdn.net/so/search/s.do?q=存储&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android/Java](https://blog.csdn.net/xqhrs232/article/category/906925)
原文地址::[http://lisaguo.iteye.com/blog/920065](http://lisaguo.iteye.com/blog/920065)
Android Parcel理解 
2010-01-30 12:44 
android 中Parcel 的使用，他是一个存储基本数据类型和引用数据类型的容器，在andorid 中通过IBinder来绑定数据在进程间传递数据。 
Parcel parcel = Parcel.obtain();// 获取一个Parcel 对象 
下面就可以对其进行方法进行操作了，createXXX(),wirteXXX(),readXXX(), 
其中dataPosition(),返回当前Parcel 当前对象存储数据的偏移量，而setDataPosition(),设置当前Parcel 对象的偏移量，方便读取parcel 中的数据，可问题就出在我读取出来的数据要么是空（null）,要么永远是第一个偏移量处的值，存储和读取数据的。Parcel采用什么机制实现的，是以什么形式的存储的，然后我才能任意对其操作，读取目标数据。
基本数据类型的取值范围， 
boolean 1bit 
short 16bit 
int 32bit 
long 64bit 
float 32bit 
double 64bit 
char 16bit 
byte 8bit 
由此我可以猜想，Parcel 32bit 作为基本单位存储写入的变量,4byte*8=32bit,在内存中的引用地址变量是采用16进制进行编码，且作为偏移量，即偏移量是4的倍数，0,4,8,12,16,20,24,28,32,36,40,44,48......4*N,
f(x) = 4*y｛y>=0&y是自然数｝ 
我想绝对不会出现向偏移量是3，6，9这样的数据。。。 
由此我们可以推断出，无论他存储的是基本数据类型或引用数据类型的变量，都是以32bit基本单位作为偏移量， 
parcel.writeInt(1); 
parcel.writeInt(2); 
parcel.writeInt(3); 
parcel.writeInt(4); 
parcel.writeInt(5); 
parcel.writeInt(6); 
parcel.writeInt(7); 
parcel.writeInt(81011111); 
parcel.writeFloat(1f); 
parcel.writeFloat(1000000000000000000000000000000000000f); 
parcel.writeXXX(),每写一次数据，在32bit的空间里能够存储要放入的变量，怎只占一个偏移量，也就之一动4个位置，而当存储的数据如 parcel.writeFloat(1000000000000000000000000000000000000f);他就自动往后移动，
parcel.writeString("a"); 
parcel.writeString("b"); 
parcel.writeString("d"); 
parcel.writeString("c"); 
和 
parcel.writeString("abcd"); 的区别。有此可见，他的内存的分配原来是这样的。 
那我怎样才能把我存进去的书据依次的去出来呢？setDataPosition(),设置parcel 的偏移量，在readXXX(),读取数据 
int size = parcel.dataSize(); 
int i = 0; 
while (i <= size ) { 
parcel.setDataPosition(i); 
int curr_int = parcel.readInt(); 
i+=4; 
int j = 0; 
j++; 
} 
由此可见parcel 写入数据是按照32bit 为基本的容器，依次存储写入的数据，基本和引用（其实引用的也是有多个基本数据类型组合而成OBJECTS－属性｜方法），读取的时候我们就可以按照这种规律根据目标数据的偏移量的位置（curr_position），以及偏移量的大小(size)，,取出已经存进去的数据了
int i ＝ curr_position； 
while (i <= size ) { 
parcel.setDataPosition(i); 
int curr_int = parcel.readXXXt(); 
i+=4; 
int j = 0; 
j++; 
} 
这样就ok 了 
他的createXXX（）方法现在没用，用了在说吧！ 
总结一句话，java 中 基本数据类型的取值范围，引用类型的数据，相当于c中的指针，以及各进制之间的相互转换和灵活的引用，以及定制自己想要的任意进制数据类型。 
