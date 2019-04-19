# C语言实现简单的单例模式 - Koma Hub - CSDN博客
2018年04月03日 20:00:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：376
单例模式就是在程序运行过程中只存在此“对象”的一个“实例”，C语言实现单例模式就是简单的运用static全局变量。
我们的学号在教务系统中无论在任何地点进行任何操作，只会影响到对应学号的数据库内容，比如你在两台电脑上先后修改了你的名字，如下：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
typedef struct ID{
    char *name;
    int score;    
} id;
static id * _id = NULL;
id *getInstance(){
    if(_id != NULL)
        return _id;
    else {
        _id = (id*)malloc(sizeof(id));
        assert(_id != NULL);
        return _id;
    }
}
int main(){
    id* i1 = getInstance() ;
    i1->name = "Rong";
    id* i2 = getInstance() ;
    i2->name = "Tao";
    if(i1 == i2)
        fprintf(stdout, " i1 == i2 \n");
    fprintf(stdout, "i1->name = %s\n",i1->name);
    fprintf(stdout, "i2->name = %s\n",i2->name);
    return 0 ;
}
```
上面的代码中显示了如何建立单例对象，其输出的结果：
```
$ ./a.out 
 i1 == i2 
i1->name = Tao
i2->name = Tao
```
但是，在上面的代码中存在一定的隐患，如果在两台电脑上同时进行修改用户名的操作，那么就可能会造成不同线程之间的等待从而导致死锁，所以需要进一步修改代码：
```cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <omp.h>
typedef struct ID{
    char *name;
    int score;    
} id;
static id * _id = NULL;
static omp_lock_t lock;
id *getInstance(){
    omp_set_lock(&lock);
    if(_id != NULL) {
        omp_unset_lock(&lock);
        return _id;
    } else {
        _id = (id*)malloc(sizeof(id));
        assert(_id != NULL);
        omp_unset_lock(&lock);
        return _id;
    }
}
int main(){
    omp_set_num_threads(20);
    id * i1, *i2;
    omp_init_lock(&lock);
    #pragma omp parallel
    {
        i1 = getInstance() ;
        i1->name = "Rong";
        i1->score = omp_get_thread_num();
    }
    #pragma omp parallel 
    {
        i2 = getInstance() ;
        i2->name = "Tao";
    }
    omp_destroy_lock(&lock);
    if(i1 == i2)
        fprintf(stdout, " i1 == i2 \n");
    fprintf(stdout, "i1->name = %s, i1->score = %d\n",i1->name, i1->score);
    fprintf(stdout, "i2->name = %s, i2->score = %d\n",i2->name, i2->score);
    return 0 ;
}
```
编译与运行：
```
$ gcc main2.c -fopenmp
$ ./a.out 
 i1 == i2 
i1->name = Tao, i1->score = 7
i2->name = Tao, i2->score = 7
```
