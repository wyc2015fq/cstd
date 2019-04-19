# __attribute__((unused))：可能不会用到，消除编译警告 - Koma Hub - CSDN博客
2019年02月25日 22:17:59[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：85
### **__attribute__ unused:可能不会用到，消除编译警告**
One of the easiest attributes to use, this marks a variable as *intentionally* being possibly unused. Not only does this quiet the compiler from issuing an unused-variable warning, it tells the human the same thing: this is intentional.
Of course, it's a good idea to actually remove variables that you're not using, this is not always possible. A common case of the unused int argc parameter to main() is one, as are variables sometimes excluded by conditional compilation.
```cpp
int main(int argc, char **argv)
{
   /* code that uses argv, but not argc */
}
```
```
$ gcc -W test.c
test.c:1: warning: unused parameter 'argc'
```
The __attribute__ is added just after the variable name, and though it can appear unwieldy, it's a style you can get used to:
```cpp
int main(int argc __attribute__((unused)), char **argv)
{ ...
```
Additional uses shown, each with a comment showing the compiler warning it might have generated. Here we assume the code is being compiled *without* the **DEBUG** macro being defined, which excludes the actual use of mypid.
```cpp
/* warning: 'someFunction' declared 'static' but never defined */
static int someFunction() __attribute__((unused));
int main(int argc __attribute__((unused)), char **argv)
{
/* warning: unused variable 'mypid' */
int	mypid __attribute__((unused)) = getpid();
#ifdef DEBUG
	printf("My PID = %d\n", mypid);
#endif
	return 0;
}
```
