# two sum - 李鑫o_O - CSDN博客





2016年03月08日 09:34:56[hustlx](https://me.csdn.net/HUSTLX)阅读数：265








```cpp
<span style="font-size:18px;">#include <string>
#include <stack>
#include <vector>
#include <map>
using namespace std;
vector<int> twoSum(vector<int>& nums, int target) {
    int len = nums.size();
    map<int,int> temp;
    vector<int> re;
    for (int i = 0; i < len; i++) {
        auto it = temp.find(target - nums[i]);
        if (it!= temp.end())
        {
            re.push_back(it->second);
            re.push_back(i);
            return re;
        }
        else temp[nums[i]] = i;
    }
}
int main() {
    vector<int> nums = { 2, 7, 11, 15 };
    int target = 9;
    vector<int> re = twoSum(nums, target);
}</span>
```





