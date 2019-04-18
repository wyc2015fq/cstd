# nsum 问题集锦_leetcode刷题总结 - zhuzhuxia183的博客 - CSDN博客





2019年01月13日 18:08:38[喵小姐的邻居小小朱](https://me.csdn.net/zhuzhuxia183)阅读数：44
所属专栏：[数据结构与算法(leetcode)](https://blog.csdn.net/column/details/33915.html)









## 对于刷到的nSum问题做一个复习
- 1、两数之和  [https://leetcode-cn.com/problems/two-sum/](https://leetcode-cn.com/problems/two-sum/)

给定一个整数数组 nums 和一个目标值 target，请你在该数组中找出和为目标值的那 两个 整数，并返回他们的数组下标。

你可以假设每种输入只会对应一个答案。但是，你不能重复利用这个数组中同样的元素。

示例:

给定 nums = [2, 7, 11, 15], target = 9

因为 nums[0] + nums[1] = 2 + 7 = 9

所以返回 [0, 1]

对于这种没有重复的数，在空间充足的情况下，我们利用一次遍历+hash的方法可以十分高效的解决次问题：

```
public int[] twoSum(int[] nums, int target) {
	HashMap<Integer,Integer> map = new HashMap<>();
        int[] res = new int[2];
        for (int i = 0; i < nums.length; i++) {
            if (map.containsKey(target-nums[i])){
                res[0] = i;
                res[1] = map.get(target-nums[i]);
                return res;
            }
            map.put(nums[i],i);
        }
        return res;
}
```

emmmm,我jio得还可以
- 2、两数之和 II - 输入有序数组 [https://leetcode-cn.com/problems/two-sum-ii-input-array-is-sorted/](https://leetcode-cn.com/problems/two-sum-ii-input-array-is-sorted/)

给定一个已按照升序排列 的有序数组，找到两个数使得它们相加之和等于目标数。

函数应该返回这两个下标值 index1 和 index2，其中 index1 必须小于 index2。

说明:

返回的下标值（index1 和 index2）不是从零开始的。

你可以假设每个输入只对应唯一的答案，而且你不可以重复使用相同的元素。

示例:
输入: numbers = [2, 7, 11, 15], target = 9

输出: [1,2]

解释: 2 与 7 之和等于目标数 9 。因此 index1 = 1, index2 = 2 。
这个题目，其实抓住输入的数组是有序的话，我们可以以o(n)的一个时间之内解决这个问题，

```
public int[] twoSum(int[] numbers, int target) {
	int left = 0,right = target;//emmm,这里不是二分法噢，哈哈哈，只是两个指针
	int[] res= {-1,-1};
	while(left<right){
		if(nums[left]+nums[right]==target){
			res[0] = left+1;
			res[1] = right+1;
			return res;
		}else if(nums[left]+nums[right]<target){
			left++;
		}else{
			right--;
		}
	}
	return res;
}
```
- 3、 三数之和 [https://leetcode-cn.com/problems/3sum/](https://leetcode-cn.com/problems/3sum/)

给定一个包含 n 个整数的数组 nums，判断 nums 中是否存在三个元素 a，b，c ，使得 a + b + c = 0 ？找出所有满足条件且不重复的三元组。

注意：答案中不可以包含重复的三元组。

例如, 给定数组 nums = [-1, 0, 1, 2, -1, -4]，

满足要求的三元组集合为：

[

[-1, 0, 1],

[-1, -1, 2]

]
```
public List<List<Integer>> threeSum(int[] nums) {
        Arrays.sort(nums);
        List<List<Integer>> res = new ArrayList<>();
        int length = nums.length;
        for (int i = 0; i < length-2;) {
            int j = i+1;
            int k = length-1;
            while(j<k){
                if (nums[i]+nums[j]+nums[k]==0){
                    List<Integer> item =  new ArrayList<>();
                    item.add(nums[i]);
                    item.add(nums[j]);
                    item.add(nums[k]);
                    res.add(item);
                    j++;
                    k--;
                    while (nums[j]==nums[j-1]&&j<k){
                        j++;
                    }
                    while (nums[k]==nums[k+1]&&j<k){
                        k--;
                    }
                }else if (nums[i]+nums[j]+nums[k]<0){
                    j++;
                    while (nums[j]==nums[j-1]&&j<k){
                        j++;
                    }
                }else{
                    k--;
                    while (nums[k]==nums[k+1]&&j<k){
                        k--;
                    }
                }
            }
            i++;
            while (nums[i]==nums[i-1] && i<length-2){
                i++;
            }
        }
        return res;
    }
```

这里的思路其实和上面一题还是有相通的地方的，首先进行排序，我们的思路是从前往后确定第一个数，遍历从0-length-2,然后i,分别从 i+1~lenth-1之间进行遍历，根绝nums[i]+nums[j]+nums[k]与0的关系来调整i,j，注意的事情是，调整i,j之后，需要判断是不是重复的数。这个操作比较重要。
- 4、最接近的三数之和  [https://leetcode-cn.com/problems/3sum-closest/](https://leetcode-cn.com/problems/3sum-closest/)

给定一个包括 n 个整数的数组 nums 和 一个目标值 target。找出 nums 中的三个整数，使得它们的和与 target 最接近。返回这三个数的和。假定每组输入只存在唯一答案。

例如，给定数组 nums = [-1，2，1，-4], 和 target = 1.

与 target 最接近的三个数的和为 2. (-1 + 2 + 1 = 2).

```
Arrays.sort(nums);
        int length =nums.length;
        int min_distance = Integer.MAX_VALUE;
        int res = 0;
        for (int i = 0; i < length-2;) {
            int j = i+1;
            int k = length-1;
            while (j<k){
                int diff = nums[i]+nums[j]+nums[k]-target;
                if (diff==0)
                    return target;
                else if (diff>0){
                    res = min_distance<diff?res:target+diff;
                    min_distance = Math.min(diff,min_distance);
                    k--;
                    while (j<k&& nums[k]==nums[k+1]){
                        k--;
                    }
                }else {
                    res = min_distance<Math.abs(diff)?res:target+diff;
                    min_distance = Math.min(-diff,min_distance);
                    j++;
                    while (j<k&& nums[j]==nums[j-1]){
                        j++;
                    }
                }
            }
            i++;
            while (i<length-2&&nums[i]==nums[i-1]){
                i++;
            }
        }
        return res;
```

这个题目，在看完上面一个题目之后，简直太easy了，都是建立在第二题的思路上来的，先排序，定第一个，剩下两个就是直接用两边往中间遍历即可。
- 四数之和 [https://leetcode-cn.com/problems/4sum/](https://leetcode-cn.com/problems/4sum/)

给定一个包含 n 个整数的数组 nums 和一个目标值 target，判断 nums 中是否存在四个元素 a，b，c 和 d ，使得 a + b + c + d 的值与 target 相等？找出所有满足条件且不重复的四元组。

注意：

答案中不可以包含重复的四元组。

示例：

给定数组 nums = [1, 0, -1, 0, -2, 2]，和 target = 0。

满足要求的四元组集合为：

[

[-1,  0, 0, 1],

[-2, -1, 1, 2],

[-2,  0, 0, 2]

]

这个题目，可以说是极度无聊了，在三数之和上面居然来了一个四数之和，思路还是十分的明显惹，相信大家可以十分轻松的写出以下的代码：
```
List<List<Integer>> result = new ArrayList<>();
        Arrays.sort(nums);
        
        int i,j,k,l;
        for(i=0;i<nums.length-3;){
            for(j=i+1;j<nums.length-2;){
                k = j+1;
                l = nums.length-1;
                while(k<l){
                    if(nums[i]+nums[j]+nums[k]+nums[l]==target){
                        ArrayList<Integer> item = new ArrayList<>();
                        item.add(nums[i]);
                        item.add(nums[j]);
                        item.add(nums[k]);
                        item.add(nums[l]);
                        result.add(item); 
                        k++;
                        l--;
                        while(nums[k]==nums[k-1]&& k<l){
                            k++;
                        }
                        
                        while(nums[l]==nums[l+1]&& k<l){
                            l--;
                        }
                    }else if(nums[i]+nums[j]+nums[k]+nums[l]<target){
                        k++;
                        while(nums[k]==nums[k-1]&& k<l){
                            k++;
                        }
                    }else if(nums[i]+nums[j]+nums[k]+nums[l]>target){
                        l--;
                        while(nums[l]==nums[l+1]&& k<l){
                            l--;
                        }
                    }
                }
                j++;
                while(nums[j]==nums[j-1]&& j<nums.length-2){
                    j++;
                }     
            }
            i++;
            while(nums[i]==nums[i-1]&& i<nums.length-3){
                    i++;
            }  
        }
        return result;
```

比较长，所以得注意逻辑顺序，不然很容易错的。（写文章的时候重写一遍，由于某一个地方把j写成i，导致写错惹，很难受）

这次先到这里，拜拜各位～







