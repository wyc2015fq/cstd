# next_permutation()/prev_permutaton()的实现（Java版） - westbrook1998的博客 - CSDN博客





2018年10月25日 20:33:16[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：92








Java里也没有这个东西…

```java
public class Main{
    /**
     * 下一个排列 stl中的next_permutation()
     * @param nums
     * @return
     */
    static boolean nextPermutation(int[] nums){
        //将数组分为三部分，第二部分是一个元素，第三部分是递减子序列
        int len=nums.length;
        for(int i=len-1;i>0;i--){
            //找到第二第三部分的分界线
            if(nums[i]>nums[i-1]){
                //v即为第二部分元素
                int v=nums[i-1];
                int j=len-1;
                //在第三部分元素找到最小的大于第二部分元素的元素
                for(;j>=i;j--){
                    if(nums[j]>v){
                        break;
                    }
                }
                //进行交换
                swap(nums,i-1,j);
                int l=i;
                int r=len-1;
                //将第三部分元素逆序
                while(l<r){
                    swap(nums,l,r);
                    l++;
                    r--;
                }
                //得到新的全排列
                return true;
            }
        }
        //整个数组已经递减，没有下一个排列
        return false;
    }
    /**
     * 上一个排列 stl中的prev_permutation()
     * @param nums
     * @return
     */
    static boolean prevPermutation(int[] nums){
        int len=nums.length;
        for(int i=len-1;i>0;i--){
            if(nums[i]<nums[i-1]){
                int v=nums[i-1];
                int j=len-1;
                for(;j>=i;j--){
                    if(nums[j]<v){
                        break;
                    }
                }
                swap(nums,i-1,j);
                int l=i;
                int r=len-1;
                while(l<r){
                    swap(nums,l,r);
                    l++;
                    r--;
                }
                return true;
            }
        }
        return false;
    }

    static void swap(int[] nums,int i,int j){
        int t=nums[i];
        nums[i]=nums[j];
        nums[j]=t;
    }
    static void printArr(int[] nums){
        int l=nums.length;
        for(int i=0;i<l;i++){
            System.out.print(nums[i]+" ");
        }
        System.out.println();
    }
    public static void main(String[] args) {
        int[] a=new int[]{1,2,3,4};
        do{
            printArr(a);
        }while (nextPermutation(a));
        System.out.println("-----------");
        do{
            printArr(a);
        }while (prevPermutation(a));
    }
}
```



