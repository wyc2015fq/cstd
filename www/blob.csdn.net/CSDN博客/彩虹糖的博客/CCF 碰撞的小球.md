# CCF 碰撞的小球 - 彩虹糖的博客 - CSDN博客





2018年09月08日 17:19:09[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：106








问题描述
|试题编号：|201803-2|
|----|----|
|试题名称：|碰撞的小球|
|时间限制：|1.0s|
|内存限制：|256.0MB|
|问题描述：|问题描述　　数轴上有一条长度为L（L为偶数)的线段，左端点在原点，右端点在坐标L处。有n个不计体积的小球在线段上，开始时所有的小球都处在偶数坐标上，速度方向向右，速度大小为1单位长度每秒。			　　当小球到达线段的端点（左端点或右端点）的时候，会立即向相反的方向移动，速度大小仍然为原来大小。			　　当两个小球撞到一起的时候，两个小球会分别向与自己原来移动的方向相反的方向，以原来的速度大小继续移动。			　　现在，告诉你线段的长度L，小球数量n，以及n个小球的初始位置，请你计算t秒之后，各个小球的位置。提示　　因为所有小球的初始位置都为偶数，而且线段的长度为偶数，可以证明，不会有三个小球同时相撞，小球到达线段端点以及小球之间的碰撞时刻均为整数。			　　同时也可以证明两个小球发生碰撞的位置一定是整数（但不一定是偶数）。输入格式　　输入的第一行包含三个整数n, L, t，用空格分隔，分别表示小球的个数、线段长度和你需要计算t秒之后小球的位置。			　　第二行包含n个整数a1, a2, …, an，用空格分隔，表示初始时刻n个小球的位置。输出格式　　输出一行包含n个整数，用空格分隔，第i个整数代表初始时刻位于ai的小球，在t秒之后的位置。样例输入3 10 5			4 6 8样例输出7 9 9样例说明　　初始时，三个小球的位置分别为4, 6, 8。![](http://118.190.20.162/RequireFile.do?fid=b6beJN6e)			　　一秒后，三个小球的位置分别为5, 7, 9。![](http://118.190.20.162/RequireFile.do?fid=Ab8QmfeR)			　　两秒后，第三个小球碰到墙壁，速度反向，三个小球位置分别为6, 8, 10。![](http://118.190.20.162/RequireFile.do?fid=fgQLYbNn)			　　三秒后，第二个小球与第三个小球在位置9发生碰撞，速度反向（注意碰撞位置不一定为偶数），三个小球位置分别为7, 9, 9。![](http://118.190.20.162/RequireFile.do?fid=erfyNJDT)			　　四秒后，第一个小球与第二个小球在位置8发生碰撞，速度反向，第三个小球碰到墙壁，速度反向，三个小球位置分别为8, 8, 10。![](http://118.190.20.162/RequireFile.do?fid=m5EBf6q8)			　　五秒后，三个小球的位置分别为7, 9, 9。![](http://118.190.20.162/RequireFile.do?fid=JRHaHt4T)样例输入10 22 30			14 12 16 6 10 2 8 20 18 4样例输出6 6 8 2 4 0 4 12 10 2数据规模和约定　　对于所有评测用例，1 ≤ n ≤ 100，1 ≤ t ≤ 100，2 ≤ L ≤ 1000，0 < ai < L。L为偶数。			　　保证所有小球的初始位置互不相同且均为偶数。|



答案：

```java
import java.io.*;
import java.util.ArrayList;
import java.util.Scanner;
public class Main{
class Circle{
        int position;
        int id;
        int direction;  //-1 左，1 右
        boolean isMove;
        public Circle(int position, int id, int direction) {
            this.position = position;
            this.id = id;
            this.direction = direction;
            this.isMove=false;
        }
        public void anoherMove(Circle circle, int direction){

        }
        public void move(ArrayList<Circle> circles, int length){
            if(this.isMove==true)
                return;
            if(this.position==0){
                this.direction=1;
                this.position++;
                this.isMove=true;
            }
            else if(this.position==length){
                this.direction=-1;
                this.position--;
                this.isMove=true;
            }
            else{
                for(Circle circle:circles){
                    if(circle.position==this.position&&circle.direction==-this.direction&&circle.isMove==false){
                        this.direction=-this.direction;
                        this.position+=this.direction;
                        this.isMove=true;
                        circle.direction=-circle.direction;
                        circle.position+=circle.direction;
                        circle.isMove=true;
                        return;
                    }
                }
                this.position+=this.direction;
                this.isMove=true;
            }
        }
    }

    public void run(){
        Scanner in = new Scanner(System.in);
        int counter = 0;
        int n = in.nextInt();
        int l = in.nextInt();
        int t = in.nextInt();
        ArrayList<Integer> integers = new ArrayList<>();
        ArrayList<Circle> circles = new ArrayList<>();
        for(int i=0;i<n;i++){
            integers.add(in.nextInt());
        }
        for(Integer integer:integers){
            circles.add(new Circle(integer,counter,1));
        }
        for(int i=0;i<t;i++){
            for(Circle circle:circles){
                circle.move(circles,l);
            }
            for(Circle circle:circles){
                circle.isMove=false;
            }
        }
        for(Circle circle:circles){
            System.out.print(circle.position+" ");
        }
    }

    public static void main(String[] args) {
       new Main().run();
    }
}
```





