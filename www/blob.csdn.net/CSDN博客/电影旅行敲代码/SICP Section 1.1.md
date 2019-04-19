# SICP Section 1.1 - 电影旅行敲代码 - CSDN博客
2018年12月24日 21:41:47[电影旅行敲代码](https://me.csdn.net/dashuniuniu)阅读数：57
SICP 1.1节的标题是《The Elements of Programming》，分为如下几个小节，重点在于加粗的内容。
- Expressions
- Naming and the Environment
- **Evaluating Combinations**
- **Compound Procedures**
- **The Substitution Model for Procedure Application**
- Conditional Expressions and Predicates
- Example: Square Roots by Newton’s Method
- Procedures as Black-Box Abstractions
*注：第二小节Naming and the Enviroment在编程语言设计中是单独的一部分内容，未来在些PLP的读书笔记时可能会涉及到，这一小节在SICP中着墨不多*
# 1.1.3 Evaluating Combinations
> 
Expressions such as these, formed by delimiting a list of expressions within parentheses in order to denote procedure application, are called combinations.
这一小节介绍了如何evaluate表达式组合，关于编程中的组合是什么意思见[编程语言中的「组合性」是什么意思？](1)。表达式组合最直观的理解就是如何一系列表达式的组合，如下scheme代码所示，就是用来求解$2^2 + 3^2$的值。
```
(+ (* 2 2) (* 3 3))
```
上面这个表达式就是由三个表达式组合而成的，如下图所示，由两个乘法`(* 2 2)`和`(* 3 3)`以及加法`(+ (* 2 2) (* 3 3))`组成。
![表达式组合](https://img-blog.csdnimg.cn/20181220221953630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
这个和数学中的函数非常相似，例如我们定义两个数学函数，$f(x, y) = x + y$和$g(x) = x * x$，那么上面的计算就类似于求$f(g(2), g(3))$的结果。推荐一个比较方便的画数学函数图形的网站，我们将这两个函数放入其中，计算结果会是13。需要注意的是**数学函数其实更接近于procedure的概念**，而表达式组合，很简单，其实就是数学运算中的$(2 * 2) + (3 * 3)$。
![数学函数](https://img-blog.csdnimg.cn/2018122022363273.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
*注：到后面我们会发现函数式编程最终来源于邱奇的lambda演算，而命令式编程来源于图灵的图灵机，两者的计算能力等价的*
对于表达式组合的求值，SICP给出了如下两个步骤的计算方法：
- Evaluate the subexpressions of the combination.
- Apply the procedure that is the value of the leftmost subexpression (the operator) to the arguments that are the values of the other subexpressions (the operands).
首先求解子表达式，然后将子表达式的值代入运算符，求得结果。对于$f(g(2), g(3))$来说，就是先计算$g(2)$和$g(3)$的值，也就是4和9，最终再求解$f(4,9)$。这个过程是递归进行的，也就是子表达式可能还有子表达式。例如求解$g(g(2))$，就是先求得$g(2)$
这一小节还提供了define的概念，define的概念可以理解为数学中**定义函数**的操作，例如下面的代码就类似于数学中的$f(x)=9$。
```
(define x 9)
```
# 1.1.4 Compound Procedures
当我们先按照**数学函数**的方式理解函数编程以后，一些东西就比较好理解了。
```
(define (square x) 
    (* x x))
(define (sum-of-square x y) 
    (+ (square x) (square y)))
(define (f a)
    (sum-of-squares (+ a 1) (* a 2)))
```
对于上面给出的示例代码，我们就可以使用数学中的函数进行理解，如下图所示，$s(x)$对应于`square`，$f(x, y)$对应于`sum-of-square`，而$g(x)$对应于`f`，而`f(5)`也能轻易的算出来为136。
![数学中的函数](https://img-blog.csdnimg.cn/20181220231956945.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rhc2h1bml1bml1,size_16,color_FFFFFF,t_70)
# 1.1.5 The substitution Model for Procedure Application
这一小节主要介绍了我们如何求解procedure，首先介绍了`substitution model`，替换模型的核心如下，将过程体重的形参由实参值代替，也就是替换名字的由来。
> 
To apply a compound procedure to arguments, evaluate the body of the procedure with **each formal parameter replaced by the corresponding argument**.
经过这一步骤以后，就可以转化为求表达式组合的问题，这一过程是递归的，表达式组合中可能也由procedure的求解，此时仍然需要做替换。
```
(define (square x) 
    (* x x))
(define (sum-of-square x y) 
    (+ (square x) (square y)))
(define (f a)
    (sum-of-squares (+ a 1) (* a 2)))
(f 5)
```
对于上述代码，要求$f(5)$，首先进行替换，得到如下代码：
```
(sum-of-squares (+ 5 1) (* 5 2))
```
此时就是表达式组合问题，先求解子表达式值，如下代码：
```
(sum-of-squares 6 10)
```
此时又需要进行替换，将`sum-of-squares`中的形参由实参值代替，如下所示：
```
(+ (square 6) (square 10))
```
这种方式就称之为Applicative Order的求解，也就是
> 
the interpreter **first evaluates the operator and operands** and then **applies the resulting procedure to the resulting arguments**.
我们使用前面提到的函数$g(x)$来说明Applicative Order和Normal Order的区别。
|Applicative Order|Normal Order|
|----|----|
|$f(5 + 1, 5 * 2)$|$f(5 + 1, 5 * 2)$|
|$f(6, 10)$|$f(5 + 1, 5 * 2)$|
|$(s(6) + s(10))$|$(s(5 + 1) + s (5 * 2))$|
|$((6 * 6) + (10 * 10))$|$((5+1) * (5 + 1) + ( 5 * 2) * (5 * 2))$|
|$(36 + 100)$|$(6 * 6 + 10 * 10)$|
|$136$|$136$|
从上面我们可以看到Applicative Order比Normal Order要高效，这也是scheme采用Applicative Order的原因。
# 习题
## Exercise 1.5
由于scheme采用的是Applicative Order，所以下面的代码永远也不会结束，基于substitution model一直在使用`(p)`替换`(p)`。但是采用Normal Order就不会出现这样的问题。
```
(define (p) (p)) 
(define (test x y)
    (if (= x 0) 0 y))
(test 0 (p))
```
## Exercise 1.6
类似于Exercise 1.5，下面的代码永远也不会终止。
```
(define (new-if predicate then-clause else-clause) 
    (cond (predicate then-clause)
        (else else-clause)))
(define (sqrt-iter guess x) 
    (new-if (good-enough? guess x)
        guess
        (sqrt-iter (improve guess x) x)))
```
因为`new-if`是procedure，按照Applicative Order，首先会evaluate argument，例如我们尝试将`(sqrt-iter guess x)`展开得到下面的代码，`good-enough`能够evaluate直接得到结果，`guess`也可以直接得到结果，而`sqrt-iter`会继续evaluate `new-if`，所以scheme解释器在处理这一段代码时出现了没有终止条件的递归，最终会爆栈。
```
(new-if (good-enough? guess) guess (sqrt-iter(improve guess x) x))
                 ^             ^                     ^
            predicate      then-clause          else-clause
```
## Exercise 1.7
这个问题比较tricky，`good-enough?`在处理很小的数值时不够精确可以理解，因为小数的平方会更小，所以此时将判断条件附加到平方结果上效果不是很好，例如我们使用0.001来作为判断标准，例如$0.001^2 = 0.00001$，$0.002^2 = 0.00004$。而对于特别大的数据，请访问 [https://sukritkalra94.wordpress.com/2014/05/26/sicp-exercise-1-7-problems-with-good-enough/](https://sukritkalra94.wordpress.com/2014/05/26/sicp-exercise-1-7-problems-with-good-enough/) 。
> 
This is because floats have a fixed precision. After a certain point, the guesses are unable to guess the square root of a number within 0.001 of the answer. At that point, they alternate between the two guesses, both of whose squares are more than 0.001 away from the argument.
## Exercise 1.8
```
(define (good-enough? old-guess new-guess)
  (< (abs (- old-guess new-guess)) 0.001))
(define (improve guess x)
  (/ 
    (+ 
      (/ 
        x 
          (* guess guess))
      
      (* 2 guess))
    3)
  )
(define (cube-iter guess x)
  (if (good-enough? guess (improve guess x))
    guess
    (cube-iter (improve guess x) x)
))
(define (cube-root x)
  (cube-iter 1.0 x))
(cube-root 10000000000)
```
