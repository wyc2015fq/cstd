# KMP - zkq_1986的博客 - CSDN博客





2016年11月11日 14:49:44[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：162
个人分类：[Algorithm](https://blog.csdn.net/zkq_1986/article/category/6367243)









public class KMP 

{

```
public static void main(String[] args)
{
    // TODO Auto-generated method stub
    run("acdccdcddef".toCharArray(), "cdcdde".toCharArray());
}

static void run(char[] s, char[] p)
{
    int[] next = getNext(p);//获取next数组，保存到vector中

    System.out.println("next: ");
    for(int i = 0; i < next.length; i++)
    {
        System.out.print(next[i] + " ");
    }
    System.out.println();

    int i = 0, j = 0;
    while(i != s.length && j != p.length)
    {
        if (j == -1 || s[i] == p[j])
        {
            ++i;
            ++j;
        }
        else
        {
            j = next[j];
        }
    }

    System.out.println("index: " + (j == p.length ? i - j: -1));
}

static int[] getNext(char[] p)
{

    int[] next = new int[p.length];
    next[0] = -1;

    int i = 0, j = -1;

    while (i != p.length - 1)
    {
        //这里注意，i==0的时候实际上求的是nextVector[1]的值，以此类推
        if (j == -1 || p[i] == p[j])
        {
            ++i;
            ++j;
            //update
            //next[i] = j;
            //注意这里是++i和++j之后的p[i]、p[j]
            next[i] = p[i] != p[j] ? j : next[j];
        }
        else
        {
            j = next[j];
        }
    }

    return next;
}
```

}

//输出 

next:  

-1 0 -1 0 2 0  

index: 4



