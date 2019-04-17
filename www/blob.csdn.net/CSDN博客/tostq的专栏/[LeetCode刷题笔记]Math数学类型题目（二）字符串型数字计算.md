# [LeetCode刷题笔记]Math数学类型题目（二）字符串型数字计算 - tostq的专栏 - CSDN博客





2016年07月22日 22:00:51[tostq](https://me.csdn.net/tostq)阅读数：1407









**原创文章**


转载请注册来源[http://blog.csdn.net/tostq](http://blog.csdn.net/tostq)

这类题一般都选择将数字（一般很长）转换成了字符串、数组或链表的形式
比如"12345", 1->2->3->4之后
然后以这类形式，来进行相关的数学运算。多数情况下，这类数字都比较大，在转换时非常容易出数据位溢出的情况，另外可能会有空间和时间的限制，比如重写字符串数字加法，但不允许先将字符串数字转换成整型计算后，再重新转换回来。

**一、字符串型的数字运算**
**（1）以链表形式存在数字位2. Add Two Numbers**
数字以相反的顺序存储在链表中，也就是说低位在前，高位在后，比如
输入：(2 -> 4 -> 3) + (5 -> 6 -> 4)
输出：7 -> 0 -> 8
这个问题是比较简单，主要难点在于对于进位的处理（增加结点）。


```cpp
class Solution {
public:
    ListNode* addTwoNumbers(ListNode* l1, ListNode* l2) {
        ListNode dummy(0),*ptr=&dummy;
        int carry=0;
        while(l1||l2){
            int sum=carry;
            if(l1){
                sum=sum+l1->val;
                l1=l1->next;
            }
            if(l2){
                sum=sum+l2->val;
                l2=l2->next;                
            }
            ptr->next=new ListNode(sum%10);
            carry=sum/10;
            ptr=ptr->next;
        }
        if(carry)ptr->next=new ListNode(carry);
        return dummy.next;
    }
};
```
 如果要加大难度的话，可以让数字的存储是高位在前，低位在后。


（2）以字符串形式存在数字位
67. Add Binary
同上题一样，这也是一个加法问题，不过这是一个以字符串形式的存储数字位数的题


```cpp
char* addBinary(char* a, char* b) {
    int lena=strlen(a),lenb=strlen(b);
    int lenc=lena>lenb?lena:lenb;
    lenc++;
    char* c=(char*)malloc(sizeof(char)*(lenc+1));
    c[lenc]='\0';
    
    int carry=0;
    while(lena||lenb){
        if(lena)carry+=a[--lena]-'0';
        if(lenb)carry+=b[--lenb]-'0';
        c[--lenc]=(carry&1)+'0';
        carry=carry>>1;
    }
    c[0]=carry+'0';
    return c+(carry^1);
}
```
 （3）以数组形式的存储数字位数

66. Plus One
也是加法，数字位是以数组形式或vector形式存储的。

```cpp
class Solution {
public:
    vector<int> plusOne(vector<int>& digits) {
        reverse(digits.begin(),digits.end());
        int i;
        for(i=0;i<digits.size();i++){
            if(digits[i]+1<10){
                digits[i]=digits[i]+1;
                break;
            }
            digits[i]=0;
        }
        if(i==digits.size())digits.push_back(1);
        reverse(digits.begin(),digits.end());
        return digits;
    }
};
```
 可以看到对于高位在前，低位在后的存储形式，如果是C++的话，我们可以调用reverse来将数组反转，或者直接通过数组或字符串长度来倒转查取。
（4）两字符串数组相乘
43. Multiply Strings
这道题的难点
1、不能将字符串转换成整型。
2、如何处理乘法进位和加法进位

```cpp
class Solution {
public:
    string multiply(string num1, string num2) {
        if(num1[0]=='0'||num2[0]=='0')return "0";
        int l1=num1.size();
        int l2=num2.size();
        string res(l1+l2,'0');
        
        for(int i=l1-1;i>=0;i--){
            int c=0;
            int r=i+l2;            
            for(int j=l2-1;j>=0;j--,r--){
                int m=(num1[i]-'0')*(num2[j]-'0')+(int)(res[r]-'0')+c;
                int st=m%10;
                c=m/10;
                //printf("|m=%d,c=%d,st=%d|",m,c,st);
                res[r]=(char)(st+'0');
                if(j==0&&c!=0)res[(r-1)]=res[(r-1)]+c;
            }
        }
        while(res[0]=='0')res.erase(0,1);
        return res;
    }
};
```
（5）将字符串直接转换成数字

8. String to Integer (atoi)
这道题虽然标识为简单，而实际的正确提交率只有12%，比许多困难的题目都要低
主要原因：
1、String转换成数字可能出现溢出的情况
2、对于错误表示的数字的处理

```cpp
class Solution {
public:
    int myAtoi(string str) {
        while(str[0]==' ')str.erase(0,1);
        while(str[str.size()-1]==' ')str.erase(str.size()-1,1);
        int sign=1;
        if(str[0]=='-'){sign=-1;str.erase(0,1);}
        else if(str[0]=='+'){sign=1;str.erase(0,1);}
        int s=str.size();
        if(s==0)return 0;
        int res=0;
        int i=0;
        while(str[i]>='0'&&str[i]<='9'){
            if (res >  INT_MAX / 10 || (res == INT_MAX / 10 && str[i] - '0' > 7)){
                if (sign == 1) return INT_MAX;
                else return INT_MIN;
            } // overflow
            
            res=res*10+(int)(str[i]-'0');
            i++;
        }
        return sign*res;
    }
};
```

**二、数字转换成字符串**
166. Fraction to Recurring Decimal
这个题是一个计算除法结果的题目，除法的结果一般包括了小数部分，甚至无限循环，所以这里我们需要用字符串来表示输出结果。比如
输入：numerator = 1, denominator = 2, 输出："0.5".
输入：numerator = 2, denominator = 1, 输出："2".
输入：numerator = 2, denominator = 3, 输出："0.(6)".
具体的解法如下：

```cpp
#define ABS(x) (x>0?x:-x)
class Solution {
public:
    string inttostring(int64_t num){
        if(num==0)return "0";
        string s;
        while(num>0){
            s=(char)(num%10+'0')+s;
            num=num/10;
        }
        return s;
    }
    string fractionToDecimal(int64_t numerator, int64_t denominator) {
        string res;
        string decs;
        if((numerator>0&&denominator<0)||(numerator<0&&denominator>0))res="-";
        numerator=ABS(numerator);
        denominator=ABS(denominator);
        
        int64_t integarpart=numerator/denominator;
        int64_t restartnum=numerator%denominator;
        res=res+inttostring(integarpart);
        if(restartnum==0)return res;
        
        unordered_map<int, int> rem;
        int64_t rempart=restartnum;
        int i=0;
        rem.insert(make_pair(restartnum,i));
        while(1){
            int64_t newnum=rempart*10;
            int64_t decnum=newnum/denominator;
            decs=decs+inttostring(decnum);
            rempart=newnum%denominator;
            if(rempart==0)return res+"."+decs;
            if(rem.find(rempart)!=rem.end())return res+"."+decs.insert(rem[rempart],"(")+")";
            i++;
            rem.insert(make_pair(rempart,i));
        }
    }
};
```
 这里定义输入与输出为int64_t跟long类型是一个意思，就是为了防止在绝对值后，数据溢出
这个算法难道在于如何判断存在循环和循环位置（循环开始位置不一定在小数点的后一位，比如1/6）。其他包括整数部分和不循环小数都是比较好解决的。
循环的判断：如果存在循环，那么当前除完后剩下的余数，将会在未来再次出现，而该余数首次出现的位置应该是循环出现的位置。1/6=0.1(6)
所以我们可以通过一个Hash表来存储余数和其位置，如下定义
unordered_map<int, int> rem;
rem.find(rempart)!=rem.end()来确定某值是否在表内
rem.insert(make_pair(rempart,i)); 插入新值

**三、将数字表示其它符号**
这类题非常多，主要的难点要求做题人能充分了解新符号表示的特点，
不过一旦熟悉新符号表示的特点，这类题目就不难了。
**（1）数字与罗马数字的转换**
罗马数字主要有{'I','V','X','L','C','D','M'}，分别对应于{1,5,10,50,100,500,1000}
其将十分成了二等份，如果某数是在单位上增加了1，2，3（比如8在5的基础上增加了3，比如60(LX)在50(L)的基础上增加了10(X)），则往右添加低字，如XII=10+2，VII=5+7，XXX=30
如果是4，则往左添加低字，如IX=10-1=9
可以看出，如果较低字（比如I就比V低）在高字的前面，就说明该字要被减，否则一直都是加。
具体的解法如下：
**13. Roman to Integer**

```cpp
class Solution {
    const char romanNum[7]={'I','V','X','L','C','D','M'};
    const int num[7]={1,5,10,50,100,500,1000};
public:
    int romanToInt(string s) {
        if(s.size()==0)return 0;
        int res=0;
        int flag=0; //wei
        int pre=0;
        for(int i=s.size()-1;i>=0;i--){
            int j=0;
            for(j=0;s[i]!=romanNum[j];j++);
            int cur=num[j];
            if(cur>=pre){
                res=res+cur;
            }else{
                res=res-cur;
            }
            pre=cur;
        }
        return res;
    }
};
```
 这里还有一个小技巧，就是如何查询，通过for语句比switch比较要方便很多。
**12. Integer to Roman**
将数字转换成罗马数字就有些难度了，主要是因为罗马数字并不是十进位，而是五进位
而十进位的处理方式与五进位的处理方式是不同。
所以这里直接对其进行了分别考虑。

```cpp
class Solution {
    const char romanNum[7]={'I','V','X','L','C','D','M'};
    const int nums[7]={1,5,10,50,100,500,1000};
public:
    string intToRoman(int num) {
        
        string s;
        int j=6;
        while(j>=0&&num!=0){
            for(;j>=0&&num/nums[j]==0;j--); // find the max
            if(j%2==0){
                int g=num/nums[j];
                if(g==4)s=s+romanNum[j]+romanNum[j+1];
                else
                    for(int i=0;i<g;i++)s=s+romanNum[j];
                num=num%nums[j];}
            else{
                int g=(num-nums[j])/nums[j-1];
                if(g==4)s=s+romanNum[j-1]+romanNum[j+1];
                else{
                    s=s+romanNum[j];
                    for(int i=0;i<g;i++)s=s+romanNum[j-1];
                }
                num=num%nums[j-1];
            }
        }
        return s;
    }
};
```
**（2）数字与英文字母间的转换**
数字与英文之间的转换并不困难，主要的难点：
1、进位的转换，由10进制变成了26进制
2、对于0的处理，比如下面这两道数字都是从1开始，而不是从0开始的。
题目的形式如下：

    A -> 1

    B -> 2

    C -> 3

    ...

    Z -> 26

    AA -> 27

    AB -> 28
**171. Excel Sheet Column Number**

```cpp
int titleToNumber(char* s) {
    int res=0;
    char *t;
    t=s;
    while(*t!='\0'){
        res=res*26+(int)(*t)-(int)('A')+1;
        t++;
    }
    return res;
}
```

**168. Excel Sheet Column Title**

```cpp
class Solution {
public:
    string convertToTitle(int n) {
        string s;
        while(n>0){
            s=(char)((n-1)%26+'A')+s;
            n=(n-1)/26;
        }
        return s;
    }
};
```

**（3）数字与英文表示间的转换**
**273. Integer to English Words**
要求：
123 -> "One Hundred Twenty Three"
12345 -> "Twelve Thousand Three Hundred Forty Five"
1234567 -> "One Million Two Hundred Thirty Four Thousand Five Hundred Sixty Seven"
我们可以看出这是以一千为进位的，所以只要把数字划成一个个的三位数，转换就不困难的，不过对于个人来说，这道题还是花了不少的时间，主要问题就是对于空格的处理。因为字符串输入结果是要完美对应的，只要前后空格不能相互对应，就会输出错误。而我的绝大多数的时间都是花在了折腾这些空格上=_=||。
下面是具体的解法。

```cpp
class Solution {
private:
    const string numbers[20]={"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine","Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};
    const string weibers[4]={"Hundred","Thousand","Million","Billion"};
    const string tybers[10]={"","", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
public:
    string convertHundred(int num){ // num<1000
        string res;
        int h=num/100;
        res=(h>0?(numbers[h]+" "+"Hundred"):"");
        int t=num%100;
        if(t==0) return res;
        if(t<20) return res==""?numbers[t]:res+" "+numbers[t];
        int s=t%10;
        t=t/10;
        res=(res==""?tybers[t]:res+" "+tybers[t]);
        return s==0?res:(res==""?numbers[s]:res+" "+numbers[s]);
    }
    
    string numberToWords(int num) 
    {
        if(num==0)return "Zero";
        string res = convertHundred(num%1000);
        num=num/1000;
        if(num%1000)res=convertHundred(num%1000)+" Thousand"+((res == "") ? "" : " ")+res;
        num=num/1000;
        if(num%1000)res=convertHundred(num%1000)+" Million"+((res == "") ? "" : " ")+res;
        num=num/1000;
        if(num%1000)res=convertHundred(num%1000)+" Billion"+((res == "") ? "" : " ")+res;
        return res;
    }
};
```























