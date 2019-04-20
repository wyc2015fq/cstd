# C语言高级应用——把握N级指针 - 风雪夜归人 - CSDN博客
2017年08月29日 09:36:43[cdjccio](https://me.csdn.net/qq_34624951)阅读数：266
# 1、多级指针快排
[摘要] 本节内容取材于实际应用中，有较高的学习价值，故将源程序整理于此，大家可以分析分析，详细的解析将在下面给出。
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int getArrayMem(char*** mp, char* myArray1[], int n, char* str, int* count);
int sortArray(char** p, int count);
int SortArrayAndGen3Mem(char*** myArray3, char* myArray1[], int n, char* str, int* num);
int printArray(char** p, int num);
int freeArray(char*** newFree, int num);
int main(void)
{
int ret = 0;
char **myArray3 = NULL;
int num3 = 0;
//第一种内存模型
char *myArray[] = {"bbbbb", "aaa", "ccccccc"};//指针数组
char *myp = "1111111ddd";//字符数组
//要求：将myArray和myp合并为一个字符串存到myArray3里，排序后甩出来
SortArrayAndGen3Mem(&myArray3, myArray, 3, myp, &num3);
printf("\n排序之后：\n");
ret = printArray(myArray3, num3);
if(0 != ret)
{
printf("func printArray() err:%d\n", ret);
return ret;
}
if(NULL != myArray3)
{
ret = freeArray(&myArray3, num3);
if(0 != ret)
{
printf("func freeArray() err:%d\n", ret);
return ret;
}
}
system("pause");
}
int SortArrayAndGen3Mem(char*** myArray3, char* myArray1[], int n, char* str, int* num)
{
int i = 0, j = 0, count = 0;
char** myp = NULL;
getArrayMem(&myp, myArray1, n, str, &count);
for(i = 0; i < n; i++)
{
strcpy(myp[i], myArray1[i]);
}
strcpy(myp[i], str);
printf("排序之前：\n");
printArray(myp, count);
sortArray(myp, count);
*num = count;
*myArray3 = myp;
}
int sortArray(char** p, int count)
{
char tmp[100];
int i = 0, j = 0;
for(i = 0; i < count - 1; i++)
{
for(j = i + 1; j < count - 1; j++)
{
if(strcmp(p[i], p[j]) > 0)
{ 
strcpy(tmp, p[i]);
strcpy(p[i], p[j]);
strcpy(p[j], tmp);
}
}
}
}
int getArrayMem(char*** mp, char* myArray1[], int n, char* str, int* count)
{
int tmpNum = 0, i = 0, tmpCount = 0, rv = 0;
char** p = NULL;
tmpNum = n + 1;
p = (char**)malloc(tmpNum*sizeof(char*));
if(NULL == p)
{
rv = -1;
printf("func getArrayMem() NULL == p err:%d\n", rv);
return rv;
}
for(i = 0; i < n; i++, tmpCount++)
{
p[i] = (char*)malloc((strlen(myArray1[i])*sizeof(char))+1);
if(NULL == p[i])
{
rv = -1;
printf("func getArrayMem() NULL == p[i] err:%d\n", rv);
return rv;
}
}
p[i] = (char*)malloc((strlen(str)*sizeof(char))+1);
if(NULL == p[i])
{
rv = -1;
printf("func getArrayMem() NULL == p[i] err:%d\n", rv);
return rv;
}
tmpCount++;
*count = tmpCount;
*mp = p;
}
int printArray(char** p, int num)
{
int i = 0, rv = 0;
if(NULL == p)
{
rv = -1;
printf("NULL == p err:%d\n", rv);
return rv;
}
for(i = 0; i < num; i++)
{
printf("%s\n", p[i]);
}
return rv;
}
int freeArray(char*** newFree, int num)
{
int rv = 0, i = 0;
char** tmpArray = NULL;
tmpArray = *newFree;
if(NULL == newFree)
{
rv = -1;
printf("ULL == newFree err:%d\n", rv);
return rv;
}
for(i = 0; i < num; i++)
{
if(NULL != tmpArray[i])
{
free(tmpArray[i]);
}
}
free(tmpArray);
*newFree = NULL;
//if(NULL != tmpArray)
//{
//free(tmpArray);
//tmpArray = NULL;
//}
return rv;
}
```
# 2、多级指针辅助知识
引言：我深知大家对于指针是既爱又恨，起初我又何尝不是呢？说爱他是因为大师们都说指针是C的精髓，没有学会指针就没有掌握C的要义；说恨他是因为太危险，一不小心就“爆机”了。容我冒昧的引用林语堂老先生《朱门》里的那句：“只用一样东西，不明白它的道理，实在不高明!”况且处于启蒙的我们，就像侯俊杰先生说的“勿在浮沙筑高台。”
先说明些东西：
#include <stdio.h>
void main()
{
     char str[]="zllzllzll";
     char *p=str;
     printf("str   = %p        p      = %p   \n",str,p);
     printf("str+1 = %p        &str +1= %p\n",str +1,&str +1);
     printf("p+1   = %p\n",p+1);
}
这段程序运行的结果如下(不同的机器稍有偏差，但理论没问题)：
![](http://www.cjjjs.com/source/attached/image/20150721/20150721192546_2207.jpg)
注意：这里的str+与&str+1是不同的；
解析如下：str与&str的值是一样的，都是指向数组str第一个元素的地址，但是str+1与&str+1却是不一样的。因为str指向的是char类型的指针但&str指向的是char[10]类型的指针，所以（str+1）增加的是1而（&str+1）增加的是10*1=10.
现在正式开始我们的万里长征
#include <stdio.h>
void main()
{
    int *p1=NULL,*p2=NULL,*p3=NULL,*p4=NULL;
    int temp=8;
    int *p=NULL;
  p4  = (int *)(&temp);//---->其实完全可以写作p4=&temp
  p3  = (int *)(&p4);  //但编译器很笨，它要检查类型是否
  p2 = (int *)(&p3);  //匹配，所以（int *是我们无耐写给编译器看的
  p1 = (int *)(&p2);  //p1到p其实就是依次取得下一级的变量地址给上一级
  p  = (int *)(&p1);
    printf("%d \n",*(int *)(*(int *)(*(int *)(*(int *)(*p)))));
 }
对于printf这个函数调用解释一下。之所以上面有很多（int *），是为了告诉编译器从此地址开始取多少位，就像前面说的那样，编译器很笨，它不知道你在想什么，事实上你也不明它是怎么想的。
怕否？不碍事，慢慢看。现在张结构图，助于理解。
![](http://www.cjjjs.com/source/attached/image/20150721/20150721192951_5605.jpg)
本来这张图已经很直白的表达了我的意思，然我欲当回老者，多啰嗦几句。为此先说明下规则：下面的’----->’以及‘=’这两个的意思是一样的，而非语言本身之所表达的意思。
/*从上往下看，在配以上图，可谓之妙哉！*/
*p                      -----> &p1
*(*p)                   ----->*(&p1) = &p2
*(*(*p))                 ----->*(&p2) = &p3
*(*(*(*p)))               ----->*(&p3)=&p4
*(*(*(*(*p))))            ----->*(&p4)=&temp
*(*(*(*(*(*p)))))          ----->*(&temp)=temp---->8
不要烦躁，上面的式子是可以化简的 如：
printf("%d \n",*(**** (int *****)p));
来说说思路吧。int那边的5个‘*’，这么解释，源程序中的标号所示的5个语句，每次对p进行取内容时，都需要告诉编译器取4个字节长，共取5次，所以要告诉编译器5次。
到现在还没出现所谓的n级指针，总是有总见林不见鸟的感觉，可别觉得我在忽悠你呀，这不，下面就来了。。。。。
int *p1=NULL,*p2=NULL,*p3=NULL,*p4=NULL;
       int temp=8;
int *****p=NULL;
p4 = (int *)(&temp);//---->其实完全可以写作p4=&temp
p3 = (int *)(&p4);//但编译器很笨，它要检查类型是否
p2 = (int *)(&p3);//匹配，所以（int *是我们无耐写给编译器看的
p1 = (int *)(&p2);//p1到p其实就是依次取得下一级的变量地址给上一级
p  = (int *****)(&p1);//这也实属无耐，VC6.0的类型检查太严了，直接                          //给的error，而在GCC下给个警告而已，然谁又会怪
                      //罪于“苛求”呢？那是好事
printf("%d \n",*(*(*(*(*p)))));//当然这些括号’(’是可以去掉的，我
                                  //却不愿已简洁换取‘涩’，至于你如何如          
  //何，那是你的事了，呵呵。
![](http://www.cjjjs.com/source/attached/image/20150721/20150721192932_9716.jpg)
释惑：你会问：“这边为什么不用加（int *）？”我会释：“因为定义时 int *****p”就已经告诉编译器了，所以我跟喜欢用这种写法玩多级指针，一来省事，二来还略显牛逼(原谅我的粗俗)。
  这边还用前面的图示，我有点懒，呵呵
至此多级指针的要义差不多就完了，也没写出什么来。但这几张图足以解释多级指针了，或许你还想知道指针与数组的关系，没事，那是下次文章该做的事。
问你们个问题：你们C语言老师肯定说过：“指针只能进行减法而不能进行加法运算”这么说对吗？我不知道，或许知道，但你自己着实该自己尝试着分析，以擎高(深)升
其实，这篇文章是为下次我讲链表做的铺垫，或许当时也是这么玩的，但毕竟不方便，才有了后来的………
提示你一下：如果把图中的小方框中间在画一条竖线你想到了什么？
这讲主要是为“C语言高级应用——多级指针快排”那篇做铺垫，下面解决问题时会用到本讲的内容
