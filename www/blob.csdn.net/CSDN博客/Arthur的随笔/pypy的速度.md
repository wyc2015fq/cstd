# pypy的速度 - Arthur的随笔 - CSDN博客
2012年12月07日 11:01:05[largetalk](https://me.csdn.net/largetalk)阅读数：2610
无聊作PE [http://projecteuler.net/problem=10](http://projecteuler.net/problem=10) ， 求2百万以下数的所有数的总和
python 随便写了一个
```python
big = 2000000
import time
plst = [2, 3, 5, 7, 11, 13]
t1 = time.time()
for i in xrange(17, big, 2):
    for p in plst:
        if i%p == 0:
            break
        if p*p > i:
            plst.append(i)
            break
print time.time() - t1
#print len(plst)
print sum(plst)
```
然后
```python
$ python peuler10.py 
12.4184100628
142913828922
```
太慢了，不能忍受，用C试了下：
```cpp
#include <stdio.h>
#include <time.h>
int main(void)
{
    clock_t begin, end;
    unsigned long big = 2000000;
    unsigned long pl[150000] = {2,3,5,7,11,13};
    unsigned long i = 0;
    unsigned int max = 6;
    begin = clock();
    for(i=17;i<big;i+=2){
        int j = 0;
        while(1){
            if(i%pl[j] == 0)
                break;
            if(pl[j] * pl[j] > i){
                pl[max] = i;
                max += 1;
                break;
            }
            j += 1;
        }
    }
    big = 0;
    for(i=0; i < max; i++){
        big += pl[i];
    }
    end = clock();
    double cost = (double)(end - begin)/CLOCKS_PER_SEC;
    printf("%ld\n", big);
    printf("%lf seconds\n", cost);
}
```
```
$ ./a
142913828922
0.750000 seconds
```
这差距实在是太大了， 又祭起pypy试试：
```
$ pypy peuler10.py -0jit
0.665678977966
142913828922
```
竟然比C还快。
pypy是python的一线希望，就是那些扩展不知道怎么办。好像很多脚本语言都有如是困境， PHP有最近facebook搞出的HipHop解释器，js因为V8现在势头正猛，不过都还有一定差距。还是更看好go，go也比较合我口味。
