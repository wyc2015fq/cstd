# LeetCode Two Sum 两数之和 - xcw0754 - 博客园
# [LeetCode Two Sum 两数之和](https://www.cnblogs.com/xcw0754/p/4396641.html)
题意：在一个整数序列中找到两个元素，他们之和为target，并用vector返回这两个元素的位置（升序），位置从1开始算起。
思路：
方法（1）：两个指针法。也就是排序，然后一个从头扫，一个从尾扫，前提是先排序，但是给的数组是无序的，一旦排序就失去了他们的具体位置。如果是ACM的题还可以弄个结构体把他们打包起来，再根据值大小来排序，这样找到了也就能找到他们的位置。可是这里是在一个类里面的，用不上结构体，肯定还有其他方法 ，比如 pair，但是我还不会实现。于是，用multimap来代替，将元素值作为key，位置作为元素，因为multimap本来就自动已经排序了，那么就可以直接用两个指针来扫了。（刚开始不知道是不是有重复的元素，所以用multi，其实map也是可以的）。复杂度较大。两个指针扫是线性的，排序至少是nlogn，但map的还不是很了解，按nlogn算吧。那就是O(nlog)了。
```
1 class Solution {
 2 public:
 3     vector<int> twoSum( vector<int> &numbers, int target ) {
 4             multimap<int,int> hand;
 5             vector<int> ans;
 6             for(int i=0; i<numbers.size(); i++)    //装进multimap
 7                 hand.insert( std::pair<int,int>(numbers[i], i+1) );
 8             multimap<int,int>::iterator it1=hand.begin(), it2=hand.end();
 9             it2--;
10             while( it1!=it2 )    //两个指针
11             {
12                 if( it1->first + it2->first == target )
13                 {
14                     ans.push_back(it1->second);
15                     ans.push_back(it2->second);
16                     if(ans[0]>ans[1])    //为了升序
17                         reverse(ans.begin(), ans.end());
18                     break;
19                 }
20                 else if( it1->first + it2->first > target )
21                     it2--;
22                 else    it1++;
23             }
24             return ans;
25     }
26 };
twoSum
```
方法（2）：哈希法。只是求两个值之和，那么知道其中一个值，另一个如果存在于序列中，那么就找到了。这里用unordered_map实现，将所有元素装进该unordered_map中，元素值作为key，位置作为元素。接着，对每个序列中的元素，用map的find看是否有另一伴的存在，若不存在就掠过，找到还得小心个坑，也就是找到的另一伴却是自己，比如有元素{ 2,4,5,6 }，target=8，那么2+6=8是肯定的了，但是当你在找4的另一伴时，unordered_map中找到的4也能组成8，但是这是不允许的。
```
1 class Solution {
 2 public:
 3     vector<int> twoSum( vector<int> &numbers, int target ) {
 4             unordered_map<int,int> hand;
 5             vector<int> ans;
 6             for(int i=0; i<numbers.size(); i++)    //装进map
 7                 hand.insert( std::pair<int,int>(numbers[i], i+1) );
 8             unordered_map<int,int>::iterator it;
 9             for(int i=0; i<numbers.size(); i++ )
10             {
11                 it = hand.find( target-numbers[i] );
12             if( it!=hand.end() )
13             {
14                 if( it->second == i+1 )
15                     continue;
16                 else
17                 {
18                     ans.push_back( i+1 );
19                         ans.push_back( it->second );
20                         if(ans[0]>ans[1])//为了升序
21                             reverse(ans.begin(), ans.end());
22                         break;
23                 }
24             }
25             }
26             return ans;
27     }
28 };
twoSum
```

