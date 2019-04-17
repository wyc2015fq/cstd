# 【剑指offer】题61：二叉树序列化、反序列化 - xiaxzhou的博客 - CSDN博客





2017年09月03日 10:32:58[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：152







- 使用stringstream

> 
[http://blog.csdn.net/xw20084898/article/details/21939811](http://blog.csdn.net/xw20084898/article/details/21939811)


stringstream 是 C++ 提供的另一个字串型的串流(stream)物件，和之前学过的 iostream、fstream 有类似的操作方式。要使用 stringstream， 必須先加入這一行： 
`#include <sstream>`

stringstream 主要是用在將一個字串分割，可以先用 clear( )以及 str( ) 將指定字串設定成一开始的內容，再用 >> 把个別的资料输出，例如：

```cpp
string s;
stringstream ss;
int a；
char b;
string c;
getline(cin, s);
ss.clear();
ss.str(s);
ss >> a >> b >> c;
```

```cpp
#include <iostream>
#include <string>
#include <sstream>
using namespace std;

struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
        val(x), left(NULL), right(NULL) {}
};


string Serialize(TreeNode *root)
{
    string str;
    if (root == NULL)
    {
        str = "$,";
        return str;
    }
    stringstream stream;
    stream << root->val<<",";

    stream << Serialize(root->left);
    stream << Serialize(root->right);
    str = stream.str();
    return str;

}
TreeNode* Deserialize(stringstream& stream)
{
    TreeNode* pRoot(NULL);
    char c;
    stream >> c;
    if (c == '$')
    {
        stream >> c;
        return pRoot;
    }
    int num(0);
    while (c != ',')
    {
        num = num * 10 + c - '0';
        stream >> c;
    }
    pRoot = new TreeNode(num);
    pRoot->left = Deserialize(stream);
    pRoot->right= Deserialize(stream);
    return pRoot;
}


TreeNode* Deserialize(string str)
{
    TreeNode * pRoot(NULL);
    if (str.empty())
    {
        return pRoot;
    }
    stringstream stream(str);
    return Deserialize(stream);
}


int main()
{
    string str = "100,50,$,$,150,$,$";
    TreeNode* root;
    root = Deserialize(str);

    auto tmp = Serialize(root);
    root = Deserialize(str);

    tmp = Serialize(root);
    return 0;
}
```



