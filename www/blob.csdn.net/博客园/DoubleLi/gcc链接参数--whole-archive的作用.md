# gcc链接参数--whole-archive的作用 - DoubleLi - 博客园






// a.h

extern void foo();



// a.cpp

#include <stdio.h>



void foo()

{

    printf("foo\n");

}



// x.cpp

#include "a.h"



int main()

{

        foo();

        return 0;

}



// Makefile

all: x



x: x.cpp liba.so

g++ -g -o $@ $^



liba.so: liba.a

g++ -g -fPIC -shared -o $@ $^

#g++ -g -fPIC -shared -o $@ -Wl,--whole-archive $^ -Wl,-no-whole-archive



liba.a: a.o

ar cru $@ $^



a.o: a.cpp

g++ -g -c $^



clean:

rm -f x a.o liba.a liba.so



$ make

g++ -g -c a.cpp

ar cru liba.a a.o

g++ -g -fPIC -shared -o liba.so liba.a

#g++ -g -fPIC -shared -o liba.so -Wl,--whole-archive liba.a -Wl,-no-whole-archive

g++ -g -o x x.cpp liba.so

/tmp/cc6UYIAF.o: In function `main':

/data/jayyi/ld/x.cpp:5: undefined reference to `foo()'

collect2: ld returned 1 exit status

make: *** [x] Error 1



默认情况下，对于未使用到的符号（函数是一种符号），链接器不会将它们链接进共享库和可执行程序。

这个时候，可以启用链接参数“--whole-archive”来告诉链接器，将后面库中所有符号都链接进来，参数“-no-whole-archive”则是重置，以避免后面库的所有符号被链接进来。



// Makefile

all: x



x: x.cpp liba.so

g++ -g -o $@ $^



liba.so: liba.a

g++ -g -fPIC -shared -o $@ -Wl,--whole-archive $^ -Wl,-no-whole-archive



liba.a: a.o

ar cru $@ $^



a.o: a.cpp

g++ -g -c $^



clean:

rm -f x a.o liba.a liba.so









