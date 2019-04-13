
# ECMAScript Assignment Operators - cocoonyang的专栏 - CSDN博客


2017年08月07日 11:27:09[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：170个人分类：[JavaScript																](https://blog.csdn.net/cocoonyang/article/category/6457126)



# Assignment Operators
## Simple Assignment (=)
[1]The production AssignmentExpression : LeftHandSideExpression = AssignmentExpression is evaluated as follows:
get value of left hand side expression;
get value of right hand side expression;
if no exception, assigning result to left hand side expression
## Compound Assignment  (operator=)
[1]Compound Assignment[1] (operator=) is one of the following:

$$
*=     /=    \%=    +=  	-=	  <<=	  >>=	  >>>=  	&=	  ^=  	|=
$$
The production of “LeftHandSideExpression CompoundAssignmentOperator AssignmentExpression” is evaluated as follows:
get value of left hand side expression;
get value of right hand side expression;
applying operator to left hand side value and right hand side value;
if no exception, assigning result to left hand side expression
> [1] ECMA-262 5.1
> [http://www.ecma-international.org/ecma-262/5.1/](http://www.ecma-international.org/ecma-262/5.1/)

---


