# v.size() return size_t not int 返回无符号整型数 - Grandyang - 博客园







# [v.size() return size_t not int 返回无符号整型数](https://www.cnblogs.com/grandyang/p/4486638.html)







In the C++ STL, the vector size() function return size_t, which is unsigned int, not int. So imagine this, define an empty vector<int> v, and v.size() should be 0, and you have a for loop, and has an end condition is v.size() - 1, which is 4294967295 actually, not -1. This might cause problem when i = 0, which can enter the for loop, but actually was supposed not entering it.



```
vector<int> v;
cout << v.size() << endl;      // 0
cout << v.size() - 1 << endl;  // 4294967295 
cout << v.size() - 2 << endl;  // 4294967294

cout << v.size() << endl;           // 0
cout << int(v.size() - 1) << endl;  // -1 
cout << int(v.size() - 2) << endl;  // -2
```














