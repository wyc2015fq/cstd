# STL容器删除元素的陷阱 - DoubleLi - 博客园






今天看Scott Meyers大师的stl的用法，看到了我前段时间犯的一个错误，发现我写的代码和他提到错误代码几乎一模一样，有关stl容器删除元素的问题，错误的代码如下：
std::vector<struct> mFriendList;
...
std::vector<struct>::iterator iter = mFriendList.begin();
for ( ; iter != mFriendList.end(); ++iter)
{
    if (...)
        mFriendList.erase(iter);
}
记得当时Once给我说过这个问题，还给我改过代码，我当时不明白为什么，只知道程序执行的时候如果if为true那么程序就肯定会崩溃。
大师的说法是：当容易中的一个元素被删除时，指向该元素的所有迭代器都变得无效。上面的代码中，只要执行了erase(iter),那么iter就会变得无效，那么执行++iter就肯定会出错。

在网上看到有人总结如下两条：
1. 对于节点式容器(map, list, set)元素的删除，插入操作会导致指向该元素的迭代器失效，其他元素迭代器不受影响
2. 对于顺序式容器(vector，string，deque)元素的删除、插入操作会导致指向该元素以及后面的元素的迭代器失效

总结了一下，并回想Once当时给我改的代码，所以正确的写法应该是这样的：
1.对于节点式容器
std::list<struct> mList;
...
std::list<struct>::iterator iter = mList.begin();
for ( ; iter != mList.end(); )
{
    if (...)
    {
        //因为节点式只会导致当前节点迭代器失效，所以删除节点的同时对迭代器进行后移的操作，因为其他元素不会失效
        mList.erase(iter++);
    }
    else
    {
        ++iter;
    }
}

2.对于顺序式容器
std::vector<struct> mVector;
...
std::vector<struct>::iterator iter = mVector.begin();
for ( ; iter != mVector.end(); )
{
    if (...)
    {
        //这里就比较有说法了，因为顺序式容器会使本身和后面的元素迭代器都失效，所以不能简单的++操作
        //这里顺序式容器的erase()会返回紧随被删除元素的下一个元素的有效迭代器
        //而节点式容器的erase()的返回值是void，这点我感觉太神奇了，确实太神奇了！！！！
        iter = mVector.erase(iter);
    }
    else
    {
        ++iter;
    }
}

注意：容器看具体STL库的实现了，VS中两类容器的earse都返回下一个迭代器指针。









