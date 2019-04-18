# C++ 优先级表 - ljx0305的专栏 - CSDN博客
2010年02月04日 16:02:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：416标签：[c++																[access																[function																[object																[list](https://so.csdn.net/so/search/s.do?q=list&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=access&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)
个人分类：[C++](https://blog.csdn.net/ljx0305/article/category/380566)
The operators at the top of this list are evaluated first. Operators within a group have the same precedence. All operators have left-to-right associativity unless otherwise noted.
|Operator|Description|Example|
|----|----|----|
|**Group 1** (no associativity)| | |
|::|Scope resolution operator|Class::age = 2;|
|**Group 2**| | |
|()|Function call|isdigit('1')|
|[]|Array access|array[4] = 2;|
|->|Member access from a pointer|ptr->age = 34;|
|.|Member access from an object|obj.age = 34;|
|++|Post-increment|for( int i = 0; i < 10; i++ ) cout << i;|
|--|Post-decrement|for( int i = 10; i > 0; i-- ) cout << i;|
|**Group 3** (right-to-left associativity)| | |
|!|Logical negation|if( !done ) …|
|~|Bitwise complement|flags = ~flags;|
|++|Pre-increment|for( i = 0; i < 10; ++i ) cout << i;|
|--|Pre-decrement|for( i = 10; i > 0; --i ) cout << i;|
|-|Unary minus|int i = -1;|
|+|Unary plus|int i = +1;|
|*|Dereference|int data = *intPtr;|
|&|Address of|int *intPtr = &data;|
|(type)|Cast to a given type|int i = (int) floatNum;|
|sizeof|Return size of an object|int size = sizeof(floatNum);|
|**Group 4**| | |
|->*|Member pointer selector|ptr->*var = 24;|
|.*|Member object selector|obj.*var = 24;|
|**Group 5**| | |
|*|Multiplication|int i = 2 * 4;|
|/|Division|float f = 10.0 / 3.0;|
|%|Modulus|int rem = 4 % 3;|
|**Group 6**| | |
|+|Addition|int i = 2 + 3;|
|-|Subtraction|int i = 5 - 1;|
|**Group 7**| | |
|<<|Bitwise shift left|int flags = 33 << 1;|
|>>|Bitwise shift right|int flags = 33 >> 1;|
|**Group 8**| | |
|<|Comparison less-than|if( i < 42 ) …|
|<=|Comparison less-than-or-equal-to|if( i <= 42 ) ...|
|>|Comparison greater-than|if( i > 42 ) …|
|>=|Comparison geater-than-or-equal-to|if( i >= 42 ) ...|
|**Group 9**| | |
|==|Comparison equal-to|if( i == 42 ) ...|
|!=|Comparison not-equal-to|if( i != 42 ) …|
|**Group 10**| | |
|&|Bitwise AND|flags = flags & 42;|
|**Group 11**| | |
|^|Bitwise exclusive OR (XOR)|flags = flags ^ 42;|
|**Group 12**| | |
|||Bitwise inclusive (normal) OR|flags = flags | 42;|
|**Group 13**| | |
|&&|Logical AND|if( conditionA && conditionB ) …|
|**Group 14**| | |
||||Logical OR|if( conditionA || conditionB ) ...|
|**Group 15** (right-to-left associativity)| | |
|? :|Ternary conditional (if-then-else)|int i = (a > b) ? a : b;|
|**Group 16** (right-to-left associativity)| | |
|=|Assignment operator|int a = b;|
|+=|Increment and assign|a += 3;|
|-=|Decrement and assign|b -= 4;|
|*=|Multiply and assign|a *= 5;|
|/=|Divide and assign|a /= 2;|
|%=|Modulo and assign|a %= 3;|
|&=|Bitwise AND and assign|flags &= new_flags;|
|^=|Bitwise exclusive or (XOR) and assign|flags ^= new_flags;|
||=|Bitwise normal OR and assign|flags |= new_flags;|
|<<=|Bitwise shift left and assign|flags <<= 2;|
|>>=|Bitwise shift right and assign|flags >>= 2;|
|**Group 17**| | |
|,|Sequential evaluation operator|for( i = 0, j = 0; i < 10; i++, j++ ) …|
引用:http://blog.csdn.net/sheds/archive/2008/12/13/3510812.aspx
