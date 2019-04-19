# ANSI C typedef - Koma Hub - CSDN博客
2019年02月01日 21:50:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：36
个人分类：[ANSI C](https://blog.csdn.net/Rong_Toa/article/category/8653945)
**目录**
[Difference Between typedef int x[10] and #define x int[10]](#Difference%20Between%20typedef%20int%20x%5B10%5D%20and%20%23define%20x%20int%5B10%5D)
[What typedef struct foo { ... foo; } foo; Means](#What%20typedef%20struct%20foo%20%7B%20...%20foo%3B%20%7D%20foo%3B%20Means)
```cpp
void (*signal(int sig, void (*func)(int)) ) (int);
void (*signal(                          ) ) (int);
```
```cpp
typedef void (*ptr_to_func) (int);
/* this says that ptr_to_func is a pointer to a function
* that takes an int argument, and returns void
*/
```
```cpp
ptr_to_func signal(int, ptr_to_func);
/* this says that signal is a function that takes
* two arguments, an int and a ptr_to_func, and
* returns a ptr_to_func
*/
```
```cpp
typedef int *ptr, (*fun)(), arr[5];
/* ptr is the type "pointer to int"
* fun is the type "pointer to a function returning int"
* arr is the type "array of 5 ints"
*/
```
## Difference Between typedef int x[10] and #define x int[10]
```cpp
#define peach int
unsigned peach i; /* works fine */
typedef int banana;
unsigned banana i; /* Bzzzt! illegal */
```
```cpp
#define int_ptr int *
int_ptr chalk, cheese;
```
```cpp
typedef char * char_ptr;
char_ptr Bentley, Rolls_Royce;
```
## What typedef struct foo { ... foo; } foo; Means
There are multiple namespaces in C:
- * label names
- * tags (one namespace for all structs, enums and unions)
- * member names (each struct or union has its own namespace)
- * everyting else
```cpp
struct vnode {
long v_flag;
long v_usecount;
struct vnode *v_freef;
struct vnodeops *v_op;
};
```
```cpp
struct foo {int foo;} foo;
```
```cpp
typedef struct baz {int baz;} baz;
        struct baz variable_1;
               baz variable_2;
```
```cpp
typedef struct my_tag {int i;} my_type;
        struct my_tag variable_1;
              my_type variable_2;
```
```cpp
typedef struct fruit {int weight, price_per_lb } fruit; /*statement 1 */
        struct veg {int weight, price_per_lb } veg; /*statement 2 */
```
