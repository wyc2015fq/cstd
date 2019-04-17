# stdlib库中qsort函数的使用 - 在思索中前行！ - CSDN博客





2014年06月16日 20:43:14[_Tham](https://me.csdn.net/txl16211)阅读数：728








qsort ：功 能: 使用快速排序例程进行排序




用 法: void qsort(void *base, int nelem, int width, int (*fcmp)(const void *,const void *)); 　　

各参数：1 待排序数组首地址 2 数组中待排序元素数量 3 各元素的占用空间大小 4 指向函数的指针


用于确定排序的顺序 排序方法有很多种， 选择排序，冒泡排序，归并排序，快速排序等。 快速排序比选择排序，冒泡排序都要快。这是因为他速度很快，所以系统也在库里实现这个算法，便于我们的使用。 这就是qsort。

qsort 要求提供一个 比较函数，是为了做到通用性更好一点。比如你不仅仅的是要排序一个数字而已，可能你要用来排序几个数字 ，比如有一个结构 struct  num {


           int a;


          int b; 


} ; 


然后我有一个num 类型的数组， num dddd[100]; 我想给 dddd这个数组排序，那怎么办？ 我想让 a +b 最大的num元素排在数组的最前面，那又怎么办？ 这都可以通过定义比较函数来做到的。 比较函数的作用就是给qsort指明 元素的大小是怎么比较的。 


像这样的比较函数 inline int MyCmp(const void* a, const void* b) 都是有两个元素 作为参数，返回一个int 值， 如果 比较函数返回大于0，qsort就认为 a>b , 如果比较函数返回等于0 qsort就认为a 和b 这两个元素相等，返回小于零 qsort就认为 ab),你比较函数却返回一个 -1 （小于零的）那么qsort认为a<本文中排序都是采用的从小到大排序>

一、对int类型数组排序


int num[100];


Sample: 


int cmp ( const void *a , const void *b )


{ return *(int *)a - *(int *)b; }


qsort(num,100,sizeof(num[0]),cmp);

二、对char类型数组排序（同int类型）


char word[100];


Sample: int cmp( const void *a , const void *b )


{ return *(char *)a - *(char *)b; }


qsort(word,100,sizeof(word[0]),cmp);

三、对double类型数组排序（特别要注意）


double in[100];


int cmp( const void *a , const void *b )


{ return *(double *)a > *(double *)b ? 1 : -1; }


qsort(in,100,sizeof(in[0]),cmp)；

四、对结构体一级排序


struct In { double data; int other; }s[100]


//按照data的值从小到大将结构体排序,关于结构体内的排序关键数据data的类型可以很多种，


//参考上面的例子写


int cmp( const void *a ,const void *b)

{ return (*(In *)a).data > (*(In *)b).data ? 1 : -1; }


qsort(s,100,sizeof(s[0]),cmp);

五、对结构体二级排序


struct In { int x; int y; }s[100];


//按照x从小到大排序，当x相等时按照y从大到小排序

int cmp( const void *a , const void *b )

{

struct In *c = (In *)a; struct In *d = (In *)b;

if(c->x != d->x)

          return c->x - d->x;

else

          return d->y - c->y;

}


qsort(s,100,sizeof(s[0]),cmp);

六、对字符串进行排序


struct In { int data; char str[100]; }s[100];


//按照结构体中字符串str的字典顺序排序


int cmp ( const void *a , const void *b )


{

return strcmp( (*(In *)a)->str , (*(In *)b)->str );


}


qsort(s,100,sizeof(s[0]),cmp);

七、计算几何中求凸包的cmp


int cmp(const void *a,const void *b)


//重点cmp函数，把除了1点外的所有点，旋转角度排序


{


struct point *c=(point *)a;


struct point *d=(point *)b;


if( calc(*c,*d,p[1]) < 0)


         return 1;


else if( !calc(*c,*d,p[1]) && dis(c->x,c->y,p[1].x,p[1].y) < dis(d->x,d->y,p[1].x,p[1].y))


//如果在一条直线上，则把远的放在前面


        return 1;


else return -1;


}



