# LintCode 249 统计前面比自己小的数的个数【线段树】 - 紫芝的博客 - CSDN博客





2018年10月22日 14:55:47[紫芝](https://me.csdn.net/qq_40507857)阅读数：94








# [249. Count of Smaller Number before itself](https://www.lintcode.com/problem/count-of-smaller-number-before-itself/description)

### Description

Give you an integer array (index from 0 to n-1, where n is the size of this array, data value from 0 to 10000) . For each element `Ai` in the array, count the number of element before this element `Ai` is smaller than it and return count number array.

We suggest you finish problem [Segment Tree Build](http://www.lintcode.com/problem/segment-tree-build/), [Segment Tree Query II](http://lintcode.com/en/problem/segment-tree-query-ii/) and [Count of Smaller Number](http://www.lintcode.com/zh-cn/problem/count-of-smaller-number//) first.

### Clarification

Before you do this, you'd better complete the following three questions： [Segment Tree Build](http://www.lintcode.com/problem/segment-tree-build/)， [Segment Tree Query II](http://lintcode.com/en/problem/segment-tree-query-ii/)，and [Count of Smaller Number before itself I](http://lintcode.com/en/problem/count-of-smaller-number-before-itself-i/) 。

### Example

For array `[1,2,7,8,5]`, return `[0,1,2,3,2]`

### 题意：

给定一个整数数组（下标由 0 到 n-1， n 表示数组的规模，取值范围由 0 到10000）。对于数组中的每个 `ai` 元素，请计算 `ai` 前的数中比它小的元素的数量。

### 思路1

每个线段树节点储存的是原始数列index的前后值，即该区间内的最大值，在查询时，通过区域以及最大值约束，找到所有小于特定值的区间，然后求和即为前面比自己小的数的个数

适用范围：数列元素值较大：a[i]<1e9，数列元素较少：i<1e5

```cpp
#include<bits/stdc++.h>
using namespace std;
const int maxn=1e5+10;
//每个线段树节点存储原始数组的index前后值以及该区间内对应的最大值
#define nullptr NULL
class Solution {
struct SegmentTreeNode{  //线段树节点，其中max是当前区域内（left-right）最大值
public:
    int start,end,max;
    SegmentTreeNode * left;
    SegmentTreeNode * right;
    SegmentTreeNode(int x,int y,int max){
        this->start = x;
        this->end = y;
        this->max = max;
        this->left = this->right = nullptr;
    }
};
public:
    /**
     * @param A: an integer array
     * @return: A list of integers includes the index of the first number and the index of the last number
     */
    vector<int> countOfSmallerNumberII(vector<int> &A) {
        // write your code here
        //auto tree = new SegmentTreeNode(0,A.size()-1,INT_MIN);
        int len=A.size();
        SegmentTreeNode root=SegmentTreeNode(0,len-1,INT_MIN);
        SegmentTreeNode *tree=&root;
        buildTree(A,tree);
        vector<int> ret;
        for(int i = 0;i<len;++i){
            ret.push_back(query(tree,0,i,A[i]));
        }
        return ret;
    }

    int buildTree(vector<int> &A,SegmentTreeNode * root){ //建立线段树，每个节点保存该区域内最大值
        int start = root->start;
        int end = root->end;
        if(start > end) return 0;
        if(start == end) {
            root->max = A[start];
            return A[start];
        }else{
            root->left = new SegmentTreeNode(start,(start+end)/2,INT_MIN);
            root->right = new SegmentTreeNode((start+end)/2+1,end,INT_MIN);
            int L_max = buildTree(A,root->left);
            int R_max = buildTree(A,root->right);
            root->max = L_max>R_max?L_max:R_max;
            return root->max;
        }
    }

    int query(SegmentTreeNode * root,int start,int end,int q){  //查询特定区域比q小的个数
        if(root == nullptr) return 0;
        if(root->start > end || root->end < start) return 0;
        if(root->start >= start && root->end <= end && root->max<q) return root->end - root->start + 1;
        return query(root->left,start,end,q)+query(root->right,start,end,q);
    }
};
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF){
        vector<int> A(n);
    for(int i=0;i<n;i++)
    scanf("%d",&A[i]);

    Solution work;
    vector<int> res;
    res=work.countOfSmallerNumberII(A);

    for(int i=0;i<n;i++)
    {
        printf("%d",res[i]);
        if(i==n-1)
            printf("\n");
        else
            printf(" ");
    }
}
    return 0;
}
/*
5
1 2 7 8 5
*/
```

使用数组来存储二叉树，无指针版 

```cpp
#include<bits/stdc++.h>
using namespace std;
#define nullptr NULL
const int maxn=1e5+10;
//每个线段树节点存储原始数组的index前后值以及该区间内对应的最大值

struct SegmentTreeNode{  //线段树节点，其中max是当前区域内（left-right）最大值
    int start,end,max;
}segtree[maxn<<2];

class Solution {


public:
    /**
     * @param A: an integer array
     * @return: A list of integers includes the index of the first number and the index of the last number
     */
    vector<int> countOfSmallerNumberII(vector<int> &A) {
        // write your code here
        int len=A.size();
        buildTree(1,1,len,A);
        vector<int> ret;
        for(int i = 1;i<=len;++i){
            ret.push_back(query(1,1,i,A[i]));
        }
        return ret;
    }

    void pushup(int root){
        segtree[root].max=max(segtree[root<<1].max,segtree[root<<1|1].max);
    }

//建立线段树，每个节点保存该区域内最大值
    void buildTree(int root,int start,int end,vector<int> &A){
        segtree[root].start=start;
        segtree[root].end=end;
        if(start==end){
            segtree[root].max=A[start];
            return ;
        }
        int mid=(start+end)/2;
        buildTree(root<<1,start,mid,A);
        buildTree(root<<1|1,mid+1,end,A);
        pushup(root);
    }
//查询特定区域比q小的个数
    int query(int root,int start,int end,int q){
        if(segtree[root].start > end || segtree[root].end < start) return 0;
        if(segtree[root].start >= start && segtree[root].end <= end && segtree[root].max<q)
            return segtree[root].end - segtree[root].start + 1;
        return query(root<<1,start,end,q)+query(root<<1|1,start,end,q);
    }
};
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF){
        vector<int> A(n+1);
    for(int i=1;i<=n;i++)
    scanf("%d",&A[i]);

    Solution work;
    vector<int> res;
    res=work.countOfSmallerNumberII(A);

    for(int i=0;i<n;i++)
    {
        printf("%d",res[i]);
        if(i==n-1)
            printf("\n");
        else
            printf(" ");
    }
}
    return 0;
}
/*
5
1 2 7 8 5
*/
```

 思路2

对数据的区间建立线段树，在知道所有数据上届【0，1000】（数据比较集中）的情况下效率不错。

```cpp
class Solution {
struct SegmentTreeNode{//count表示当前区间所有的数个数
public:
    int start,end,count;
    SegmentTreeNode * left;
    SegmentTreeNode * right;
    SegmentTreeNode(int x,int y,int count){
        this->start = x;
        this->end = y;
        this->count = count;
        this->left = this->right = nullptr;
    }
};
public:
    /**
     * @param A: an integer array
     * @return: A list of integers includes the index of the first number and the index of the last number
     */
    vector<int> countOfSmallerNumberII(vector<int> &A) {
        // write your code here
        vector<int> res;
        SegmentTreeNode * root = buildTree(0,10001);
        for(int i=0; i<A.size(); ++i){
            res.push_back(query(root,A[i]));
            insert(root,A[i]);
        }
        return res;
    }
 
    SegmentTreeNode* buildTree(int start,int end){  //这种方法需要明确数据上界，然后直接根据数据大小建立线段树，每个节点保存落在当前区间数的个数
        if(start > end) return nullptr;
        auto res = new SegmentTreeNode(start,end,0);
        if(start == end) return res;
        int mid = (start+end)/2;
        res->left = buildTree(start,mid);
        res->right = buildTree(mid+1,end);
        return res;
    }
 
    int query(SegmentTreeNode * root,int q){   //query函数用来查询当前区域内小于q的数的个数
        if(root == nullptr) return 0;
        if(q < root->start) return 0;
        if(q > root->end) return root->count;
        return query(root->left,q)+query(root->right,q);
    }
 
    void insert(SegmentTreeNode * root,int val){//将输入数据逐个插入，从上到下逐个更新count
        if(root == nullptr) return;
        if(root->start > val || root->end < val) return;
        if(root->start <= val && root->end >= val) ++root->count;
        insert(root->left,val);
        insert(root->right,val);
    }
};
```

思路3

通过缓存区间点的“比其小的数的个数”，从而舍弃部分已知小的数，使用类似二叉树的结构，遇小放左，遇大放右；那么a[i]元素在树中所有左节点的个数，即为a[i]前面小于比自己小的节点的个数

```cpp
#include<bits/stdc++.h>
using namespace std;
#define nullptr NULL

class Solution {
public:
	struct node{
		int count=0;//小于data元素的个数 
		int data=0;//数列元素 
		int rep=0;//repeat，重复元素的个数 
		node* left=nullptr;
		node* right=nullptr;
		node(int num){data=num;}
		~node(){
			if(left)	delete left;
			if(right)	delete right;
		}	
	};
	
public:
    /**
     * @param A: an integer array
     * @return: A list of integers includes the index of the first number and the index of the last number
     */
    vector<int> countOfSmallerNumberII(vector<int> &A) {
        // write your code here
        int len=A.size();
		vector<int> res(len);
		res[0]=0;
		node* root=new node(A[0]);
		node* temp=nullptr;
		int count=0;
		for(int i=1;i<len;i++){
			temp=root;
			count=0;
			while(true){
				if(temp->data > A[i]){
					temp->count++;
					if(temp->left)
					temp=temp->left;
					else{
						temp->left=new node(A[i]);
						break;
					}
				}else if(temp->data < A[i]){
					count+=(temp->count+temp->rep+1);
					if(temp->right){
						temp=temp->right;
					}else{
						temp->right=new node(A[i]);
						break;
					}
				}else{
					count+=temp->count;
					temp->rep++;
					break;
				}
			}
		res[i]=count;
		}//for
		delete root;
		return res;
    }
};
int main()
{
    int n;
    while(scanf("%d",&n)!=EOF){
        vector<int> A(n);
    for(int i=0;i<n;i++)
    scanf("%d",&A[i]);

    Solution work;
    vector<int> res;
    res=work.countOfSmallerNumberII(A);

    for(int i=0;i<n;i++)
    {
        printf("%d",res[i]);
        if(i==n-1)
            printf("\n");
        else
            printf(" ");
    }
}
    return 0;
}
/*
5
1 2 7 8 5
*/
```





