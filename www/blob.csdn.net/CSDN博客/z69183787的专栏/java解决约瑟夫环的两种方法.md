# java解决约瑟夫环的两种方法 - z69183787的专栏 - CSDN博客
2018年03月30日 18:57:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1028
[https://blog.csdn.net/shuaicihai/article/details/54847433](https://blog.csdn.net/shuaicihai/article/details/54847433)
选择不同的数据结构，对于解决问题使用的算法也会存在很大的不同，举例分别使用组数和队列来解决约瑟夫环问题。
### 使用数组数据结构
```
/**
 * 数组解决约瑟夫环问题
 * @param personNumber 人数
 * @param number       数的数
 */
public static void count(int personNumber,int number){
    //1.把人放到数组中,数据结构
    int[] persons = new int[personNumber];
    for (int i = 0; i < personNumber; i++) {
        persons[i] = i+1;
    }
    //2.算法
    int index = 0;//从0开始
    int dunNumbers = 0;//记录蹲下人数的数
    int duns = 0;//记录蹲下人的个数
    while (duns !=personNumber ) {
        //有人没有蹲下
        //1.判读当前位置是否是蹲下
        if (persons[index%persons.length] == 0) {
            //蹲下,继续数数
            dunNumbers++;
            index++;
        }else {
            //2.没有蹲下,判读数的数是否是number的数
            if ((index + 1 -dunNumbers)%number == 0) {
                //是
                //打印该位置
                System.out.println(persons[index%persons.length]);
                //用0标识蹲下
                persons[index%persons.length] = 0;
                //蹲下的人数++
                duns++;
                //继续数数
                index++;
            }else {
                //否
                //继续数数
                index++;
            }
        }
    }
}
```
### 使用队列数据结构
```
/**
 * 队列解决约瑟夫环问题
 * @param personNumber 人数
 * @param number          数的数
 */
public static void countQueue(int personNumber,int number){
    //1.把人放到队列中
    Queue<Integer> persons =  new LinkedList<Integer>();
    for (int i = 0; i < personNumber; i++) {
        persons.add(i+1);
    }
    //2.算法
    int counts = 0;//计数器
    while (!persons.isEmpty()) {
        //1.出队列
        Integer person = persons.poll();
        //2.计数器++
        counts++;
        //3.判断
        if (counts % number == 0) {
            //是,打印
            System.out.println(person);
        } else {
            //不是,继续入队列
            persons.add(person);
        }
    }
}
```
