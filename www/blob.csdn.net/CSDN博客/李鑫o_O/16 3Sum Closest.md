# 16. 3Sum Closest - 李鑫o_O - CSDN博客





2016年03月07日 15:05:48[hustlx](https://me.csdn.net/HUSTLX)阅读数：378








```cpp
#include <string>
#include <stack>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

int threeSumClosest(vector<int>& nums, int target) {
    sort(nums.begin(), nums.end());
    int len = nums.size();
    int min = INT_MAX;
    int res;
    for (int i = 0; i < len; i++) {
        int low = i + 1;
        int high = len - 1;
        while (low < high) {
            int temp = target - nums[low] - nums[high]-nums[i];
            if (abs(temp) < min) {
                min = abs(temp);
                res = nums[low] + nums[high] + nums[i];
                //while (low < high && nums[low+1] == nums[low]) low++;
                //while (low < high && nums[high-1] == nums[high]) high--;
            }
            if (temp > 0) low++;
            else if (temp < 0) high--;
            else return target;      
        }
        while ((i + 1 < len) && nums[i] == nums[i + 1]) i++;
    }
    return res;
}
int main() {
    vector<int> nums = {13,2,0,-14,-20,19,8,-5,-13,-3,20,15,20,5,13,14,-17,-7,12,-6,0,20,-19,-1,-15,-2,8,-2,-9,13,0,-3,-18,-9,-9,-19,17,-14,-19,-4,-16,2,0,9,5,-7,-4,20,18,9,0,12,-1,10,-17,-11,16,-13,-14,-3,0,2,-18,2,8,20,-15,3,-13,-12,-2,-19,11,11,-10,1,1,-10,-2,12,0,17,-19,-7,8,-19,-17,5,-5,-10,8,0,-12,4,19,2,0,12,14,-9,15,7,0,-16,-5,16,-12,0,2,-16,14,18,12,13,5,0,5,6 };
    int target = -59;
    int re = threeSumClosest(nums, target);
}
```




