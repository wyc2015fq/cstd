# 31. Next Permutation - 李鑫o_O - CSDN博客





2016年03月24日 11:25:00[hustlx](https://me.csdn.net/HUSTLX)阅读数：349








```cpp
void nextPermutation(vector<int>& nums) {
    int n = nums.size();
    if (n <= 1) return;
    int min = nums[n - 1];
    for (int i = n - 1; i >= 0; i--) {
        if (i-1>=0&&nums[i-1] < nums[i]) {
            int j = n-1;
            while (j) {
                if (nums[j] > nums[i - 1]) {
                    min = nums[j];
                    nums.erase(nums.begin()+j);
                    break;
                }
                j--;
            }
            nums.insert(nums.begin() + i - 1, min);
            sort(nums.begin() + i, nums.end());
            return;
        }        
    }
    sort(nums.begin(), nums.end());
}
```




