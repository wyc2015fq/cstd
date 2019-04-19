# 【Offer2012】C++ this指针|| - 三少GG - CSDN博客
2012年02月19日 13:12:54[三少GG](https://me.csdn.net/scut1135)阅读数：706
                
1. this指针的用处:
  一个对象的this指针并不是对象本身的一部分，不会影响sizeof(对象)的结果。this作用域是在类内部，当在类的非静态成员函数中访问类的非静态成员的时候，编译器会自动将对象本身的地址作为一个隐含参数传递给函数。也就是说，即使你没有写上this指针，编译器在编译的时候也是加上this的，它作为非静态成员函数的隐含形参，对各成员的访问均通过this进行。
   例如，调用date.SetMonth(9) <===> SetMonth(&date, 9)，this帮助完成了这一转换
2. this指针的使用:
  一种情况就是，在类的非静态成员函数中返回类对象本身的时候，直接使用 return *this；另外一种情况是当参数与成员变量名相同时，如this->n = n （不能写成n = n）。
3. this指针程序示例:
  this指针存在于类的成员函数中,指向被调用函数所在的类实例的地址。
  根据以下程序来说明this指针
   #include<iostream.h>
   class Point
   {
   int x, y;
   public:
   Point(int a, int b) { x=a; y=b;}
   void MovePoint( int a, int b){ x+=a; y+=b;}
   void print(){ cout<<"x="<<x<<"y="<<y<<endl;}
   };
   void main( )
   {
   Point point1( 10,10);
   point1.MovePoint(2,2);
   point1.print( );
   }
   当对象point1调用MovePoint(2,2)函数时，即将point1对象的地址传递给了this指针。
   MovePoint函数的原型应该是 void MovePoint( Point *this, int a, int b);第一个参数是指向该类对象的一个指针，我们在定义成员函数时没看见是因为这个参数在类中是隐含的。这样point1的地址传递给了this，所以在MovePoint函数中便显式的写成：
   void MovePoint(int a, int b) { this->x +=a; this-> y+= b;}
   即可以知道，point1调用该函数后，也就是point1的数据成员被调用并更新了值。
   即该函数过程可写成 point1.x+= a; point1. y + = b;
4. 关于this指针的一个经典回答:
  当你进入一个房子后，
   你可以看见桌子、椅子、地板等，
  但是房子你是看不到全貌了。
  对于一个类的实例来说，
  你可以看到它的成员函数、成员变量，
  但是实例本身呢？
  this是一个指针，它时时刻刻指向你这个实例本身
意义： this的意义是默认指针，指向this存在的那个类。
用法： 如：
class point
{
public:
     point(float x,float y)
{
     this->x=x;       //this->x 表示private中声明的 x；x 表示构造函数point(float x,float y)中的 x。
     this->y=y;      //this->y 表示private中声明的 y；y 表示构造函数point(float x,float y)中的 y。
}
private:
     float x,y;
} 
这个只能在类的成员函数中用，就是类本身的指针
如：类有成员变量a， a可以表示成this->a
      其成员函数 b();   可以表示成this->b();
你如果理解它是指向自己的指针就OK了，跟指向其他的指针用法一样，就是只能在类内使用 
楼上的回答的很好，另外加一点，对于非静态成员函数，他有一个隐含的参数就是this指针。
所以如果有A::b(int,int)这个函数，系统调用时，其实会这样调用b(int,int,this)，其中this就是指向A类的一个对象的指针。
**static成员函数是没有隐含this指针这个参数的，原因是static函数是类的函数，与类的具体对象无关，所以自然不会传this指针给这种类型的函数。**
