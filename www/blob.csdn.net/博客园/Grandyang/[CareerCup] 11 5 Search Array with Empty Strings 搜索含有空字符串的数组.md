# [CareerCup] 11.5 Search Array with Empty Strings 搜索含有空字符串的数组 - Grandyang - 博客园







# [[CareerCup] 11.5 Search Array with Empty Strings 搜索含有空字符串的数组](https://www.cnblogs.com/grandyang/p/4887169.html)







11.5 Given a sorted array of strings which is interspersed with empty strings, write a method to find the location of a given string.
 EXAMPLE
 Input: find "ball" in {"at", "", "", "", "ball", "", "", "car", "", "", "dad", "", ""}
 Output: 4



这道题给了我们一个有序的字符串数组，但是在中间加入了很多空字符串，让我们来查找一个给定字符串。如果没有这些空字符串，那么我们用二分查找法很容易搜索，但是这些空字符串就有很大的干扰作用。那么我们要在原有的二分查找法上做修改，类似的修改二分查找法的里有[Search in Rotated Sorted Array 在旋转有序数组中搜索](http://www.cnblogs.com/grandyang/p/4325648.html)和[Search in Rotated Sorted Array II 在旋转有序数组中搜索之二](http://www.cnblogs.com/grandyang/p/4325840.html)。这道题我们的思路是，查找中间的字符串，如果是空字符串，那么我们用二分查找法来找周围最近的非空字符串，然后把mid移到飞空字符串的位置，继续二分查找。相当于二分查找中又嵌套了一个二分查找，参见代码如下：



```
class Solution {
public:
    int search(vector<string> strings, string str) {
        int first = 0, last = strings.size() - 1;
        while (first <= last) {
            int mid = first + (last - first) / 2;
            if (strings[mid].empty()) {
                int left = mid - 1, right = mid + 1;
                while (true) {
                    if (left < first && right > last) return -1;
                    else if (right <= last && !strings[right].empty()) {
                        mid = right; break;
                    }
                    else if (left >= first && !strings[left].empty()) {
                        mid = left; break;
                    }
                    ++right;
                    --left;
                }
            }
            int res = strings[mid].compare(str);
            if (res == 0) return mid;
            else if (res < 0) first = mid + 1;
            else last = mid - 1;
        }    
        return -1;
    }
};
```














