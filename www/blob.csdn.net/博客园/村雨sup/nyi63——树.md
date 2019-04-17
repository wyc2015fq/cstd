# nyi63——树 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
#include<bits/stdc++.h>
using namespace std;

int cnt;

struct node
{
    int data;
    int flag;
    node *lchild,*rchild;
    node(){            //无参构造函数
        flag = -1;
        lchild = rchild = NULL;
    }
};

node* createtree(int d)
{
    queue<node*>q;  //注意node*其实是一个类型
    node* root = new(node);     //node *root = new(node) 不一样，直接用node 会出错
    int count = 0;
    root->data = ++count;
    q.push(root);
    node* t = root;
    while(count < (1 << d)-1){
        t = q.front();
        q.pop();
        node* t1 = new(node);
        t1->data = ++count;
        t->lchild = t1;
        q.push(t1);
        node* t2 = new(node);
        t2->data = ++count;
        t->rchild = t2;
        q.push(t2);
    }
    return root;
}

void gogo(node* root)        //这个递归蛮有意思的
{
    if(root->rchild&&root->lchild){
        if(root->flag == -1){
            gogo(root->lchild);
            root->flag = 1;
        }
        else{
            gogo(root->rchild);
            root->flag = -1;
        }
    }
    else{
        cnt = root->data;   //因为递归到最后就是一个叶节点
    }
}




int main()
{
    int d,t;
    while(cin >> d >> t){
        if(d == 0&&t == 0) break;
        cnt = 0;
        node* rt = createtree(d);
        for(int i=0;i < t;i++){
            gogo(rt);
        }
        cout << cnt << endl;
    }
    return 0;
}
```

——











