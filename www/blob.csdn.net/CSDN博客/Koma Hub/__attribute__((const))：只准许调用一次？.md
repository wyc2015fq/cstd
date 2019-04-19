# __attribute__((const))：只准许调用一次？ - Koma Hub - CSDN博客
2019年02月26日 22:01:55[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：17
个人分类：[GNU](https://blog.csdn.net/Rong_Toa/article/category/8661294)
the compiler can choose to call the function just once and cache the return value.
This attribute marks the function as considering *only* its numeric parameters. This is mainly intended for the compiler to optimize away repeated calls to a function that the compiler knows will return the same value repeatedly. It applies mostly to math functions that have no static state or side effects, and whose return is solely determined by the inputs.
In this highly-contrived example, the compiler normally *must* call the **square()** function in every loop even though we know that it's going to return the same value each time:
```cpp
extern int square(int n) __attribute__((const));
...
	for (i = 0; i < 100; i++ )
	{
		total += square(5) + i;
	}
```
By adding **__attribute__((const))**, the compiler can choose to call the function just once and cache the return value.
In virtually every case, **const** can't be used on functions that take pointers, because the function is not considering just the function parameters but *also the data the parameters point to*, and it will almost certainly break the code very badly in ways that will be nearly impossible to track down.
Furthermore, the functions so tagged cannot have any side effects or static state, so things like **getchar()** or **time()** would behave very poorly under these circumstances.
