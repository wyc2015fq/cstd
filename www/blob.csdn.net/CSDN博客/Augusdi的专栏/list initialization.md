
# list initialization - Augusdi的专栏 - CSDN博客


2016年10月10日 13:39:13[Augusdi](https://me.csdn.net/Augusdi)阅读数：781


﻿﻿
Initializes an object from braced-init-list
|Contents
|[|[hide](http://en.cppreference.com/w/cpp/language/list_initialization#)|]
|1|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#Syntax)|Syntax|1.1|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#direct-list-initialization)|direct-list-initialization
|1.2|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#copy-list-initialization)|copy-list-initialization
|2|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#Explanation)|Explanation
|3|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#Narrowing_conversions)|Narrowing conversions
|4|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#Notes)|Notes
|5|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#Example)|Example
|6|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#Defect_reports)|Defect reports
|7|[ ](http://en.cppreference.com/w/cpp/language/list_initialization#See_also)|See also
|
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=1)
### ]
### Syntax
#### [
#### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=2)
#### ]
#### direct-list-initialization
|
|T|object|{|arg1, arg2, ...|};|(1)|
|
|T|{|arg1, arg2, ...|};|(2)|
|
|new|T|{|arg1, arg2, ...|}|(3)|
|
|Class|{|T|member|{|arg1, arg2, ...|}; };|(4)|
|
|Class|::|Class|() :|member|{|arg1, arg2, ...|} {...|(5)|
|
#### [
#### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=3)
#### ]
#### copy-list-initialization
|
|T|object|= {|arg1, arg2, ...|};|(6)|
|
|function|( {|arg1, arg2, ...|} ) ;|(7)|
|
|return {|arg1, arg2, ...|} ;|(8)|
|
|object|[ {|arg1, arg2, ...|} ] ;|(9)|
|
|object|=|{|arg1, arg2, ...|} ;|(10)|
|
|U|( {|arg1, arg2, ...|} )|(11)|
|
|Class|{|T|member|=|{|arg1, arg2, ...|}; };|(12)|
|
List initialization is performed in the following situations:
direct-list-initialization (both explicit and non-explicit constructors are considered)
1)initialization of a named variable with a*braced-init-list*(that is, a possibly empty brace-enclosed list of expressions or nested braced-init-lists)
2)initialization of an unnamed temporary with a*braced-init-list*
3)initialization of an object with dynamic storage duration with a[new-expression](http://en.cppreference.com/w/cpp/language/new), where the initializer is a brace-init-list
4)in a non-static[data member initializer](http://en.cppreference.com/w/cpp/language/data_members#Member_initialization)that does not use the equals sign
5)in a[member initializer list](http://en.cppreference.com/w/cpp/language/initializer_list)of a constructor if*braced-init-list*is used
copy-list-initialization (only non-explicit constructors may be called)
6)initialization of a named variable with a*braced-init-list*after an equals sign
7)in a function call expression, with*braced-init-list*used as an argument and list-initialization initializes the function parameter
8)in areturnstatement with braced-init-list used as the return expression and list-initialization initializes the returned object
9)in a[subscript expression](http://en.cppreference.com/w/cpp/language/operator_member_access#Built-in_subscript_operator)with a user-defined`operator[]`, where list-initialization initializes the parameter of the overloaded operator
10)in an[assignment expression](http://en.cppreference.com/w/cpp/language/operator_assignment#Builtin_direct_assignment), where list-initialization initializes the parameter of the overloaded`operator=`
11)[functional cast expression](http://en.cppreference.com/w/cpp/language/explicit_cast)or other constructor invocations, where*braced-init-list*is used in place of a constructor argument. Copy-list-initialization initializes the constructor's parameter (note; the type U in this example is not the type that's being list-initialized; U's constructor's parameter is)
12)in a non-static[data member initializer](http://en.cppreference.com/w/cpp/language/data_members#Member_initialization)that uses the equals sign
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=4)
### ]
### Explanation
The effects of list initialization of an object of type`T`are:
|If|T|is an aggregate type and the initializer list has a single element of the same or derived type (possibly cv-qualified), the object is initialized from that element (by copy-initialization for copy-list-initialization, or by direct-initialization for direct-list-initialization).
|Otherwise, if|T|is a character array and the initializer list has a single element that is an appropriately-typed string literal, the array is|[initialized from the string literal as usual](http://en.cppreference.com/w/cpp/language/aggregate_initialization#Character_arrays)
|(since C++14)|
|If the braced-init-list is empty and|T|is a class type with a default constructor,|[value-initialization](http://en.cppreference.com/w/cpp/language/value_initialization)|is performed.
|Otherwise, if|T|is an|[aggregate type](http://en.cppreference.com/w/cpp/language/aggregate_initialization)|,|[aggregate initialization](http://en.cppreference.com/w/cpp/language/aggregate_initialization)|is performed.
|(until C++14)|
|If|T|is an|[aggregate type](http://en.cppreference.com/w/cpp/language/aggregate_initialization)|,|[aggregate initialization](http://en.cppreference.com/w/cpp/language/aggregate_initialization)|is performed.
|Otherwise, If the braced-init-list is empty and|T|is a class type with a default constructor,|[value-initialization](http://en.cppreference.com/w/cpp/language/value_initialization)|is performed.
|(since C++14)|
Otherwise, ifTis a specialization of[std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list), theTobject is direct-initialized or copy-initialized, depending on context, froma prvalue of the same time initialized from(until C++17)the braced-init-list.
Otherwise, the constructors ofTare considered, in two phases:
All constructors that take[std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)as the only argument, or as the first argument if the remaining arguments have default values, are examined, and matched by[overload resolution](http://en.cppreference.com/w/cpp/language/overload_resolution)against a single argument of type[std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)
If the previous stage does not produce a match, all constructors ofTparticipate in[overload resolution](http://en.cppreference.com/w/cpp/language/overload_resolution#Implicit_conversion_sequence_in_list-initialization)against the set of arguments that consists of the elements of the braced-init-list, with the restriction that only non-narrowing conversions are allowed. If this stage produces an explicit constructor as the best match for a copy-list-initialization, compilation fails (note, in simple copy-initialization, explicit constructors are not considered at all).
|Otherwise, if|T|is a|[enumeration type](http://en.cppreference.com/w/cpp/language/enum)|that is either scoped or unscoped with fixed underlying type, and if the braced-init-list has only one initializer, and if the conversion from the initializer to the underlying type is non-narrowing, and if the initialization is direct-list-initialization, then the enumeration is initialized with the result of converting the initializer to its underlying type.
|(since C++17)|
Otherwise (ifTis not a class type), if the braced-init-list has only one element and eitherTisn't a reference type or is a reference type that is compatible with the type of the element,Tis[direct-initialized](http://en.cppreference.com/w/cpp/language/direct_initialization)(in direct-list-initialization) or[copy-initialized](http://en.cppreference.com/w/cpp/language/copy_initialization)(in copy-list-initialization), except that narrowing conversions are not allowed.
Otherwise, ifTis a reference type that isn't compatible with the type of the element,a temporary of the referenced type is list-initialized, and the reference is bound to that temporary(until C++17)the reference is direct-initialized from a prvalue expression of the referenced type that list-initializes its result object(since C++17). (this fails if the reference is a non-const lvalue reference)
Otherwise, if the braced-init-list has no elements,Tis[value-initialized](http://en.cppreference.com/w/cpp/language/value_initialization).
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=5)
### ]
### Narrowing conversions
list-initialization limits the allowed[implicit conversions](http://en.cppreference.com/w/cpp/language/implicit_cast)by prohibiting the following:
conversion from a floating-point type to an integer type
conversion from alongdoubletodoubleor tofloatand conversion fromdoubletofloat, except where the source is a[constant expression](http://en.cppreference.com/w/cpp/language/constant_expression)and overflow does not occur
conversion from an integer type to a floating-point type, except where the source is a constant expression whose value can be stored exactly in the target type
conversion from integer or unscoped enumeration type to integer type that cannot represent all values of the original, except where source is a constant expression whose value can be stored exactly in the target type
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=6)
### ]
### Notes
A braced-init-list is not an expression and therefore has no type, e.g.decltype({1,2})is ill-formed. Having no type implies that template type deduction cannot deduce a type that matches a braced-init-list, so given the declarationtemplate<classT>voidf(T);the expressionf({1,2,3})is ill-formed. A special exception is made for[type deduction using the keyword ](http://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts)auto[ ](http://en.cppreference.com/w/cpp/language/template_argument_deduction#Other_contexts), which deduces any braced-init-list as[std::initializer_list](http://en.cppreference.com/w/cpp/utility/initializer_list)
Also because braced-init-list has no type,[special rules for overload resolution](http://en.cppreference.com/w/cpp/language/overload_resolution#Implicit_conversion_sequence_in_list-initialization)apply when it is used as an argument to an overloaded function call.
|Aggregates copy/move initialize directly from single-element braced-init-lists of the same type, but non-aggregates consider initializer_list constructors first:
|struct|X|{|X|(|)|=|default|;|X|(|const|X|&|)|=|default|;|}|;|struct|Q|{|Q|(|)|=|default|;|Q|(|Q|const|&|)|=|default|;|Q|(|std::|initializer_list|<|Q|>|)|{|}|}|;|int|main|(|)|{|X x|;|X x2|=|X|{|x|}|;|// copy-constructor (not aggregate initialization)|Q q|;|Q q2|=|Q|{|q|}|;|// initializer-list constructor (not copy constructor)|}
|(since C++14)|
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=7)
### ]
### Example


```python
#include <iostream>
#include <vector>
#include <map>
#include <string>
 
struct Foo {
    std::vector<int> mem = {1,2,3}; // list-initialization of a non-static member
    std::vector<int> mem2;
    Foo() : mem2{-1, -2, -3} {} // list-initialization of a member in constructor
};
 
std::pair<std::string, std::string> f(std::pair<std::string, std::string> p)
{
    return {p.second, p.first}; // list-initialization in return statement
}
 
int main()
{
    int n0{};     // value-initialization (to zero)
    int n1{1};    // direct-list-initialization
    std::string s1{'a', 'b', 'c', 'd'}; // initializer-list constructor call
    std::string s2{s1, 2, 2};           // regular constructor call
    std::string s3{0x61, 'a'}; // initializer-list ctor is preferred to (int, char)
 
    int n2 = {1}; // copy-list-initialization
    double d = double{1.2}; // list-initialization of a temporary, then copy-init
 
    std::map<int, std::string> m = { // nested list-initialization
           {1, "a"},
           {2, {'a', 'b', 'c'} },
           {3, s1}
    };
 
    std::cout << f({"hello", "world"}).first // list-initialization in function call
              << '\n';
 
    const int (&ar)[2] = {1,2}; // binds a lvalue reference to a temporary array
    int&& r1 = {1}; // binds a rvalue reference to a temporary int
//  int& r2 = {2}; // error: cannot bind rvalue to a non-const lvalue ref
 
//  int bad{1.0}; // error: narrowing conversion
    unsigned char uc1{10}; // okay
//  unsigned char uc2{-1}; // error: narrowing conversion
 
    Foo f;
 
    std::cout << n0 << ' ' << n1 << ' ' << n2 << '\n'
              << s1 << ' ' << s2 << ' ' << s3 << '\n';
    for(auto p: m)
        std::cout << p.first << ' ' << p.second << '\n';
    for(auto n: f.mem)
        std::cout << n << ' ';
    for(auto n: f.mem2)
        std::cout << n << ' ';
}
```
Compiler messages:
main.cpp: In function 'int main()':main.cpp:26:12: warning: unused variable 'd' [-Wunused-variable]double d = double{1.2}; // list-initialization of a temporary, then copy-init^main.cpp:37:17: warning: unused variable 'ar' [-Wunused-variable]const int (&ar)[2] = {1,2}; // binds a lvalue reference to a temporary array^~main.cpp:38:11: warning: unused variable 'r1' [-Wunused-variable]int&& r1 = {1}; // binds a rvalue reference to a temporary int^~main.cpp:42:19: warning: unused variable 'uc1' [-Wunused-variable]unsigned char uc1{10}; // okay^~~
Output:
world
0 1 1
abcd cd aa
1 a
2 abc
3 abcd
1 2 3 -1 -2 -3
world0 1 1abcd cd aa1 a2 abc3 abcd1 2 3 -1 -2 -3

### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=8)
### ]
### Defect reports
The following behavior-changing defect reports were applied retroactively to previously published C++ standards.
|DR|Applied to|Behavior as published|Correct behavior|
|[CWG 1467](http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1467)|C++14|same-type initialization of aggregates and char arrays was prohibited|same-type initialization allowed|
|[CWG 1467](http://open-std.org/JTC1/SC22/WG21/docs/cwg_defects.html#1467)|C++14|std::initializer_list constructors had priority over copy constructors for single-element lists|single-element lists initialize directly|
### [
### [edit](http://en.cppreference.com/mwiki/index.php?title=cpp/language/list_initialization&action=edit&section=9)
### ]
### See also
[default initialization](http://en.cppreference.com/w/cpp/language/default_initialization)
[direct initialization](http://en.cppreference.com/w/cpp/language/direct_initialization)
[aggregate initialization](http://en.cppreference.com/w/cpp/language/aggregate_initialization)
[copy initialization](http://en.cppreference.com/w/cpp/language/copy_initialization)




