# Lintcode——518. 超级丑数 - westbrook1998的博客 - CSDN博客





2018年04月26日 15:37:40[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：54








思路： 

1. 最小的丑数是1 

2. 对于任意丑数x,2x,3x,5x也是丑数 

3. 对于超级丑数来说，对于给定的数组[n1,n2…nn]，对于任何丑数x，n1x,n2x…nnx也是丑数 

4. 需要用到的数据结构是集合（Set）和优先队列（PriorityQueue）
代码： 

（c++版）

```cpp
class Solution {
public:
    /**
     * @param n: a positive integer
     * @param primes: the given prime list
     * @return: the nth super ugly number
     */
    int nthSuperUglyNumber(int n, vector<int> &primes) {
        // write your code here
        priority_queue<long long,vector<long long>,greater<long long> > pq;
        set<long long> s;
        pq.push(1);
        s.insert(1);
        for(int i=1;;i++){
            long long x=pq.top();
            pq.pop();
            if(i==n){
                return x;
                break;
            }
            for(int j=0;j<primes.size();j++){
                long long x2=x*primes[j];
                if(!s.count(x2)){
                    s.insert(x2);
                    pq.push(x2);
                }
            }
        }
    }
};
```

（java版）

```java
public class Solution {
    /**
     * @param n: a positive integer
     * @param primes: the given prime list
     * @return: the nth super ugly number
     */
    Comparator<Long> cmp=new Comparator<Long>() {
        @Override
        public int compare(Long o1, Long o2) {
            if(o1>o2){
                return 1;
            }
            else if(o1<o2){
                return -1;
            }
            else{
                return 0;
            }
        }
    };
    public int nthSuperUglyNumber(int n, int[] primes) {
        // write your code here
        Queue<Long> pq=new PriorityQueue<>(cmp);
        Set<Long> s=new HashSet<>();
        pq.add((long)1);
        s.add((long)1);
        for(int i=1;;i++){
            long x=pq.poll();
            if(i==n){
                return (int)x;
            }
            for(int j=0;j<primes.length;j++){
                long x2=x*primes[j];
                if(!s.contains(x2)){
                    s.add(x2);
                    pq.add(x2);
                }
            }
        }
    }
}
```




