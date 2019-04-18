# 剑指offer - 把二叉树打印成多行 - curryche的博客 - CSDN博客





2018年08月21日 16:40:49[curryche](https://me.csdn.net/whwan11)阅读数：37








## 题目

把二叉树打印成多行 

时间限制：1秒 空间限制：32768K 热度指数：87516

题目描述 

从上到下按层打印二叉树，同一层结点从左至右输出。每一层输出一行。

## 解法

代码

```cpp
/*
struct TreeNode {
    int val;
    struct TreeNode *left;
    struct TreeNode *right;
    TreeNode(int x) :
            val(x), left(NULL), right(NULL) {
    }
};
*/
class Solution {
public:
        vector<vector<int> > Print(TreeNode* pRoot) {
            vector<vector<int>> result;
            if(pRoot==NULL)
                return result;

            queue<TreeNode*> nodes;
            queue<bool> labels;
            vector<int> current_layer;
            bool current_label;


            current_label=true;
            nodes.push(pRoot);
            labels.push(true);
            while(!nodes.empty())
            {
                if(labels.front()==current_label)
                {
                    current_layer.push_back(nodes.front()->val);
                }
                else
                {
                    result.push_back(current_layer);
                    current_layer.clear();
                    current_layer.push_back(nodes.front()->val);
                    current_label=(!current_label);
                }

                if(nodes.front()->left!=NULL)
                {
                    nodes.push(nodes.front()->left);
                    labels.push(!current_label);
                }
                if(nodes.front()->right!=NULL)
                {
                    nodes.push(nodes.front()->right);
                    labels.push(!current_label);
                }

                nodes.pop();
                labels.pop();
            }

            if(!current_layer.empty())
                result.push_back(current_layer);

            return result;
        }

};
```



