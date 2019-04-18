# 二分查找--AVL查找树 - Orisun - 博客园







# [二分查找--AVL查找树](https://www.cnblogs.com/zhangchaoyang/articles/1808928.html)






二分查找可以找到元素在数组中的下标位置，而查找树由于采用的是树结构，所以只能判断出元素是不是在树中以及给出最大/小值，而不能确定指定元素的确切位置。

**二分查找**


#include<vector>

#include<iostream>

#include<algorithm>

#include<cstdlib>

#include<iomanip>		//cout格式化输出

#include<ctime>

using namespace std;

 

const int NOT_FOUND=-1;

const long MUL=16807L;             

const long ADD=13849;                   

const long MOD=0x7fffffffL;   

 

//线性同余法产生无符号随机数

inline unsigned myRand(){

 static long seed=(long)time(NULL);

 seed=(seed*MUL+ADD)%MOD;

 return (unsigned)seed>>17;      //linear congruential method高16位随机性比较好,我们只用它的高15位,返回值在[0,32767]

}

 

template <typename Comparable>

int binarySearch(const vector<Comparable> & a,const Comparable & x){

    int low=0;

    int high=a.size()-1;

    while(low<=high){

int mid=(low+high)/2;

if(x>a[mid])

    low=mid+1;

else if(x<a[mid])

    high=mid-1;

else

    return mid;

    }

    return NOT_FOUND;

}

 

int main(){

    clock_t begin=clock();

    vector<int> v;

    for(int i=0;i<1000000;i++)//生成一百万个随机数进行排序

	v.push_back((int)myRand());  

    sort(v.begin(),v.end());

    int position=binarySearch(v,20109);

    clock_t finish=clock();

    long elapse=(long)(finish-begin);

    cout<<"find the element in position: "<<position<<endl;

    cout<<"elapsed time:"<<setprecision(5)<<(float)elapse/CLOCKS_PER_SEC<<"s"<<endl;

 

    return 0;

} 





clock_t的值转换为秒应该是除以CLOCKS_PER_SEC这个宏，而这个宏在Windows平台下是1000，而到了Linux平台下就是1000000了



**AVL查找树**


#include<iostream>
#include<iomanip>


using namespace std;



const long MUL=16807L;             

const long ADD=13849L;                   

const long MOD=0x7fffffffL;   



//线性同余法产生无符号随机数

inline unsigned myRand(){

 static long seed=(long)time(NULL);

 seed=(seed*MUL+ADD)%MOD;

 return (unsigned)seed>>17;      //linear congruential method高16位随机性比较好,我们只用它的高15位,返回值在[0,32767]

}



//定义平衡二叉树

struct AvlNode{

    int element;

    AvlNode *left;

    AvlNode *right;

    int height;



    AvlNode(const int & theElement,AvlNode *lt,AvlNode *rt,int h=0):

	element(theElement),left(lt),right(rt),height(h){}

};

//计算节点的高度

int height(AvlNode * t){

    return t==0?-1:t->height;

}

//返回两个整值的最大者

int max(const int l,const int r){

if(l>r)

return l;

else

return r;

}

//左旋

void rotateWithLeftChild(AvlNode * & k2){

    AvlNode *k1=k2->left;

    k2->left=k1->right;

    k1->right=k2;

    k2->height=max(height(k2->right),height(k2->left))+1;

    k1->height=max(height(k1->left),k2->height)+1;

    k2=k1;

}

//右旋

void rotateWithRightChild(AvlNode * & k2){

    AvlNode *k1=k2->right;

    k2->right=k1->left;

    k1->left=k2;

    k2->height=max(height(k2->left),height(k2->right))+1;

    k1->height=max(height(k1->right),k2->height)+1;

    k2=k1;

}

//左--右旋

void doubleWithRightChild(AvlNode * & k3){

    rotateWithLeftChild(k3->right);

    rotateWithRightChild(k3);

}

//右--左旋

void doubleWithLeftChild(AvlNode * & k3){

    rotateWithRightChild(k3->left);

    rotateWithLeftChild(k3);

}

//插入新节点

void insert(const int & x,AvlNode * & t){

    if(t==0)

t=new AvlNode(x,0,0);

    else if(x<t->element){

insert(x,t->left);

if(height(t->left)-height(t->right)==2){

    if(x<t->left->element)

rotateWithLeftChild(t);

    else

doubleWithLeftChild(t);

}

    }

    else if(x>t->element){

insert(x,t->right);

if(height(t->right)-height(t->left)==2){

    if(x>t->right->element)

rotateWithRightChild(t);

    else

doubleWithRightChild(t);

}

    }

    else

;//Duplicate;Do nothing

    t->height=max(height(t->left),height(t->right))+1;

}



//判断树中是否包含要查找的节点

bool contains(const int & x,AvlNode * t){

    if(t==0)

return false;

    else if(x<t->element)

return contains(x,t->left);

    else if(x>t->element)

return contains(x,t->right);

    else

return true;

}



int main(){

    clock_t begin=clock();

    AvlNode * root=new AvlNode(0,0,0);

    for(int i=0;i<1000000;i++)//生成一百万个随机数进行排序

insert((int)myRand(),root);



    if(contains(20109,root))

cout<<"find"<<endl;

    else

        cout<<"not find"<<endl;

    clock_t finish=clock();

    long elapse=(long)(finish-begin);

    cout<<"elapsed time:"<<setprecision(5)<<(float)elapse/CLOCKS_PER_SEC<<"s"<<endl;



    return 0;

}



**比较结果**

生成一百万个随机数，在我的机子上进行查找，运行10次的平均时间（建立存储结构+查找时间）：秒  
|二分查找|平衡二叉查找树|
|----|----|
|0.67|1.11|













