# LRU Cache 的简单 C++ 实现 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [吳YH堅](http://www.jobbole.com/members/wuyinghao) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
## 什么是 LRU
LRU Cache是一个Cache的置换算法，含义是“最近最少使用”，把满足“最近最少使用”的数据从Cache中剔除出去，并且保证Cache中第一个数据是最近刚刚访问的，因为这样的数据更有可能被接下来的程序所访问。
LRU的应用比较广泛，最基础的内存页置换中就用了，对了，这里有个概念要清楚一下，Cache不见得是CPU的高速缓存的那个Cache，这里的Cache直接翻译为缓存，就是两种存储方式的速度有比较大的差别，都可以用Cache缓存数据，比如硬盘明显比内存慢，所以常用的数据我们可以Cache在内存中。
## LRU 基本算法描述
前提：
由于我只是简单实现一下这个算法，所以数据都用int代替，下一个版本会改成模板形式的，更加通用。
要求：
- 只提供两个接口，一个获取数据getValue(key),一个写入数据putValue(key,value)
- 无论是获取还是写入数据，当前这个数据要保持在最容易访问的位置
- 缓存数量有限，最长时间没被访问的数据应该置换出缓存
算法：
为了满足上面几个条件，实际上可以用一个双向链表来实现，每次访问完数据（不管是获取还是写入），调整双向链表的顺序，把刚刚访问的数据调整到链表的最前方，以后再访问的时候速度将最快。
为了方便，提供一个头和一个尾节点，不存具体的数，链表的基本形式如下面的这个简单表述
Head <===> Node1 <===> Node2 <===> Node3 <===> Near
OK，就这么些，比较简单，实现起来也不难，用c++封装一个LRUCache类，类提供两个方法，分别是获取和更新，初始化类的时候传入Cache的节点数。
先定义一个存数据的节点数据结构


```
typedef struct _Node_{  
  
    int key;    //键  
    int value;  //数据  
      
    struct _Node_ *next;  //下一个节点  
    struct _Node_ *pre;   //上一个节点  
  
}CacheNode;
```
类定义：


```
class LRUCache{  
      
public:  
      
    LRUCache(int cache_size=10);  //构造函数，默认cache大小为10  
    ~LRUCache();<span style="white-space:pre">      </span>      //析构函数  
  
  
    int getValue(int key);             //获取值  
    bool putValue(int key,int value);  //写入或更新值   
    void displayNodes();               //辅助函数，显示所有节点  
      
      
private:  
      
    int cache_size_;                   //cache长度  
    int cache_real_size_;              //目前使用的长度  
    CacheNode *p_cache_list_head;      //头节点指针  
    CacheNode *p_cache_list_near;      //尾节点指针  
          
    void detachNode(CacheNode *node);  //分离节点  
    void addToFront(CacheNode *node);  //将节点插入到第一个  
  
};
```
类实现：


```
LRUCache::LRUCache(int cache_size)  
{  
    cache_size_=cache_size;  
    cache_real_size_=0;  
    p_cache_list_head=new CacheNode();  
    p_cache_list_near=new CacheNode();  
    p_cache_list_head->next=p_cache_list_near;  
    p_cache_list_head->pre=NULL;  
    p_cache_list_near->pre=p_cache_list_head;  
    p_cache_list_near->next=NULL;  
      
}  
  
LRUCache::~LRUCache()  
{  
    CacheNode *p;  
    p=p_cache_list_head->next;  
    while(p!=NULL)  
    {     
        delete p->pre;  
        p=p->next;  
    }  
  
    delete p_cache_list_near;  
      
}  
  
  
void LRUCache::detachNode(CacheNode *node)  
{  
    node->pre->next=node->next;  
    node->next->pre=node->pre;  
}  
  
void LRUCache::addToFront(CacheNode *node)  
{  
    node->next=p_cache_list_head->next;  
    p_cache_list_head->next->pre=node;  
    p_cache_list_head->next=node;  
    node->pre=p_cache_list_head;  
}  
  
  
int LRUCache::getValue(int key)  
{  
    CacheNode *p=p_cache_list_head->next;      
    while(p->next!=NULL)  
    {  
          
        if(p->key == key) //catch node  
        {  
              
            detachNode(p);  
            addToFront(p);  
            return p->value;  
        }     
        p=p->next;     
    }  
    return -1;  
}  
  
  
  
bool LRUCache::putValue(int key,int value)  
{  
    CacheNode *p=p_cache_list_head->next;  
    while(p->next!=NULL)  
    {  
          
          
        if(p->key == key) //catch node  
        {  
            p->value=value;  
            getValue(key);  
            return true;  
        }     
        p=p->next;     
    }  
      
      
    if(cache_real_size_ >= cache_size_)  
    {  
        cout << "free" <<endl;  
        p=p_cache_list_near->pre->pre;  
        delete p->next;  
        p->next=p_cache_list_near;  
        p_cache_list_near->pre=p;  
    }  
      
    p=new CacheNode();//(CacheNode *)malloc(sizeof(CacheNode));  
      
    if(p==NULL)  
        return false;  
  
    addToFront(p);  
    p->key=key;  
    p->value=value;  
          
    cache_real_size_++;  
          
    return true;      
}  
  
  
void LRUCache::displayNodes()  
{  
    CacheNode *p=p_cache_list_head->next;  
      
    while(p->next!=NULL)  
    {  
        cout << " Key : " << p->key << " Value : " << p->value << endl;   
        p=p->next;  
          
    }  
    cout << endl;  
      
}
```
## 说在后面的话
其实，程序还可以优化，首先，把数据int类型换成模板形式的通用类型，另外，数据查找的时候复杂度为O(n)，可以换成hash表来存数据，链表只做置换处理，这样查找添加的时候速度将快很多。
> 
**打赏支持我写出更多好文章，谢谢！**
[打赏作者](#rewardbox)
#### 打赏支持我写出更多好文章，谢谢！
![](http://jbcdn2.b0.upaiyun.com/2016/09/db956e69f586b9e6b487bc448ef34dcb.jpeg)
