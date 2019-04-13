
# 599. Minimum Index Sum of Two Lists - OraYang的博客 - CSDN博客

2017年08月28日 16:37:16[OraYang](https://me.csdn.net/u010665216)阅读数：186标签：[C++																](https://so.csdn.net/so/search/s.do?q=C++&t=blog)[leetcode																](https://so.csdn.net/so/search/s.do?q=leetcode&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=C++&t=blog)个人分类：[Leetcode																](https://blog.csdn.net/u010665216/article/category/7026962)
所属专栏：[基础算法](https://blog.csdn.net/column/details/16604.html)



题目：
Suppose Andy and Doris want to choose a restaurant for dinner, and they both have a list of favorite restaurants represented by strings.
You need to help them find out theircommon interestwith theleast list index sum. If there is a choice tie between answers, output all of them with no order requirement. You could assume
 there always exists an answer.
Example 1:
Input:["Shogun", "Tapioca Express", "Burger King", "KFC"]
["Piatti", "The Grill at Torrey Pines", "Hungry Hunter Steakhouse", "Shogun"]Output:["Shogun"]Explanation:The only restaurant they both like is "Shogun".
Example 2:
Input:["Shogun", "Tapioca Express", "Burger King", "KFC"]
["KFC", "Shogun", "Burger King"]Output:["Shogun"]Explanation:The restaurant they both like and have the least index sum is "Shogun" with index sum 1 (0+1).
Note:
The length of both lists will be in the range of [1, 1000].
The length of strings in both lists will be in the range of [1, 30].
The index is starting from 0 to the list length minus 1.
No duplicates in both lists.
思路：本题思路很简单，就便利比较即可，使用两层for循环
代码：

```python
class Solution {
public:
    vector<string> findRestaurant(vector<string>& list1, vector<string>& list2) {
        int min = 2000,count;
        vector<string> str;
        for(int i=0;i<list1.size();i++)
        {
            for(int j =0;j<list2.size();j++)
            {
                if(list1[i]==list2[j])
                {
                    count=i+j;
                    if(min>count)
                    {
                        str.clear();
                        str.push_back(list1[i]);
                        min = count;
                    }
                    else if(min==count)
                    {
                        str.push_back(list1[i]);
                    }
                    break;
                }
            }
        }
        return str;
    }
};
```


