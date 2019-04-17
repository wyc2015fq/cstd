# nyoj35——逆波兰表达式 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





逆波兰表达式又称作后缀表达式，在四则混合运算的程序设计中用到。

例如：

1+2写成后缀表达式就是12+

4+5*(3-2)的后缀表达式就是4532-*+

后缀表达式在四则运算中带来了意想不到的方便，在生成过程中自动保持了优先级；

生成逆波兰表达式的算法如下：

我们首先的用两个栈结构来存储运算符和操作数；

从做到右遍历我们输入的中缀表达式：

1、如果是操作数的话，那么就直接压入存放操作数的堆栈；

2、如果是"("左括号的话，那么就直接压入存放运算符的堆栈；

3、如果是")"右括号的话，那么就从运算符堆栈中弹数据，并将弹出的数据压入到操作数堆栈中，直到遇到"("为止，这里值得注意的是，"("是必须要从运算符堆栈中弹出的，但是不压入到操作数堆栈，后缀表达式中是不包含括号的；

4、如果是其他符号，就是其他的运算符+-*/这些，那么就：

　　a、如果运算符堆栈为空，则直接压入运算符堆栈；

　　b、如果不为空且此时运算符堆栈的栈顶元素为括号，包括左括号和右括号，那么也是直接压入运算符堆栈中；

　　c、如果此时遍历到的元素的优先级比此时运算符堆栈栈顶元素的优先级高，则直接压入运算符堆栈；

　　d、如果正在遍历的元素的优先级和运算符堆栈栈顶的元素的优先级相等或者更小，则需要将栈顶元素弹出并且放到操作数堆栈中，并且将正在遍历的元素压入到运算符堆栈，其实运算符堆栈中的元素顺序就是优先级的顺序；

5、直到遍历完表达式，此时还需要将运算符堆栈中的所有元素压入到操作数堆栈中，算法完成。



-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------



根据以上算法可以得到逆波兰表达式，我们得到逆波兰表达式之后，必须求得正确的计算结果，有了逆波兰表达式，计算起来就攘夷多了，算法如下：

用另一个堆栈来保存数据，从左到右遍历逆波兰表达式：

1、如果是数字，则直接压入堆栈中；

2、如果是运算符（+,-,*,/），则弹出堆栈中的两个数据，进行相应的计算，计算完成之后得到一个数据，然后又压入堆栈中；

3、直到遍历完成，此时堆栈中的数据就是最后计算的结果，简单吧

以上摘自：https://blog.csdn.net/uestclr/article/details/50630906

AC代码：

```
#include <bits/stdc++.h>
using namespace std;

int prio(char x)  //定义优先级
{
    switch (x){
        case '+': return 1;
        case '-': return 1;
        case '*': return 2;
        case '/': return 2;
        default : return 0; //默认会出现括号，而如果有括号都得压进去
    }
}




int main()
{
    int t;
    cin >> t;
    while(t--){
        stack<char>s;
        stack<double>d;
        string s1,s2;
        cin >> s1;
        int i=0;
        s2 = "";
        s.push('#');
        while(i < s1.length()-1){ // 等于号 不用扫描
            if('(' == s1[i]){
                s.push(s1[i++]);
            }
            else if(')' == s1[i]){
                while(s.top() != '('){
                    s2 += s.top();
                    s2 += ' '; //加空格是为了计算时的方便
                    s.pop();
                }
                s.pop();//把左括号弹出
                i++;
            }
            else if(s1[i] == '+'||s1[i] == '-'||s1[i] == '*'||s1[i] == '/'){
                while(prio(s.top()) >= prio(s1[i])){
                    s2 += s.top();
                    s2 += ' ';
                    s.pop();
                }
                s.push(s1[i]);
                i++;
            }
            else{//如果是数字类
                while(s1[i] >= '0'&&s1[i] <= '9'||s1[i] == '.'){
                    s2 += s1[i++];
                }
                s2 += ' ';
            }
        }
        while(s.top() != '#'){ //当表达式扫描完，栈中还有剩余符号
            s2 += s.top();
            s2 += ' ';
            s.pop();
        }

//        cout << s2 << endl;

        double result = 0;
        i = 0;
        while(i < s2.length()){
            switch (s2[i]){
                case '+':
                    result = d.top();
                    d.pop();
                    result += d.top();
                    d.pop();
                    i++;
                    break;
                case '-':
                    result = d.top();
                    d.pop();
                    result = d.top() - result;
                    d.pop();
                    i++;
                    break;
                case '*':
                    result = d.top();
                    d.pop();
                    result *= d.top();
                    d.pop();
                    i++;
                    break;
                case '/':
                    result = d.top();
                    d.pop();
                    result = d.top()/result;//注意因为是栈，所以除法的顺序发生变化
                    d.pop();
                    i++;
                    break;
                default:
                    result = 0;
                    double fac = 10.0;
                    while(s2[i] >= '0'&&s2[i] <= '9'){
                        result = result*10 + s2[i] - '0';
                        i++;
                    }
                    if(s2[i++] == '.'){
                        while(s2[i] >= '0'&&s2[i] <= '9'){
                            result += (s2[i] - '0')/fac;
                            fac *= 10;
                            i++;
                        }
                    }
            }
//            cout << result << endl;
            d.push(result);
            while(s2[i] == ' '){
                i++;
            }
        }
        printf("%.2lf\n",d.top());
    }
    return 0;
}
```

 ——











