# 剑指Offer - 数组中重复的数字 - curryche的博客 - CSDN博客





2018年07月31日 11:54:10[curryche](https://me.csdn.net/whwan11)阅读数：23标签：[剑指offer																[笔试](https://so.csdn.net/so/search/s.do?q=笔试&t=blog)](https://so.csdn.net/so/search/s.do?q=剑指offer&t=blog)
个人分类：[笔试](https://blog.csdn.net/whwan11/article/category/7789918)








## 题目

## 解法

### 解法1

代码

```
class Solution {
public:
    // Parameters:
    //        numbers:     an array of integers
    //        length:      the length of array numbers
    //        duplication: (Output) the duplicated number in the array number
    // Return value:       true if the input is valid, and there are some duplications in the array number
    //                     otherwise false
    bool duplicate(int numbers[], int length, int* duplication) {
        int i,j,temp;
        i=1;

        while(i<length)
        {
            j=i-1;
            while(j>=0)
            {
                if(numbers[j]==numbers[j+1])
                {
                    *duplication=numbers[j+1];
                    return true;
                    //dup_i++;
                    //break;
                }
                else if(numbers[j]>numbers[j+1])
                {
                    temp=numbers[j+1];
                    numbers[j+1]=numbers[j];
                    numbers[j]=temp;
                    j--;
                }
                else
                {                   
                    break;
                }
            }
            i++;          
        }        
        return false;      
    }
};
```

### 解法2

代码

```
class Solution {
public:
    // Parameters:
    //        numbers:     an array of integers
    //        length:      the length of array numbers
    //        duplication: (Output) the duplicated number in the array number
    // Return value:       true if the input is valid, and there are some duplications in the array number
    //                     otherwise false
    bool duplicate(int numbers[], int length, int* duplication) {
        int i,j,temp;

        for(i=0;i<length;i++)
        {
            while(numbers[i]!=i)
            {
                if(numbers[i]==numbers[numbers[i]])
                {
                    *duplication=numbers[i];
                    return true;
                }
                temp=numbers[numbers[i]];
                numbers[numbers[i]]=numbers[i];
                numbers[i]=temp;
            }
        }
        return false;      
    }
};
```




