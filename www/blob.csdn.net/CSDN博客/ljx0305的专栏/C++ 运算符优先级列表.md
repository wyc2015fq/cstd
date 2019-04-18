# C++ 运算符优先级列表  - ljx0305的专栏 - CSDN博客
2008年05月06日 13:10:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：820
### C++ Operator Precedence
The operators at the top of this list are evaluated first.
|Precedence|Operator|Description|Example|Associativity|
|----|----|----|----|----|
|1|::|Scoping operator|Class::age = 2;|none|
|2|()[]->.++--|Grouping operatorArray accessMember access from a pointerMember access from an objectPost-incrementPost-decrement|(a + b) / 4;array[4] = 2;ptr->age = 34;obj.age = 34;for( i = 0; i < 10; i++ ) ...for( i = 10; i > 0; i-- ) ...|left to right|
|3|!~++---+*&(type)[sizeof](http://www.cppreference.com/keywords/sizeof.html)|Logical negationBitwise complementPre-incrementPre-decrementUnary minusUnary plusDereferenceAddress ofCast to a given typeReturn size in bytes|if( !done ) ...flags = ~flags;for( i = 0; i < 10; ++i ) ...for( i = 10; i > 0; --i ) ...int i = -1;int i = +1;data = *ptr;address = &obj;int i = (int) floatNum;int size = sizeof(floatNum);|right to left|
|4|->*.*|Member pointer selectorMember object selector|ptr->*var = 24;obj.*var = 24;|left to right|
|5|*/%|MultiplicationDivisionModulus|int i = 2 * 4;float f = 10 / 3;int rem = 4 % 3;|left to right|
|6|+-|AdditionSubtraction|int i = 2 + 3;int i = 5 - 1;|left to right|
|7|<<>>|Bitwise shift leftBitwise shift right|int flags = 33 << 1;int flags = 33 >> 1;|left to right|
|8|<<=>>=|Comparison less-thanComparison less-than-or-equal-toComparison greater-thanComparison geater-than-or-equal-to|if( i < 42 ) ...if( i <= 42 ) ...if( i > 42 ) ...if( i >= 42 ) ...|left to right|
|9|==!=|Comparison equal-toComparison not-equal-to|if( i == 42 ) ...if( i != 42 ) ...|left to right|
|10|&|Bitwise AND|flags = flags & 42;|left to right|
|11|^|Bitwise exclusive OR|flags = flags ^ 42;|left to right|
|12|||Bitwise inclusive (normal) OR|flags = flags | 42;|left to right|
|13|&&|Logical AND|if( conditionA && conditionB ) ...|left to right|
|14||||Logical OR|if( conditionA || conditionB ) ...|left to right|
|15|? :|Ternary conditional (if-then-else)|int i = (a > b) ? a : b;|right to left|
|16|=+=-=*=/=%=&=^=|=<<=>>=|Assignment operatorIncrement and assignDecrement and assignMultiply and assignDivide and assignModulo and assignBitwise AND and assignBitwise exclusive OR and assignBitwise inclusive (normal) OR and assignBitwise shift left and assignBitwise shift right and assign|int a = b;a += 3;b -= 4;a *= 5;a /= 2;a %= 3;flags &= new_flags;flags ^= new_flags;flags |= new_flags;flags <<= 2;flags >>= 2;|right to left|
|17|,|Sequential evaluation operator|for( i = 0, j = 0; i < 10; i++, j++ ) ...|left to right|
One important aspect of C++ that is related to operator precedence is the **order of evaluation** and the **order of side effects** in expressions. In some circumstances, the order in which things happen is not defined. For example, consider the following code:
float x =1;
  x = x /++x;
The value of x is not guaranteed to be consistent across different compilers, because it is not clear whether the computer should evaluate the left or the right side of the division first. Depending on which side is evaluated first, x could take a different value.
Furthermore, while `++x` evaluates to `x+1`, the side effect of actually storing that new value in x could happen at different times, resulting in different values for x.
The bottom line is that expressions like the one above are horribly ambiguous and should be avoided at all costs. When in doubt, break a single ambiguous expression into multiple expressions to ensure that the order of evaluation is correct.
### 引用地址:[http://www.cppreference.com/operator_precedence.html](http://www.cppreference.com/operator_precedence.html)
