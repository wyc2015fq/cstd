# [CareerCup] 13.1 Print Last K Lines 打印最后K行 - Grandyang - 博客园







# [[CareerCup] 13.1 Print Last K Lines 打印最后K行](https://www.cnblogs.com/grandyang/p/4920079.html)







13.1 Write a method to print the last K lines of an input file using C++.



这道题让我们用C++来打印一个输入文本的最后K行，最直接的方法是先读入所有的数据，统计文本的总行数，然后再遍历一遍打印出最后K行。这个方法需要读两遍文件，我们想使用一种更简便的方法，只需要读取一遍文本就可以打印出最后K行，这里我们使用一个循环数组Circular Array，原理是我们维护一个大小为K的字符串数组，当数组存满后，新进来的数据从开头开始存，覆盖原有的数据。这样当我们读完整个文本，最后K行就保存在了这个大小为K的字符串数组，只不过顺序不相同，我们始终要有个变量来记录最后一个进来的数据的位置，这样我们就可以按原文本的顺序打印出最后K行了，参见代码如下：



```
class Solution {
public:
    void printLast5Lines(char *fileName) {
        const int K = 5;
        ifstream file(fileName);
        string L[K];
        int size = 0;
        while (file.good()) {
            getline(file, L[size % K]);
            ++size;
        }
        int start = size > K ? (size % K) : 0;
        int cnt = min(K, size);
        for (int i = 0; i < cnt; ++i) {
            cout << L[(start + i) % K] << endl;
        }
    }
};
```














