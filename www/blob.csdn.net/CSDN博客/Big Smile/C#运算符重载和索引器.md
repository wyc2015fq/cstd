# C#运算符重载和索引器 - Big Smile - CSDN博客
2015年10月18日 19:44:55[王啸tr1912](https://me.csdn.net/tr1912)阅读数：878
所属专栏：[C#学习基础](https://blog.csdn.net/column/details/csharp-wx.html)
# 一、运算符重载
        接着上一篇博客的叙述，我们看到了运算符的重载，重载在上一篇博客中讲过了，其实就是对一个方法的不同情况的处理过程。
        运算符的重载就是从新定义运算符的意义，总的来说就是让运算符的作用与原来的作用有稍许的不同，但是表示的方法还是一样的，需要注意的是，重载的逻辑运算符需要成对出现，如：
         ==和！=             > 和  <=     < 和  >=     等
举个例子：
```
private  const  double  Epsilon=0.0000001;
public  static  bool  operator  ==(vector  lhs, vector  rhs)
{
      if (system.math.abs(lhs.x-rhs.x)<epsilon&&system.math.abs(lhs.y-rhs.y)<Epsilon&&system.math.abs(lhs.z-rhs.z)<Epsilon)
          return  true;
     else
          return  false;
}
public  static bool  operator  !=(vector  lhs,vector  rhs)
{
       return  !(lhs==rhs);
}
```
        其中，lhs和rhs为操作符左右两边的操作数，而重新定义的正是两个操作数之间的关系，注意是这里是逻辑关系，不过具体的实现情况还是没有太弄清楚，只是知道了他能重写功能，并且只是在操作符的特征下重写，例如逻辑运算输出的结果只能是1或0等。
# **二、索引器**
        索引器通俗的来说就是一个声明对象的标号，以数组为例子，他既可以用里面所存储的数据来表示，又可以用数组的下标来表示这个对象，这就是索引器的作用。实际的用法就是：
```
[访问修饰符]数据类型 this [数据类型  标识符]
{
       get{};
       set{};
}
```
   其中get是用来获取索引以检索，set是用来获取索引以添加。
下面照片的例子非常形象：
```
class  Photo
{
       string_title;
       public  Photo(string  title)
       {
              this._title=title;
       }
       public  string  Title
       {
                get
                {
                      return _title;
                 }
       }
}
class  Album
{
         Photo[]  photos;
         public  Album (int  capacity)
         {
                photos =new  Photo[capacity];
         }
         public  Photo  this [index]
         {
                   get
                   {
                          if (index<0||index>=photos.Length)
                          {
                                console.WriteLine("索引无效")；
                                return   null；
                          }
                                return photos[index];
                   }
                   set
                   {
                           if(index<=0||index>=photos.Length)
                           {
                                    console.writeLine ("索引无效")；
                                    return;
                           }
                            photos[index]=value;
                   }
         }
}
static void  Main(string[] args)
{
       Album  friends = new Album(3);
       Photo  first =new Photo("Jenn");
       Photo second=new Photo("Smith");
       Photo  third=new  Photo("Mark");
       friends[0]=frist;
       friends[1]=second;
       friends[2]=third;
       Photo  obj1Photo =friends[2];
       console.writeline(obj1Photo.Title);
       Photo  obj2Photo =friends["Jenn"];
       console.writeline(obj2Photo.Title);
}
```
通过这个例子我们看出相册（album）这个类有个索引，就是声明的photo[index]，其中的index代表的就是照片对象的序号，查找照片的时候可以通过索引查，也可以通过名字查。这就是索引的作用。
# 三、小结
        通过对这两个例子具体应用的总结，了解了前面学习的内容应用和另外的一种使用方法，除此之外就是用this来使用索引，更是很稀奇，所以看来面向对象的编程我们还有很多的东西要学习啊。
