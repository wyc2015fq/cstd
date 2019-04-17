# Java程序排序之交换排序 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2011年09月01日 15:01:16[boonya](https://me.csdn.net/boonya)阅读数：1192标签：[java																[string																[c#](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[Java](https://blog.csdn.net/boonya/article/category/878447)





1、定义交换排序：根据序列中两个记录键值的比较结果来对换这两个记录在序列中的位置，交换排序的特点是：将键值较大的记录向序列的尾部移动，键值较小的记录向序列的前部移动。

2、 Java实现：

public static void main(String[] args) {

  Scanner sc=new Scanner(System.in);

  int[]score=new int [5];

        int i, j;

        int temp;

        System.out.println("请输入5个学员成绩：");

        for (i = 0; i < 5;i++ )

        {

         System.out.println("第"+(i+1)+"个学员的成绩：");

            score[i] =sc.nextInt();

        }

        for (i = 0; i < score.length- 1;i++ )

        {

            for (j = 0; j < score.length - i - 1;j++ ) 

            {

                if (score[j]>score[j+1])

                {

                    temp = score[j];

                    score[j] = score[j + 1];

                    score[j+1]=temp;

                }

            }

        }

        System.out.println("排序后的顺序输出成绩为：");

        for (i=0;i<score.length;i++)

        {

         System.out.print(score[i]+"\t");

        }


 }



3、 C#实现：

 static void Main(string[] args)

        {

            int[]score=new int [5];

            int i, j;

            int temp;

            Console.WriteLine("请输入5个学员成绩：");

            for (i = 0; i < 5;i++ )

            {

                Console.WriteLine("第{0}个学员的成绩：",i+1);

                score[i] = int.Parse(Console.ReadLine());

            }

            for (i = 0; i < score.Length - 1;i++ )

            {

                for (j = 0; j < score.Length - i - 1;j++ ) 

                {

                    if (score[j]>score[j+1])

                    {

                        temp = score[j];

                        score[j] = score[j + 1];

                        score[j+1]=temp;

                    }

                }

            }

            Console.WriteLine("排序后的顺序输出成绩为：");

            for (i=0;i<score.Length;i++)

            {

                Console.Write("{0}\t",score[i]);

            }

            Console.ReadLine();

        }

    }](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




