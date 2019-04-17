# [CareerCup] 13.7 Node Pointer 节点指针 - Grandyang - 博客园







# [[CareerCup] 13.7 Node Pointer 节点指针](https://www.cnblogs.com/grandyang/p/4929763.html)







13.7 Write a method that takes a pointer to a Node structure as a parameter and returns a complete copy of the passed in data structure. The Node data structure contains two pointers to other Nodes.



在这道题让我们通过一个节点指针来复制整个数据结构，节点类Node中包含两个节点指针，我们需要用哈希表来建立原数据结构中每一个节点的地址到相对应的新结构中的地址，这样我们就可以在用DFS的时候知道哪些节点我们已经拷贝过了，就可以直接跳过。通过这种方式来标记访问过的节点可以不用在节点内部存储。拷贝过程如下所示：



```
class Node {
public:
    Node *ptr1;
    Node *ptr2;
};

typedef unordered_map<Node*, Node*> NodeMap;

class Solution {
public:
    Node* copy_structure(Node *root) {
        NodeMap m;
        return copy_recursive(root, m);
    }
    Node* copy_recursive(Node *cur, NodeMap &m) {
        if (cur == nullptr) return nullptr;
        auto it = m.find(cur);
        if (it != m.end()) return it->second;
        Node *node = new Node;
        m[cur] = node;
        node->ptr1 = copy_recursive(cur->ptr1, m);
        node->ptr2 = copy_recursive(cur->ptr2, m);
        return node;
    }
};
```














