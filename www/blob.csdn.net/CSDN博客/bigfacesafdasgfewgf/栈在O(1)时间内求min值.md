# 栈在O(1)时间内求min值 - bigfacesafdasgfewgf - CSDN博客





2015年02月23日 17:30:35[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2044








**思路：1：在stack的数据结构中加两个个字段，如 typedef struct { **

**       int data[MAX];   // 存储栈中的所有数       int top;   // 存储栈顶元素在data数组中的位置       int min;  // 最小值       int second;  // 次小值}stack; pop,push的时候都去栈顶元素，所以是O(1) min的时候取stack的min字段，所以也是O(1) 每次push的时候进行比较，如果当前push的元素比stack->min小，则用当前元素替换stack->min，用原来的min替换second。如果当前push的元素比stack->min大，但比second小，则用当前元素替换stack->second。于是达到了取min的效果，程序如下 int push(stack * s,int x){        ASSERT(s!=NULL);        if(s->top>=MAX) // 防止栈溢出       {               printf("Stackoverload!");               return -1;        }        s->data[s->top++]=x;  // push的基本功能，把x压入栈，栈顶元素的位置自增1       if(x < s->min)  // 如果新压入的x比当前min小，则替换mins->second = s->min;               s->min=x; **





**       else if(x< s->second) //否则，如果x还比second小，这替换second**



**              s->second= x;// 否则，min和second都不变       return 0; } 每次pop的时候进行比较，如果pop的元素为min，则用second替换min（这里用到了second）。于是达到了取min的效果，程序如下 int pop(stack *s,int *x) {        ASSERT(s!=NULL);        if(s->top<=0)        {               printf("StackEmpty!");               return -1;        }        (*x) =s->data[s->top--];   // pop的基本功能，把x出栈，栈顶元素的位置自减1       if(x==s->min)  // 如果x正好等于min，那么就要把second给min**

**              s->min=s->second;        return 0; } int min( stack *s,int *x ) {        ASSERT(s!=NULL);        (*x)=s->min;        return 0; } **

** 思路2：设置辅助栈ass,记录每个状态下的最小值，每次插入时，得到辅助栈当前值，和插入的值比较如果小则插入到最小值栈的就是它，否则就是原来的最小值，通过这种方式，pop，push，min三个都是O(1)算法的。 **

**    如果还需要求最大值，那么就另外再设置一个ass2来记录当前栈的最大值。typedef struct { int data[MAX]; int top; }stack; STATIC int push_stack(stack *s,int x) { ASSERT((s!=NULL)); if(s->top>=MAX) { printf("Stack overload"); return -1; } s->data[s->top++]=x; return 0; } STATIC int pop_stack(stack *s,int *x) { ASSERT(s!=NULL); if(s->top<=0) { printf("Stack Empty"); return -1; } (*x)=s->data[s->top--]; return 0; } int push(stack *main,stack *ass,int x) { ASSERT((main!=NULL)&&(ass!=NULL)); int temp; pop_stack(ass,&temp); push_stack(main,x); if(x<temp) { push_stack(ass,temp); push_stack(ass,x); } else { push_stack(ass,temp); push_stack(ass,temp); } return 0; } int pop(stack *main,stack *ass,int *x) { ASSERT((main!=NULL)&&(ass!=NULL)); int temp; pop_stack(ass,&temp); pop_stack(main,x); return 0; } int min(stack *ass,int *x) { ASSERT((main!=NULL)&&(ass!=NULL)); pop_stack(ass,x); push_stack(ass,(*x)); return 0; }**



