# 3Sum - 李鑫o_O - CSDN博客

2016年03月08日 09:33:56[hustlx](https://me.csdn.net/HUSTLX)阅读数：215



```cpp
<span style="font-size:18px;">#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
vector<vector<int>> threeSum(vector<int>& nums) {
    sort(nums.begin(), nums.end());
    int len = nums.size();
    vector<vector<int>> res;
    for (int i = 0; i < len; i++) {
        int sum = 0 - nums[i];
        int low = i + 1;
        int high = len - 1;
        while (low < high) {
            if (sum > nums[low] + nums[high]) low++;
            else if (sum < nums[low] + nums[high]) high--;
            else {
                vector<int> temp = { nums[i],nums[low],nums[high] };
                res.push_back(temp);
                while (low < high &&nums[low] == temp[1]) low++;
                while (low < high && nums[high] == temp[2]) high--;
            }
        }
        while ((i + 1 < len) && nums[i] == nums[i + 1]) i++;
    }
    return res;
}

int main() {
    vector<int> nums = {-1,-1,0,1 };
    vector<vector<int>> re=threeSum(nums);
}</span>
```

