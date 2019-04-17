# [CareerCup] 10.2 Data Structures for Large Social Network 大型社交网站的数据结构 - Grandyang - 博客园







# [[CareerCup] 10.2 Data Structures for Large Social Network 大型社交网站的数据结构](https://www.cnblogs.com/grandyang/p/4853252.html)







10.2 How would you design the data structures for a very large social network like Facebook or Linkedln? Describe how you would design an algorithm to show the connection, or path, between two people (e.g., Me -> Bob -> Susan -> Jason -> You).



这道题让我们实现大型社交网站的数据结构，首先用户类Person需要包含好友和其他的一些信息，而且大型网站一般可能会有上百万的用户，我们一般不可能把所有的数据都存在一台机器上，所以我们在查找好友时，需要先查找好友所在的机器，再在机器上查询好友，每个好友或机器都有自己的编号，为了快速查找，均使用了哈希表来建立映射，参见代码如下：



```
class Person {
public:
    Person(int id): _personID(id) {}
    int getID() { return _personID; }
    void addFriend(int id) { _friendIDs.push_back(id); }
    
private:
    vector<int> _friendIDs;
    int _personID;
};

class Machine {
public:
    unordered_map<int, Person*> _persons;
    int _machineID;
    Person* getPersonWithID(int personID) {
        if (_persons.find(personID) == _persons.end()) {
            return nullptr;
        }
        return _persons[personID];
    }
};

class Server {
public:
    unordered_map<int, Machine*> _machines;
    unordered_map<int, int> _personToMachineMap;
    Machine* getMatchineWithId(int machineID) {
        if (_machines.find(machineID) == _machines.end()) {
            return nullptr;
        }
        return _machines[machineID];
    }
    int getMachineIDForUser(int personID) {
        if (_personToMachineMap.find(personID) == _personToMachineMap.end()) {
            return -1;
        }
        return _personToMachineMap[personID];
    }
    Person* getPersonWithID(int personID) {
        if (_personToMachineMap.find(personID) == _personToMachineMap.end()) {
            return nullptr;
        }
        int machineID = _personToMachineMap[personID];
        Machine *machine = getMatchineWithId(machineID);
        if (machine == nullptr) return nullptr;
        return machine->getPersonWithID(personID);
    }
};
```



优化：减少机器跳跃

机器之间的跳跃花费大，我们一般不会在机器之间进行随机跳跃，一般若我有好多个好友在同一个机器上，会将他们归到一起访问。

优化：智能的分类人和机器

由于人们更有可能会添加和他们来自同一个国家的人，所以将同一个城市，州，国家的人都尽量存贮到同一台机器上，这样查找时会减少机器跳跃

问题：BFS搜索需要将点标记为已读，这里怎样处理?

由于可能会有很多个搜索同时进行，所以我们不会对数据进行直接标记，但我们会使用哈希表来建立映射来标记数据是否访问过。

还有一些其他的问题可以考虑:

1. 在现实中，如果服务器崩溃了怎么办？

2. 你怎么利用好缓存功能？

3. 你会搜到图的尽头吗，你怎么决定什么时候停止搜索？

4. 实际中，每个人的朋友数都不同，有人想在你和别人之间产生一个好友链，你该怎么用这数据确定在哪开始遍历？














