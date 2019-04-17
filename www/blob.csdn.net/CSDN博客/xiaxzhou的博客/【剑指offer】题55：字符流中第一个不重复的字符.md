# 【剑指offer】题55：字符流中第一个不重复的字符 - xiaxzhou的博客 - CSDN博客





2017年07月06日 21:43:41[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：194








> 
使用哈希表记录次数，使用链表记录顺序 

  哈希表保存指向链表的指针，用于删除重复出现的字符


数组中第一个只出现一次的字符问题也可以使用类似的方法，这样只需遍历一遍字符串即可得到结果

```cpp
class Solution
{
public:
    Solution(){ vec.resize(256); };

    //Insert one char from stringstream
    void Insert(char ch)
    {
        if (vec[ch].first == 0)
        {
            vec[ch].first = 1;
            lis.push_back(ch);
            vec[ch].second = (--lis.end());//前置--
        }
        else 
        {
            if (vec[ch].first == 1)
            {
                vec[ch].first++; 
                lis.erase(vec[ch].second);
            }
        }
    }
    //return the first appearence once char in current stringstream
    char FirstAppearingOnce()
    {
        if (!lis.empty())
        {
            return *lis.begin();
        }
        else
            return '#';
    }

private:
    vector<pair<int,list<char>::iterator>> vec;
    list<char> lis;
};
```



