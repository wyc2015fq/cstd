# C++ sort vector<vector<int> > or vector<MyClass> 容器的排序 - Grandyang - 博客园







# [C++ sort vector<vector<int> > or vector<MyClass> 容器的排序](https://www.cnblogs.com/grandyang/p/4843528.html)







C++的STL中提供了很强大的排序函数sort，可以对任意数组，结构体及类进行排序，下面我们先来看最简单的数组排序。默认的升序排列，我们也可以在后面加上less或greater来告诉编译器我们想要的排序顺序。



```
vector<int> v = {2, 0, 1, 5, 9, 2, 7};

// Ascending order
sort(v.begin(), v.end());
sort(v.begin(), v.end(), less<int>());

// Descending order
sort(v.rbegin(), v.rend());
sort(v.begin(), v.end(), greater<int>());
```



如果是一个二维数组，也可以是用sort，我们可以选择根据某一列来进行排序，如果我们不重写cmp函数，那么默认的是根据第一列来排序，当然我们可以通过重写来根据其他列来排序：



```
/* Input matrix
m = [
        1 4 2 
        0 8 3
        3 5 1
    ]
*/

// Ascending order by first column
sort(m.begin(), m.end());
/*
m = [
        0 8 3
        1 4 2 
        3 5 1
    ]
*/

// Descending order by first column
sort(m.rbegin(), m.rend());
/*
m = [
        3 5 1
        1 4 2 
        0 8 3
    ]
*/


// Ascending order by second column
sort(m.begin(), m.end(), [](const vector<int> &a, const vector<int> &b) { return a[1] < b[1]; } );

bool cmp(const vector<int> &a, const vector<int> &b) {
    return a[0] > b[0];
}
sort(m.begin(), m.end(), cmp);
/*
m = [
        1 4 2 
        3 5 1
        0 8 3
    ]
*/


// Descending order by second column
sort(m.begin(), m.end(), [](const vector<int> &a, const vector<int> &b) { return a[1] > b[1]; } );

bool cmp(const vector<int> &a, const vector<int> &b) {
    return a[0] < b[0];
}
sort(m.begin(), m.end(), cmp);
/*
m = [
        0 8 3
        3 5 1
        1 4 2 
    ]
*/
```



下面来看如何给class排序，我们将自定义的类class装入到容器vector中，然后根据class中的一个参数为依据进行排序，这里分两种情况，一个是vector中装指针的class，一种的装正常的class，它们再重写cmp比较函数上有些区别的，参见代码如下：

```
class A {
public:
    int a1, a2;
    A(int m, int n): a1(m), a2(n) {}
};

class B {
public:
    int b1, b2;
    B(int m, int n): b1(m), b2(n) {}
};

bool cmp1(A const *a, A const *b) {
    return a->a1 < b->a1;
}

bool cmp2(B const &a, B const &b) {
    return a.b1 < b.b1;
}

void printArray(vector<A*> array) {
    for (int i = 0; i < array.size(); ++i) {
        cout << array[i]->a1 << " " << array[i]->a2 << endl;
    }
    cout << endl;
}

void printArray2(vector<B> array) {
    for (int i = 0; i < array.size(); ++i) {
        cout << array[i].b1 << " " << array[i].b2 << endl;
    }
    cout << endl;
}

int main() {

    vector<A*> array;
    array.push_back(new A(65, 100));
    array.push_back(new A(70, 150));
    array.push_back(new A(56, 90));
    array.push_back(new A(75, 190));
    array.push_back(new A(60, 95));
    array.push_back(new A(68, 110));
    
    printArray(array);
    sort(array.begin(), array.end(), cmp1);
    printArray(array);
    
    vector<B> array2;
    array2.push_back(B(65, 100));
    array2.push_back(B(70, 150));
    array2.push_back(B(56, 90));
    array2.push_back(B(75, 190));
    array2.push_back(B(60, 95));
    array2.push_back(B(68, 110));
    
    printArray2(array2);
    sort(array2.begin(), array2.end(), cmp2);
    printArray2(array2);
    
    return 0;
}
```














