# C++ Split string into vector<string> by space - Grandyang - 博客园







# [C++ Split string into vector<string> by space](https://www.cnblogs.com/grandyang/p/4858559.html)







在C++中，我们有时候需要拆分字符串，比如字符串string str = "dog cat cat dog"想以空格区分拆成四个单词，Java中实在太方便了，直接String[] v = str.split(" ");就搞定了，而c++中没有这么方便的实现，但也有很多的方法能实现这个功能，下面列出五种常用的实现的方法，请根据需要选择，个人觉得前三种使用起来比较方便，参见代码如下：``



```
#include <vector>
    #include <iostream>
    #include <string>
    #include <sstream>
    
    string str = "dog cat cat dog";
    istringstream in(str);
    vector<string> v;
    
    // Method 1
    string t;
    while (in >> t) {
        v.push_back(t);
    }


    // Method 2
    // #include <iterator>
    copy(istream_iterator<string>(in), istream_iterator<string>(), back_inserter(v));
        

    // Method 3
    string t;
    while (getline(in, t, ' ')) {
        v.push_back(t);
    }


    // Method 4
    string str2 = str;
    while (str2.find(" ") != string::npos) {
        int found = str2.find(" ");
        v.push_back(str2.substr(0, found));
        str2 = str2.substr(found + 1);
    }    
    v.push_back(str2);
    

    // Method 5
    // #include <stdio.h>
    // #include <stdlib.h>  
    // #include <string.h>
    char *dup = strdup(str.c_str());
    char *token = strtok(dup, " ");
    while (token != NULL) {
        v.push_back(string(token));
        token = strtok(NULL, " ");
    }
    free(dup);
```














