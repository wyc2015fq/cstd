
# C printf() 样例 - cocoonyang的专栏 - CSDN博客


2017年02月08日 11:16:47[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：243个人分类：[C																](https://blog.csdn.net/cocoonyang/article/category/6705806)





```python
#include <stdio.h>
#include <stdlib.h>
int main( int argc, char* argv[] )
{
	printf("printf() Benchmark \n");
	printf("\n");
	printf("int printf(const char *format,[argument]);\n");
	printf("Print formatted data to stdout \n");
	printf("\n");
	printf("Format prototype: \n");
	printf("%%[flags][width][.precision][length]specifier \n");
	printf("\n");

	printf("Specifiers:\n");
	printf("d or i \t Signed decimal integer  \n");
	printf("u \t Unsigned decimal integer\n");
	printf("f \t Decimal floating point, lowercase	\n");
	printf("F \t Decimal floating point, uppercase \n");
	printf("e \t Scientific notation (mantissa/exponent), lowercase	\n");
	printf("E \t Scientific notation (mantissa/exponent) \n");
	printf("s \t String of characters \n");
	printf("\n");

	printf("The format specifier can also contain sub-specifiers: \n");
	printf("    flags, width, .precision and modifiers (in that order)\n");
	printf("\n");
	printf("width  \n");
	printf("(number)\t Minimum number of characters to be printed. \n");
	printf("        \t If the value to be printed is shorter than this number, \n");
	printf("        \t the result is padded with blank spaces. \n");
	printf("        \t The value is not truncated even if the result is larger. \n");
	printf("\n");

	printf(".precision  \n");
	printf(".number\t For integer specifiers (d, i, o, u, x, X):   \n");
	printf("       \t precision specifies the minimum number of digits to be written.\n");
	printf("       \t If the value to be written is shorter than this number, \n");
	printf("       \t the result is padded with leading zeros. \n");
	printf("       \t The value is not truncated even if the result is longer. \n");
	printf("       \t A precision of 0 means that no character is written for the value 0. \n");
	printf("       \t For a, A, e, E, f and F specifiers: \n");
	printf("       \t this is the number of digits to be printed after the decimal point \n");
	printf("       \t (by default, this is 6). \n");

	printf("\n");
	printf("Characters: %c %c \n", 'a', 65);
	printf("Decimals: %d %ld\n", 1977, 650000L);
	printf("Some different radices: %d %x %o %#x %#o \n", 100, 100, 100, 100,100);
	printf("floats: %4.2f %+.0e %E \n", 3.1416, 3.1416, 3.1416);
	printf("Width trick: %*d \n", 5, 10);
	printf("\n");
	double value = 123456.123456;
	printf("Testing value = %f \n", value);
	printf("printf(\"%%f\"):%f \n", value);
	printf("printf(\"%%6.3f\"):%6.3f \n", value);
	printf("printf(\"%%6.6f\"):%6.6f \n", value);
	printf("printf(\"%%6.8f\"):%6.8f \n", value);
	printf("printf(\"%%16.8f\"):%6.8f \n", value);
	printf("printf(\"%%e\"):%E\n", value);
	printf("\n");
	printf("Preceding with blanks:  \n");
	printf("printf(\"%%10d\"):%10d\n", 1977);
	printf("\n");
	printf("Preceding with zeros:  \n");
	printf("printf(\"%%010d\"):%010d \n", 1977);
	printf("\n");
	printf("%s \n", "String Benchmarks");
	printf("printf(\"%%s\"):%s\n", "Hello, world!");
	printf("printf(\"%%15s\"):%15s\n", "Hello, world!");
	printf("printf(\"%%.10s\"):%.10s\n", "Hello, world!");
	printf("printf(\"%%-10s\"):%-10s\n", "Hello, world!");
	printf("printf(\"%%-15s\"):%-15s\n", "Hello, world!");
	printf("printf(\"%%.15s\"):%.15s\n", "Hello, world!");
	printf("printf(\"%%15.10s\"):%15.10s\n", "Hello, world!");
	printf("printf(\"%%-15.10s\"):%-15.10s\n", "Hello, world!");
	return 0;
}
```

输出结果：

```python
printf() Benchmark
int printf(const char *format,[argument]);
Print formatted data to stdout
Format prototype:
%[flags][width][.precision][length]specifier
Specifiers:
d or i   Signed decimal integer
u        Unsigned decimal integer
f        Decimal floating point, lowercase
F        Decimal floating point, uppercase
e        Scientific notation (mantissa/exponent), lowercase
E        Scientific notation (mantissa/exponent)
s        String of characters
The format specifier can also contain sub-specifiers:
    flags, width, .precision and modifiers (in that order)
width
(number)         Minimum number of characters to be printed.
                 If the value to be printed is shorter than this number,
                 the result is padded with blank spaces.
                 The value is not truncated even if the result is larger.
.precision
.number  For integer specifiers (d, i, o, u, x, X):
         precision specifies the minimum number of digits to be written.
         If the value to be written is shorter than this number,
         the result is padded with leading zeros.
         The value is not truncated even if the result is longer.
         A precision of 0 means that no character is written for the value 0.
         For a, A, e, E, f and F specifiers:
         this is the number of digits to be printed after the decimal point
         (by default, this is 6).
Characters: a A
Decimals: 1977 650000
Some different radices: 100 64 144 0x64 0144
floats: 3.14 +3e+00 3.141600E+00
Width trick:    10
Testing value = 123456.123456
printf("%f"):123456.123456
printf("%6.3f"):123456.123
printf("%6.6f"):123456.123456
printf("%6.8f"):123456.12345600
printf("%16.8f"):123456.12345600
printf("%e"):1.234561E+05
Preceding with blanks:
printf("%10d"):      1977
Preceding with zeros:
printf("%010d"):0000001977
String Benchmarks
printf("%s"):Hello, world!
printf("%15s"):  Hello, world!
printf("%.10s"):Hello, wor
printf("%-10s"):Hello, world!
printf("%-15s"):Hello, world!
printf("%.15s"):Hello, world!
printf("%15.10s"):     Hello, wor
printf("%-15.10s"):Hello, wor
```



