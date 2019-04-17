# 中缀表达式(infix)转换为后缀表达式(postfix) - miner_zhu的博客 - CSDN博客





2018年07月23日 18:54:47[miner_zhu](https://me.csdn.net/miner_zhu)阅读数：492








### 算法描述
- 
第一种情况，表达式不含括号。

假设存在一个函数`prcd(op1,op2)`，其中op1和op2是两个操作符（在中缀表达式中，op1在op2的左边），如果op1的优先级高于或等于op2，函数返回True，否则返回False。例如：

```python
prcd('\*','+') --> True

prcd('+','-') --> True

prcd('\*','/') --> True

prcd('-','*') --> False
```

考虑表达式 A+B*C。当扫描到A时，将A添加到postfix中，接着遇到‘+’操作符，此时，由于只有一个操作数，‘+’只能先存入栈stack中；当扫描到B时，将其追加至postfix；接着遇到*，栈不为空，那么调用pcdr(pop(stack),’*’),因为‘+’优先级低于‘*’，返回False，将‘*’压入栈中,push(stack,’*’),接着遇到C，将其追加至postfix。这时，表达式扫描完，栈仍不为空，依次pop出数据，追加至postfix。最终结果为‘BC*A+’

- 
第二种情况，表达式含有括号。

当扫描到‘（’时，将其压入堆栈，这可以通过使`prcd(op1,'(')`对于任何op1都返回False来实现，另外，还定义`prcd('(',op2)`对于任何op2都返回False，这样一来，‘（’后的所有运算符（除了‘）’）都被压入堆栈 

	当扫描到右括号‘）’时，必须将堆栈中运算符都取出来，直到遇到‘（’。这可以通过定义`prcd(op1,')')`对于所有非左括号的运算符op1返回True来实现。当栈顶元素为左括号时，必须将其取出并和右括号一起作废。


### Python实现

```python
def infix2postfix(exp):
        postfix = ''
        stack = []
        def opOrder(op1,op2):
            order_dic = {'*':4,'$':5,'/':4,'+':3,'-':3}
            if op1 == '(' or op2 == '(':
                return False
             elif op2 == ')':
                return True
            else:
                if order_dic[op1] < order_dic[op2]:
                    return False
                else:
                    return True

        for s in exp:
            if s.isalpha():
                postfix += s
            else:
                while len(stack) != 0 and opOrder(stack[-1],s):
                    op = stack.pop()
                    postfix += op
                if len(stack) == 0 or s != ')':
                    stack.append(s)
                else:
                    top_op = stack.pop()
        if len(stack):
            postfix += ''.join(stack[::-1])
        return postfix

    if __name__ == '__main__':
        for exp in ['A+B*C','(A+B)*C','((A-(B+C))*D)$(E+F)']:
            print infix2postfix(exp)
```



原文链接：[https://blog.csdn.net/sinat_16968575/article/details/44423673](https://blog.csdn.net/sinat_16968575/article/details/44423673)



