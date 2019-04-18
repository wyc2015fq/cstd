# OOC 面向对象 C 语言编程实践 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [scottcgi](http://www.jobbole.com/members/scottcgi) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
面向对象是一种编程思想，虽然C并没有提供面向对象的语法糖，但仍然可以用面向对象的思维来抽象和使用。这里分享一套C面向对象的写法，可以完成面向对象编程并进行流畅的抽象。这套写法是在实践中不断调整的结果，目前已经比较稳定，进行了大量的功能编写。
这套OOC有以下特性：
- 没有强行去模仿c++的语法设定，而是使用C的特性去面向对象设计
- 实现继承，组合，封装，多态的特性
- 一套命名规范去增加代码的可读性
# 第一，封装
在C中可以用struct来封装数据，如果是方法，我们就需要用函数指针存放到struct里面来模拟。

```
typedef struct Drawable Drawable;
struct  Drawable
{
	 float positionX;
	 float positionY;
};
```


```
typedef struct {
	Drawable* (*Create)                ();
	void      (*Init)                  (Drawable* outDrawable);
}
_ADrawable_;
extern _ADrawable_ ADrawable[1];
```


```
static void InitDrawable(Drawable* drawable)
{
     drawable->positionX = 0.0f;
     drawable->positionY = 0.0f;
}
static Drawable* Create()
{
	Drawable* drawable = (Drawable*) malloc(sizeof(Drawable));
	InitDrawable(drawable);
	return drawable;
}
static void Init(Drawable* outDrawable)
{
	InitDrawable(outDrawable);
}
_ADrawable_ ADrawable[1] =
{
	Create,
	Init,
};
```
- 数据我们封装在Drawable结构里，通过Create可以再堆上创建需要自己free，Init是在栈上创建
- 函数封装在ADrawable这个全局单例对象里，由于没有this指针，所有方法第一个参数需要传入操作对象
- Create和Init方法将会管理，对象的数据初始化工作。如果对象含有其它对象，就需要调用其Create或Init方法
# 第二，继承和组合


```
typedef struct Drawable Drawable;
struct  Drawable
{
	 Drawable* parent;
	 Color     color[1];
};
```
- 继承，就是在结构体里，嵌入另一个结构体。这样malloc一次就得到全部的内存空间，释放也就一次。嵌入的结构体就是父类，子类拥有父类全部的数据空间内容。
- 组合，就是在结构体，存放另一个结构体的指针。这样创建结构体时候，要需要调用父类的Create方法去生成父类空间，释放的时候也需要额外释放父类空间。
- 这里parent就是组合，color就是继承。
- 继承是一种强耦合，无论如何子类拥有父类全部的信息。
- 组合是一种低耦合，如果不初始化，子类只是存放了一个空指针来占位关联。
- 可以看到，C里面一个结构体可以，继承任意多个父类，也可以组合任意多个父类。
- color[1] 使用数组形式，可以直接把color当做指针使用
子类访问父类，可以直接通过成员属性。那么如果通过父类访问子类呢 ?  通过一个宏定义来实现这个功能。


```
/** 
 * Get struct pointer from member pointer 
 */  
#define StructFrom2(memberPtr, structType) \  
    ((structType*) ((char*) memberPtr - offsetof(structType, memberPtr)))  
  
  
/** 
 * Get struct pointer from member pointer 
 */  
#define StructFrom3(memberPtr, structType, memberName) \  
    ((structType*) ((char*) memberPtr - offsetof(structType, memberName)))
```


```
typedef struct Sprite Sprite;
struct  Sprite
{
	Drawable drawable[1];
};
```


```
Sprite* sprite = StructFrom2(drawable, Sprite);
```
这样，我们就可以，通过Sprite的父类Drawable属性，来获得子类Sprite的指针。其原理，是通过offsetof获得成员偏移量，然后用成员地址偏移到子类地址上。有了这个机制，我们就可以实现多态，接口等抽象层。
我们可以在接口函数中，统一传入父类对象，就可以拿到具体的子类指针，执行不同的逻辑，让接口方法体现出多态特性。
# 第三， 多态


```
typedef struct Drawable Drawable;
struct  Drawable
{
	 /** Default 0.0f */
	 float     width;
	 float     height;
	 /**
	  * Custom draw called by ADrawable's Draw, not use any openGL command
	  */
	 void (*Draw)  (Drawable* drawable);
};
```
当，我们把一个函数指针放入，结构体对象的时候。意味着，在不同的对象里，Draw函数可以替换为不同的实现。而不是像在ADrawable里面的函数只有一个固定的实现。在子类继承Drawable的时候，我们可以给Draw赋予具体的实现。而统一的调用Draw(Drawable* drawable)的时候，就会体现出多态特性，不同的子类有不懂的实现。

```
typedef struct
{
    Drawable drawable[1];
}
Hero;
```


```
typedef struct
{
    Drawable drawable[1];
}
Enemy;
```


```
Drawable drawables[] =   
{  
     hero->drawable,  
     enemy->drawable,  
};  
  
for (int i = 0; i < 2; i++)  
{  
    Drawable* drawable = drawables[i];  
    drawable->Draw(drawable);  
}
```
 在Hero和Enemy的Create函数中，我们分别实现Draw(Drawable* drawable)函数。如果，我们有一个绘制队列，里面都是Drawable对象。传入Hero和Enemy的Drawable成员属性。在统一绘制调用中，drawable->Draw(drawable)，就会分别调用Hero和Enemy不同的draw函数实现，体现了多态性。
# 第四，重写父类方法
 在继承链中，我们常常需要重写父类方法，有时候还需要调用父类方法。

```
typedef struct
{
    Sprite sprite[1];
}
SpriteBatch;
```
比如，SpriteBatch 继承 Sprite， 我们需要重写Draw方法，还需要调用Sprite的Draw方法。那么我们就需要把Sprite的Draw方法公布出来。

```
typedef struct
{
   Drawable drawable[1];
}
Sprite;
typedef struct
{
    void (*Draw)(Drawable* drawable);
}
_ASprite_;
extern _ASprite_ ASprite;
```
这样，每个Sprite的Draw方法可以，通过ASprite的Draw访问。

```
// override
static void SpriteBatchDraw(Drawable* drawable)
{
      // call father
      ASprite->Draw(drawable);
}
spriteBatch->sprite->drawable->Draw = SpriteBatchDraw;
```
那么，SpriteBatch就重写了父类的Draw方法，也能够调用父类的方法了。
# 第五，内存管理
 一个malloc对应一个free，所以Create出来的对象需要自己手动free。关键是，在于组合的情况。就是对象内有别的对象的指针，有些是自己malloc的，有些是共用的。其实，计数器是一个简单的方案，但我仍然觉得复杂了。在想到更好的方案之前，我倾向于更原始的手动方法，给有需要内存管理的对象添加Release方法。

```
typedef struct
{
    Drawable* parent;
    Array*    children;
}
Drawable;
typedef struct
{
    void (*Release)(Drawable* drawable);
}
_ADrawable_;
extern _ADrawable_ ADrawable;
```
Drawable 含有两个指针， 一个是parent可能别的对象也会使用，所以这个parent在Release函数中不能确定释放。还有一个children这个数组本身是可以释放的，所以在Create函数里，我们自己malloc的，都要在Release方法里自己free。
所以，对于Create方法我们需要free + Release。对于Init 只需要调用Release方法就可以释放完全了。那么，parent这种公用的指针，就需要paren对象自己在合适的时机去释放自己。肯定没有计数器来的方便，但是这个足够简单开销也很小。
