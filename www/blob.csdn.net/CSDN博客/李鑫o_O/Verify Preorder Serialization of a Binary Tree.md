# Verify Preorder Serialization of a Binary Tree - 李鑫o_O - CSDN博客





2016年03月07日 11:26:44[hustlx](https://me.csdn.net/HUSTLX)阅读数：361








```cpp
bool isValidSerialization(string preorder) {

    int len = preorder.size();
    vector<char> temp;
    bool flag = true;
    for (int i = 0; i < len; i++) {
        if (flag == true) {
            temp.push_back(preorder[i]);
            flag = false;
        }   
        if (preorder[i] == ',') {
            flag = true;
            continue;
        }
        int sz = temp.size();
        while (sz > 1 && temp[sz - 1] == '#'&&temp[sz - 2] == '#') {
            temp.pop_back();
            temp.pop_back();
            if (temp.empty()) return false;
            temp.pop_back();        
            temp.push_back('#');
            sz = temp.size();
        }
    }
    return temp.size()==1&&temp[0]=='#';
}
```





