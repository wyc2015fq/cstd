# C语言算法（一）基础 - 长歌行 - CSDN博客





2013年01月03日 18:16:14[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：702








代码来自C语言编程百例




一、魔方阵

[矩阵阶数是 3

     8    1    6

     3    5    7

     4    9    2](http://write.blog.csdn.net/postedit)




```cpp
for(i=1; i<=n; i++)
 		for(j=1; j<=n; j++)
 			array[i][j] = 0;
 
 	/* 建立魔方阵 */
 	j = n/2 + 1;
 	array[1][j] = 1;
 	for(k=2; k<=n*n; k++)
 	{
 		i = i - 1;
 		j = j + 1;
 		if((i<1) && (j>n))
 		{
 			i = i + 2;
 			j = j - 1;
 		}
 		else
 		{
 			if(i < 1)
 				i = n;
 			if(j > n)
 				j = 1;
 		}
 		if(array[i][j] == 0)
 			array[i][j] = k;
 		else
 		{
 			i = i + 2;
 			j = j - 1;
 			array[i][j] = k;
 		}
 	}
```


i, j均是从1开始计数




链表



```cpp
#include <stdio.h>
 #include <stdlib.h> 
 struct chain
 {
 	int value;
 	struct chain *next;
 };
 struct chain *create()
 {
 	struct chain *head, *tail, *p;
 	int x;
 	head = tail = NULL;
 	printf("Input data.\n");
 	while (scanf("%d",&x) == 1)	/*如果输入的是一个整型的数据，那么向下执行*/
 	{	
 p = (struct chain *)malloc (sizeof (struct chain));
 /*首先为要新创建的表元p开辟一个内存空间*/
 		p->value = x;
 		p->next = NULL;
 		if(head == NULL)
 			head = tail = p;
 		else 
 /*tail为倒数第二个表元指针，tail->始终指向最后一个表元*/
 			tail = tail ->next;	
 			tail ->next = p;	
 	}
 	return head;
 }
 
 void main(){
 	struct chain *p,*q;
 	q = create();
 	while(q) {
 		printf("%d\n",q->value);
 		p = q->next;
 		free(q);
 		q = p;
 	}
 }
```



队列


```cpp
#include <stdio.h>
 #define Max 100
 
 void SetNull(int *front, int *rear)
 {
 	*front = 0;
 	*rear = 0;
 }
 
 int Empty(int *front, int *rear)
 {
 	if(*front == *rear)
 		return(1);
 	else
 		return(0);
 }
 
 int EnQueue(int q[],int x,int *front, int *rear)
 {
 	*rear = (*rear+1) % Max;
 	if(*front == *rear)
 
 	{
 		printf("队列发生上溢\n");
 		return(-1);
 	}
 	else
 	{
 		q[*rear] = x;
 		return(0);
 	}
 }
 
 int DelQueue(int q[], int *y, int *front, int *rear)
 {
 	*front = (*front +1)%Max;
 	if(*front == *rear)
 	{
 		printf("队列发生下溢\n");
 		return(-1);
 	}
 	else
 	{
 		*y = q[*front];
 		return(0);
 	}
 }
 
 void main()
 {
 	int q[Max];
 	int f = 0, r = 0;	/*f和r分别对应队列的头和尾在整个队列存储区域的位置*/
 	int i,x,m,n;
 	int a;
 	SetNull(&f,&r);			/*清空队列*/
 	printf("要输入队列的字符个数：\n");
 	scanf("%d",&m);
 	printf("输入队列的整型数据：\n");
 	for (i=0; i<m; i++)
 	{
 		i=i;
 		scanf("%d",&x);
 		a = EnQueue(q,x,&f,&r);
 		if(a == -1)
 			break;
 	}
 	printf("要提出队列的字符个数：");
 	scanf("%d",&n);
 	printf("输出从队列中提取的数据：\n");
 	for (i = 0; i<n; i++)
 	{
 		if(DelQueue(q,&x,&f,&r) == -1)
 			break;
 		printf("%d\n",x);
 	}
 	if(Empty(&f,&r) == 1)
 		printf("队列为空");
 	else
 		printf("队列中还有%d个数据",(m-n));
 }
```

栈


```cpp
#include <stdio.h>
 #include <stdlib.h>
 #define Max 100
 
 int *p;
 int *tos;
 int *bos;
 
 /*添加一个数据放到堆栈对顶端*/
 void push(int i)
 {
 	if(p > bos)
 	{
 		printf("堆栈以满\n");
 		return;
 	}
 	*p = i;
 	p++;
 }
 
 /*丛堆栈顶端取出一个数据*/
 int pop(void)
 {
 	p--;
 	if(p < tos)
 	{
 		printf("堆栈下溢\n");
 		return 0;
 	}
 	return *p;
 }
 
 void main(void)
 {
 	int a,b;
 	char s[80];
 	p = (int *)malloc(Max*sizeof(int));
 	if(!p)
 	{
 		printf("分配内存失败");
 		exit(1);
 	}
 	tos = p;
 	bos = p + Max -1;
 	printf("请输入第一个数据:\n");
 	scanf("%d",&a);
 	push(a);
 	printf("请输入第二个数据:\n");
 	scanf("%d",&b);
 	push(b);
 	printf("请输入操作符:\n");
 	scanf("%s",s);
 	switch (*s)
 	{
 	case '+':
 		a = pop();
 		b = pop();
 		printf("结果是a+b = %d\n",(a+b));
 		push(a+b);
 		break;
 	case '-':
 		a = pop();
 		b = pop();
 		printf("结果是a-b = %d\n",(a-b));
 		push(a-b);
 		break;
 	case '*':
 		a = pop();
 		b = pop();
 		printf("结果是a*b = %d\n",(a*b));
 		push(a*b);
 		break;	
 	case '/':
 		a = pop();
 		b = pop();
 		printf("结果是a/b = %d\n",(a/b));
 		push(a/b);
 		break;
 	default:
 		printf("请输入正确操作符\n");
 	}
 }
```


二叉树


```cpp
#include <stdio.h>
 #include <stdlib.h>
 
 struct tree
 {
 	char info;
 	struct tree *left;
 	struct tree *right;
 };
 
 struct tree *root;		/*树的第一个结点*/
 struct tree *construct(struct tree *root, struct tree *r, char info);
 void print(struct tree *r, int l);
 
 int main(void)
 {
 	char s[80];
 	root = NULL;
 	do
 	{
 		printf("请输入一个字符:");
 		gets(s);
 		root = construct(root,root,*s);
 	}while(*s);
 	print(root,0);
 	return 0;
 }
 
 struct tree *construct(
 	struct tree *root, 
 	struct tree *r, 
 	char info)
 {
 	if(!r)
 	{
 		r = (struct tree *)malloc(sizeof(struct tree));
 		if(!r)
 		{
 			printf("内存分配失败！");
 			exit(0);
 		}
 		r->left = NULL;
 		r->right = NULL;
 		r->info = info;
 		if(!root)
 			return r;
 		if(info < root->info)
 			root->left = r;
 		else 
 			root->right = r;
 		return r;
 	}
 	if(info < r->info)
 		construct(r,r->left,info);
 	else
 		construct(r,r->right,info);
 
 	return root;	
 }
```






