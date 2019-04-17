# 【剑指offer】题24：二叉搜索树后序遍历 - xiaxzhou的博客 - CSDN博客





2017年07月02日 14:51:22[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：95
个人分类：[剑指offer](https://blog.csdn.net/xiaxzhou/article/category/6949916)









```cpp
bool VerifySquenceOfBST_core(vector<int> sequence) {
        int n = sequence.size();
        if (n < 3)
        {
            return true;
        }
        int root = sequence[n-1];
        int mid(0);
        while (sequence[mid]<root)
        {
            mid++;
        }
        for (auto i = mid; i < n - 1;++i)
        {
            if (sequence[i]<root)
            {
                return false;
            }
        }
        vector<int> left(sequence.begin(), sequence.begin() + mid);
        vector<int> right(sequence.begin()+mid, sequence.end()-1);
        return VerifySquenceOfBST_core(left) && VerifySquenceOfBST_core(right);
    }
```



