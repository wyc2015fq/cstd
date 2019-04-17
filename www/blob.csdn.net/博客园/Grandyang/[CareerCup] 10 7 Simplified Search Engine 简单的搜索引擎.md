# [CareerCup] 10.7 Simplified Search Engine 简单的搜索引擎 - Grandyang - 博客园







# [[CareerCup] 10.7 Simplified Search Engine 简单的搜索引擎](https://www.cnblogs.com/grandyang/p/4874236.html)







10.7 Imagine a web server for a simplified search engine. This system has 100 machines to respond to search queries, which may then call out using processSearch(string query) to another cluster of machines to actually get the result. The machine which responds to a given query is chosen at random, so you can not guarantee that the same machine will always respond to the same request. The method processSearch is very expensive. Design a caching mechanism for the most recent queries. Be sure to explain how you would update the cache when data changes.



这道题说假设有一个简单搜索引擎的网络服务器，系统共有100个机子来响应检索，可以用processSearch(string query)来得到其他机子上的结果，每台机子响应检索是随机的，不保证每个机子都会响应到同一个请求。processSearch方法非常昂贵，设计一个缓存机制来应对近期检索。根据书中描述，我们先来做一些假设：

1. 与其说根据需要调用processSearch，倒不如设定所有的检索处理发生在第一个被调用的机子上。

2. 我们需要缓存的检索是非常大量的。

3. 机器之间的调用很快。

4. 检索的结果是一个有序的URL链表，每个URL由50个字符的标题和200个字符的概要组成。

5. 最常访问的检索会一直出现的缓存器中。



系统需求：

主要需要实现下列两个功能：

1. 高效查找当给定了一个关键字时

2. 新数据会代替旧数据的位置

我们还需要更新和清楚缓存当搜索结果改变了。由于一些检索非常的常见病永久的在缓存器中，我们不能等缓存器自然失效。



步骤一：设计单个系统的存存器

我们可以混合使用链表和哈希表来实现，我们建立一个链表，当某个节点被访问了，自动将其移到开头，这样链表的末尾就是最老的数据。我们用哈希表来建立检索和链表中节点的映射，这样不仅可以让我们高效的返回缓存的结果，而且可以把节点移到链表前段，参见代码如下：



```
class Node {
public:
    Node *pre;
    Node *next;
    vector<string> results;
    string query;
    Node(string q, vector<string> res) {
        results = res;
        query = q;
    }
};

class Cache {
public:
    const static int MAX_SIZE = 10;
    Node *head, *tail;
    unordered_map<string, Node*> m;
    int size = 0;
    Cache() {}
    void moveToFront(Node *node) {
        if (node == head) return;
        removeFromLinkedList(node);
        node->next = head;
        if (head != nullptr) {
            head->pre = node;
        }
        head = node;
        ++size;
        if (tail == nullptr) {
            tail = node;
        }
    }
    void moveToFront(string query) {
        if (m.find(query) == m.end()) return;
        moveToFront(m[query]);
    }
    void removeFromLinkedList(Node *node) {
        if (node == nullptr) return;
        if (node->next != nullptr || node->pre != nullptr) {
            --size;
        }
        Node *pre = node->pre;
        if (pre != nullptr) {
            pre->next = node->next;
        }
        Node *next = node->next;
        if (next != nullptr) {
            next->pre = pre;
        }
        if (node == head) {
            head = next;
        }
        if (node == tail) {
            tail = pre;
        }
        node->next = nullptr;
        node->pre = nullptr;
    }
    vector<string> getResults(string query) {
        if (m.find(query) == m.end()) return vector<string>();
        Node *node = m[query];
        moveToFront(node);
        return node->results;
    }
    void insertResults(string query, vector<string> results) {
        if (m.find(query) != m.end()) {
            Node *node = m[query];
            node->results = results;
            moveToFront(node);
            return;
        }
        Node *node = new Node(query, results);
        moveToFront(node);
        m[query] = node;
        if (size > MAX_SIZE) {
            for (unordered_map<string, Node*>::iterator it = m.begin(); it != m.end(); ++it) {
                if (it->first == tail->query) m.erase(it);
            }
            removeFromLinkedList(tail);
        }
    }
};
```



步骤二： 扩展到多个机子

对于多个机子，我们有许多中选择:

选择1：每个机子有自己的缓存器，这种方法的好处是快速，因为没有机子间的调用，但是缺点是不高效

选择2：每个机子都有缓存器的拷贝，当新项目添加到缓存器，发送给所有的机器，设计目的是为了让常用检索存在于所有机子上，缺点是缓存器的空间有限，无法保存大量数据

选择3：每个机器保存缓存器的一部分，当机器i需要得到一个检索的结果，它需要找出哪个机子有这个结果，并到那个机子上取结果。但是问题是机子i怎么知道那个机子上有结果，我们可以用哈希表来建立映射，hash(query)%N，能快速知道哪个机子有想要的结果。



步骤三：更新结果当内容改变

有些检索很常用，所以会永远存在缓存器中，我们需要一些机制能更新结果，当其内容改变了，缓存器中的结果页应该相应变换，主要有下列三种情况：

1. URL的内容改变了

2. 当网页的排行改变了，那么结果的顺序也变了

3. 对于特定的检索有了新的页面

对于1和2，我们建立单独的哈希表告诉我们哪一个检索和哪个URL之间有映射，这个可以在不同的机子上分别完成，但是可能会需要很多数据。另外，如果数据不需要即时刷新，我们也可以周期性的来更新缓存器。对于3，我们可以实现一个自动超时机制，我们设定一个时间段，如果在这个时间段里没有检索，不管它之间被检索的有多频繁，我们都清除它，这样保证了所有数据都会被周期性的更新。



步骤四：进一步地增强

一个优化是当某个检索特别频繁时，比如一个检索占了1%的比重时，我们与其让机器i发送请求给机器j，倒不如在机器i上将结果存在自己的缓存器中。

再有就是我们将检索根据哈希值分别不同机器，而不是随机分配。

另外一个优化就是之前提到的自动超时Automatic Time Out机制，就是x分钟后自动清除数据，但是有时候我们对不同的数据希望设定不同的x值，这样每一个URL都有一个超时值基于此页面过去被更新的频率。














