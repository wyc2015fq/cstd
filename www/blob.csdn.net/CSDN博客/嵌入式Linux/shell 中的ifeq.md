
# shell 中的ifeq - 嵌入式Linux - CSDN博客

2015年09月15日 15:46:50[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：2729


```python
libs_for_gcc = -lgnu
    normal_libs =
    foo: $(objects)
    ifeq ($(CC),gcc)
            $(CC) -o foo $(objects) $(libs_for_gcc)
    else
            $(CC) -o foo $(objects) $(normal_libs)
    endif
```
可见，在上面示例的这个规则中，目标“foo”可以根据变量“$(CC)”值来选取不同的函数库来编译程序。
我们可以从上面的示例中看到三个关键字：ifeq、else和endif。ifeq的意思表示条件语句的开始，并指定一个条件表达式，表达式包含两个参数，以逗号分隔，表达式以圆括号括起。else表示条件表达式为假的情况。endif表示一个条件语句的结束，任何一个条件表达式都应该以endif结束。
当我们的变量$(CC)值是“gcc”时，目标foo的规则是：
foo: $(objects)
$(CC) -o foo $(objects) $(libs_for_gcc)
而当我们的变量$(CC)值不是“gcc”时（比如“cc”），目标foo的规则是：
foo: $(objects)
$(CC) -o foo $(objects) $(normal_libs)
当然，我们还可以把上面的那个例子写得更简洁一些：
```python
libs_for_gcc = -lgnu
    normal_libs =
    ifeq ($(CC),gcc)
      libs=$(libs_for_gcc)
    else
      libs=$(normal_libs)
    endif
    foo: $(objects)
            $(CC) -o foo $(objects) $(libs)
```


