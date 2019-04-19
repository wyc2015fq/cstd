# 剑指offer——替换空格 - 刘炫320的博客 - CSDN博客
2017年07月04日 20:47:52[刘炫320](https://me.csdn.net/qq_35082030)阅读数：272
所属专栏：[算法编程习题解答（java版）](https://blog.csdn.net/column/details/16358.html)
# 1. 问题描述
> 
请实现一个函数，将一个字符串中的空格替换成“%20”。例如，当字符串为We Are Happy.则经过替换之后的字符串为We%20Are%20Happy。
# 2. 解答思路
对于这一题，考察的是字符串的知识，不过我们还是想由浅入深的来进行思考。
## 2.1 level1
看到字符串替换，那么首先想到的应该是replace，这毋庸置疑，于是乎，就有了下面的代码：
```
public static String replaceSpace(StringBuffer str) {
        return str.toString().replaceAll(" ","%20");
     }
```
但如果面试的时候你这么做，不一定能够通过，虽然它确实可以实现功能。
## 2.2 level2
于是乎，就出现了下面这种不直接replaceall的方法，思路是原字符串先变成一个字符队列，然后每个字符依次出队，如果出队的字符不是空格，则将其插入到新字符串中，否则，插入%20。于是，就有下面这种代码：
```
public static String replaceSpace(StringBuffer str) {
        String sti = str.toString();
        char[] strChar = sti.toCharArray();
        StringBuffer stb = new StringBuffer();
        for(int i=0;i<strChar.length;i++){
            if(strChar[i]==' '){
                stb.append("%20");
            }else{
                stb.append(strChar[i]);
            }
        }
        return stb.toString();
    }
```
但是这样有2个问题，第一，如果使用了String类型的tochararray方法，那么replaceall同样能够使用；第二，它新开辟了一个空间，这有可能是不被允许的。
## 2.3 level3
为了克服以上两点，于是就有了第三级别的方法，它的思路是只是用StringBuffer自带的方法来进行操作，这样可以最大程度上减少依赖项。每次确定字符串中的空格的位置，然后删掉这个空格，再在这个位置上插入%20即可。代码如下：
```
public static String replaceSpace(StringBuffer str){
         int offset=str.indexOf(" ");
         while(offset!=-1){
             str.deleteCharAt(offset);
             str.insert(offset, "%20");
             offset=str.indexOf(" ");
         }
         return str.toString();
     }
```
## 2.4 level4
当然，这可能是由于给出的测试接口和语言的问题，如果是C++里的char*的话，那么就要复杂很多了。思路我已经注释在代码中了，代码如下：
```
class Solution {
public:
    void replaceSpace(char *str,int length) {
        int numOfBlank = 0;
        int old_len = 0;
        int new_len = 0;
        char *new_p = NULL;
        char *old_p = NULL;
        char *tmp_str = NULL;
         //判断是不是空的
        if(str == NULL || length <= 0)
            return ;
        tmp_str = str;
        //如过没有到头的话
        while(*str != '\0')
        {
            //统计字符串长度
            old_len++;
            if(*str == ' ')
                //统计空格数量
                numOfBlank++;
            str++;
        }
        //得出新的字符串长度
        new_len = old_len + 2*numOfBlank;
        //如果新的字符串长度大于限制长度，直接返回空
        if(new_len > length)
            return ;
        //两个位置指针，都从结尾开始。
        old_p = tmp_str+old_len;
        new_p = tmp_str+new_len;
        while(old_p >= tmp_str && new_p > old_p)
        {
            if(*old_p == ' ')
            {
                *(new_p--) = '0';
                *(new_p--) = '2';
                *(new_p--) = '%';
            }else{
                *(new_p--) = *old_p;
            }
            old_p--;
        }
    }
};
```
# 3. 小结
这一次的题目不难，就是一个字符串中子串替换的问题，这里只是替换了一个空格，也有可能是替换一个子串。
但是，我们要考虑到什么层面，是一个重要问题，如果仅仅为了代码量，可能使用库函数是最佳的方案，但是有可能有一些限制条件，比如不能重新创建新空间，又比如只能使用已给出的类进行操作等，至于要不要深入到最底层，那要看面试的目的了，一般C/C++都是需要最底层对于指针的操作的，需要非常扎实的基本功才能够完成。
