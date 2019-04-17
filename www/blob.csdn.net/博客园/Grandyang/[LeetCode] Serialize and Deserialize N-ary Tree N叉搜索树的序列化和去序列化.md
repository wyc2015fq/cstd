# [LeetCode] Serialize and Deserialize N-ary Tree N叉搜索树的序列化和去序列化 - Grandyang - 博客园







# [[LeetCode] Serialize and Deserialize N-ary Tree N叉搜索树的序列化和去序列化](https://www.cnblogs.com/grandyang/p/9945453.html)









Serialization is the process of converting a data structure or object into a sequence of bits so that it can be stored in a file or memory buffer, or transmitted across a network connection link to be reconstructed later in the same or another computer environment.

Design an algorithm to serialize and deserialize an N-ary tree. An N-ary tree is a rooted tree in which each node has no more than N children. There is no restriction on how your serialization/deserialization algorithm should work. You just need to ensure that an N-ary tree can be serialized to a string and this string can be deserialized to the original tree structure.

For example, you may serialize the following `3-ary` tree



![](https://assets.leetcode.com/uploads/2018/10/12/narytreeexample.png)



as `[1 [3[5 6] 2 4]]`. You do not necessarily need to follow this format, so please be creative and come up with different approaches yourself.



Note:
- `N` is in the range of `[1, 1000]`
- Do not use class member/global/static variables to store states. Your serialize and deserialize algorithms should be stateless.





这道题让我们对N叉树进行序列化和去序列化，[序列化](https://zh.wikipedia.org/wiki/%E5%BA%8F%E5%88%97%E5%8C%96)就是将一个数据结构或物体转化为一个位序列，可以存进一个文件或者内存缓冲器中，然后通过网络连接在相同的或者另一个电脑环境中被还原，还原的过程叫做去序列化。现在让我们来序列化和去序列化一个二叉树，并给了我们例子。由于我们有了之前那道[Serialize and Deserialize Binary Tree](http://www.cnblogs.com/grandyang/p/4913869.html)对二叉树的序列化和去序列化的基础，那么这道N叉树的方法也是大同小异了。首先使用先序遍历的递归解法，递归的写法就十分的简洁了，对于序列化，我们需要一个helper函数，里面首先判断结点，若为空，则结果res加上一个井字符，否则加上当前结点值，跟一个空格，再加上子结点的个数值，再跟一个空格。之后就是遍历子结点了，对每个子结点都调用递归函数即可。去序列函数需要用一个字符串流类来帮助读字符，这个类是按空格来断开字符串的，所以我们在序列化的时候中间都是用的空格。我们同样需要一个helper函数，首先读出结点值，如果读出了井字号，直接返回空。否则继续读出子结点的个数，有了结点值我们就可以新建一个结点了，同时知道了子结点的个数，那么我们就循环调用递归函数相同的次数，将返回的子结点加入子结点数组即可，参见代码如下：



解法一：

```
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(Node* root) {
        string res;
        serializeHelper(root, res);
        return res;
    }
    
    void serializeHelper(Node* node, string& res) {
        if (!node) res += "#";
        else {
            res += to_string(node->val) + " " + to_string(node->children.size()) + " ";
            for (auto child : node->children) {
                serializeHelper(child, res);
            }
        }
    }

    // Decodes your encoded data to tree.
    Node* deserialize(string data) {
        istringstream iss(data);
        return deserializeHelper(iss);
    }
    
    Node* deserializeHelper(istringstream& iss) {
        string val = "", size = "";
        iss >> val;
        if (val == "#") return NULL;
        iss >> size;
        Node *node = new Node(stoi(val), {});
        for (int i = 0; i < stoi(size); ++i) {
            node->children.push_back(deserializeHelper(iss));
        }
        return node;
    }
};
```



我们还可以使用层序遍历的迭代写法，序列化的函数相对来说好一点，还是先判空，若为空，直接返回井字号。否则就使用队列，加入根结点，然后就进行while循环，先取出队首结点，然后res加入结点值，再加入空格，加入子结点个数，再加上空格。之后再把每一个子结点都加入队列中即可。去序列化函数稍稍复杂一些，还是要用字符流类来读取字符，需要用两个队列，分别来保存结点，和子结点个数。首先我们先取出结点值，如果是井字号，直接返回空。否则再取出子结点个数，我们先根据之前取出的结点值新建一个结点，然后加入结点队列，把子结点个数加入个数队列。然后就开始遍历了，首先分别取出结点队列和个数队列的队首元素，然后循环子结点个数次，再取出结点值，和子结点个数，如果其中某个值没取出来，就break掉。根据取出的结点值新建一个结点，然后将结点值加入结点队列，子结点个数加入个数队列，然后将子结点加入子结点数组，参见代码如下：



解法二：

```
class Codec {
public:

    // Encodes a tree to a single string.
    string serialize(Node* root) {
        if (!root) return "#";
        string res;
        queue<Node*> q{{root}};
        while (!q.empty()) {
            Node *t = q.front(); q.pop();
            res += to_string(t->val) + " " + to_string(t->children.size()) + " ";
            for (Node *child : t->children) {
                q.push(child);
            }
        }
        return res;
    }

    // Decodes your encoded data to tree.
    Node* deserialize(string data) {
        istringstream iss(data);
        queue<Node*> qNode;
        queue<int> qSize;
        string val = "", size = "";
        iss >> val;
        if (val == "#") return NULL;
        iss >> size;
        Node *res = new Node(stoi(val), {}), *cur = res;
        qNode.push(cur);
        qSize.push(stoi(size));
        while (!qNode.empty()) {
            Node *t = qNode.front(); qNode.pop();
            int len = qSize.front(); qSize.pop();
            for (int i = 0; i < len; ++i) {
                if (!(iss >> val)) break;
                if (!(iss >> size)) break;
                cur = new Node(stoi(val), {});
                qNode.push(cur);
                qSize.push(stoi(size));
                t->children.push_back(cur);
            }
        }
        return res;
    }
};
```



类似题目：

[Serialize and Deserialize BST](http://www.cnblogs.com/grandyang/p/6224510.html)

[Serialize and Deserialize Binary Tree](http://www.cnblogs.com/grandyang/p/4913869.html)

[Encode N-ary Tree to Binary Tree](https://www.cnblogs.com/grandyang/p/9945345.html)



参考资料：

[https://leetcode.com/problems/serialize-and-deserialize-n-ary-tree](https://leetcode.com/problems/serialize-and-deserialize-n-ary-tree)



[LeetCode All in One 题目讲解汇总(持续更新中...)](http://www.cnblogs.com/grandyang/p/4606334.html)












