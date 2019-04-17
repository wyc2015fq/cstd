# 【剑指offer】题36：数组中的逆序对 - xiaxzhou的博客 - CSDN博客





2017年07月04日 17:46:23[xiaxzhou](https://me.csdn.net/xiaxzhou)阅读数：128








```cpp
int InversePairs_Core(vector<int>& data, vector<int>& copy, int left, int right)
{
    if (left>=right)
        return 0;

    int mid = left + ((right - left) >> 1);
    int leftNum = InversePairs_Core(copy, data, left, mid);
    int rightNum = InversePairs_Core(copy, data, mid + 1, right);
    int count(0), i(right), left_index(mid), right_index(right);
    while (left_index>=left&&right_index>=mid+1)
    {
        if (data[left_index]>data[right_index])
        {
            count += right_index-mid;
            if (count>1000000007)
            {
                count %= 1000000007;
            }
            copy[i--] = data[left_index--];
        }
        else
        {
            copy[i--] = data[right_index--];
        }
    }
    while (left_index>=left)
    {
        copy[i--] = data[left_index--];
    }
    while (right_index>=mid+1)
    {
        copy[i--] = data[right_index--];
    }
    return (count + leftNum + rightNum) % 1000000007;
}

int InversePairs(vector<int>& data)
{
    if (data.size() < 2)
        return 0;
    vector<int> copy(data);
    return InversePairs_Core(data, copy, 0, data.size() - 1);
}

int main()
{
    vector<int> vec{ 7,5,6,4 };
    cout<<InversePairs(vec);
#ifdef debug_
#else
#endif
return 0;
}
```



