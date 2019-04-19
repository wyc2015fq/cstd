# C++算法之遗传算法及实现 - fanyun的博客 - CSDN博客
2018年07月08日 22:53:55[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：410
# 1、遗传算法基本思想
核心是达尔文优胜劣汰适者生存的进化理论的思想。一个种群，通过长时间的繁衍，种群的基因会向着更适应环境的趋势进化，适应性强的个体基因被保留，后代越来越多，适应能力低个体的基因被淘汰，后代越来越少。经过几代的繁衍进化，留下来的少数个体，就是相对能力最强的个体了。
# 2、遗传算法简单应用
首先，我们先看看一个经典组合问题：“背包问题”。
“背包问题（Knapsack problem）是一种组合优化的NP完全问题。问题可以描述为：给定一组物品，每种物品都有自己的重量和价格，在限定的总重量内，我们如何选择，才能使得物品的总价格最高。问题的名称来源于如何选择最合适的物品放置于给定背包中。”
这个问题的衍生简化问题“0-1背包问题” 增加了限制条件：每件物品只有一件，可以选择放或者不放，更适合我们来举例 
这样的问题如果数量少，当然最好选择穷举法 
比如一共3件商品，用0表示不取，1表示取，那么就一共有
```
000 001 010 
011 100 101
110 111
```
- 1
- 2
- 3
这样8种方案，然后让计算机去累加和，与重量上限比较，留下来的解里取最大即可。 
但如果商品数有300,3000,甚至3w种呢，计算量太大穷举法可能就不适用了，这时如果遗传算法使用得当，就能在较短的时间内帮我们找到近似的最优解，我们继续往下看： 
新的问题是12件商品的0-1背包问题 
我们先让计算机随机产生1000个12位的二进制数。把总重量超过背包上限的解筛掉，剩下的两两一对随机交换“基因片段”产生下一代 
交换前：
```
0000 1100 1101
0011 0101 0101
```
- 1
- 2
交换后：
```
0000 0101 1101
0011 1100 0101
```
- 1
- 2
再筛选，再交配，如此反复几代，留下的“基因型“差不多就是最好的了，如此这般与生物进化规律是一样的。 
同时，在生物繁殖过程中，新产生的基因是有一定几率突变的，这是很多优良性状的重要来源，遗传算法中可也不能忽略它 
比如： 
变异前：
`000101100101`- 1
变异后：
`000101110101`- 1
产生突变的位置，就是一个概率问题。在设计算法的时候，会给每个基因位设置一个突变概率（当然是非常了）同样的在基因交换阶段交换哪些基因呢，也是一个算法设置问题。
# 3、遗传算法关键内容
一个基本函数：适度函数f(x) 
三个基本操作：选择，交叉，变异
- 
适度函数 
适度函数其实就是指解的筛选标准，比如上文所说的把所有超过上限重量的解筛选掉，但是不是有更好的筛选标准呢？这将直接影响最后结果的接近程度以及求解所耗费的时间，所以设置一个好的适度函数很重要。 
通常将将该函数设计成单调递增的函数，如果实际问题中的函数是单调递减的，则应当转化为单调递增的。
- 
选择 
在遗传算法中选择也是个概率问题，在解的范围中适应度更高的基因型有更高的概率被选择到。所以，在选择一些解来产生下一代时，一种常用的选择策略是赌轮法（Roulette Wheel Selection），也就是个体被选中的概率与其适应度函数值成正比。假设群体的个体总数是M，那么那么一个体Xi被选中的概率为f(Xi)/( f(X1) + f(X2) + …….. + f(Xn) )。
- 
交叉 
在均等概率下基因位点的交叉，衍生出新的基因型。上述例子中是通过交换两个基因型的部分基因，来构造两个子代的基因型。 
通常有单点交叉和双点交叉。
- 
变异 
在衍生子代的过程中，新产生的解中的基因型会以一定的概率出错，称为变异。变异发生的概率设置为Pm，记住该概率是很小的一个值。因为变异是小概率事件！
- 
基本遗传算法优化 
为了防止进化过程中产生的最优解被变异和交叉所破坏。《遗传算法原理及应用》介绍的最优保存策略是：即当前种群中适应度最高的个体不参与交叉运算和变异运算，而是用它来替换掉本代群体中经过交叉、变异等遗传操作后所产生的适应度最低的个体。 
当随迭代代数的增加，种群中的优秀个体较多，此时交叉概率当逐渐减小，以免优秀个体的基因结构被破坏，而在迭代初期交叉概率应较大，以扩大搜索范围。同样的道理，变异概率在迭代初期值应较小，而在迭代后期应较大，避免局部最优。
# 4遗传算法的优点：
- 
与问题领域无关且快速随机的全局搜索能力。传统优化算法是从单个初始值迭代求最优解的；容易误入局部最优解。遗传算法从串集开始搜索，复盖面大，利于全局择优。
- 
搜索从群体出发，具有潜在的并行性，可以进行多个个体的同时比较，鲁棒性高！
- 
搜索使用评价函数启发，过程简单。
- 
使用概率机制进行迭代，具有随机性。遗传算法中的选择、交叉和变异都是随机操作，而不是确定的精确规则。这说明遗传算法是采用随机方法进行最优解搜索，选择体现了向最优解迫近，交叉体现了最优解的产生，变异体现了全局最优解的复盖。
- 
具有可扩展性，容易与其他算法结合。遗传算法求解时使用特定问题的信息极少，仅仅使用适应值这一信息进行搜索，并不需要问题导数等与问题直接相关的信息。遗传算法只需适应值和串编码等通用信息，故几乎可处理任何问题，容易形成通用算法程序。
- 
具有极强的容错能力。遗传算法的初始串集本身就带有大量与最优解甚远的信息；通过选择、交叉、变异操作能迅速排除与最优解相差极大的串；这是一个强烈的滤波过程；并且是一个并行滤波机制。故而，遗传算法有很高的容错能力。
遗传算法具有良好的全局搜索能力，可以快速地将解空间中的全体解搜索出，而不会陷入局部最优解的快速下降陷阱；并且利用它的内在并行性，可以方便地进行分布式计算，加快求解速度。
# 5遗传算法的缺点：
- 
遗传算法的编程实现比较复杂,首先需要对问题进行编码,找到最优解之后还需要对问题进行解码
- 
三个算子的实现也有许多参数,如交叉率和变异率,并且这些参数的选择严重影响解的品质,而目前这些参数的选择大部分是依靠经验
- 
没有能够及时利用网络的反馈信息,故算法的搜索速度比较慢，要得要较精确的解需要较多的训练时间
- 
算法对初始种群的选择有一定的依赖性（下图所示），能够结合一些启发算法进行改进
- 
算法的并行机制的潜在能力没有得到充分的利用，这也是当前遗传算法的一个研究热点方向。
同时，遗传算法的局部搜索能力较差，导致单纯的遗传算法比较费时，在进化后期搜索效率较低。在实际应用中，遗传算法容易产生过早收敛的问题。采用何种选择方法既要使优良个体得以保留，又要维持群体的多样性，一直是遗传算法中较难解决的问题。
下面举例来说明遗传算法用以求函数最大值
函数为y = -x2+ 5的最大值，-32<=x<=31
### 一、编码以及初始种群的产生
编码采用二进制编码，初始种群采用矩阵的形式，每一行表示一个染色体，每一个染色体由若干个基因位组成。关于染色体的长度（即基因位的个数）可根据具体情况而定。比如说根据要求极值的函数的情况，本文-32<=X<=31，该范围内的整数有64个，所以可以取染色体长度为6，（26=64）。综上所述，取染色体长度为6，前5个二进制构成该染色体的值（十进制），第6个表示该染色体的适应度值。若是所取得染色体长度越长，表示解空间搜索范围越大，对应的是待搜索的X范围越大。关于如何将二进制转换为十进制，文后的C代码中函数x即为转换函数。 
该初始种群共有4个染色体，第1列表示各个染色体的编号，第2列表示该染色体值的正负号，0表示正，1表示负。第3列到第7列为二进制编码，第8列表示各个染色体的适应度值。第2列到第7列的0-1值都是随机产生的。
### 二、适应度函数
一般情况下，染色体（也叫个体，或一个解）的适应度函数为目标函数的线性组合。本文直接以目标函数作为适应度函数。即每个染色体的适应度值就是它的目标函数值，f(x)=-x^2+ 5。
### 三、选择算子
初始种群产生后，要从种群中选出若干个体进行交叉、变异，那么如何选择这些个体呢？选择方法就叫做选择算子。一般有轮盘赌选择法、锦标赛选择法、排序法等。本文采用轮盘赌选择法来选择。
### 四、交叉算子
那么接下来就要对新种群中选出的两个个体进行交叉操作，一般的交叉方法有单点交叉、两点交叉、多点交叉、均匀交叉、融合交叉。方法不同，效果不同。本文采用最简单的单点交叉。交叉点随机产生。但是交叉操作要在一定的概率下进行，这个概率称为交叉率，一般设置为0.5~0.95之间。通过交叉操作，衍生出子代，以补充被淘汰掉的个体。
### 五、变异
变异就是对染色体的基因进行变异，使其改变原来的结构（适应值也就改变），达到突变进化的目的。变异操作也要遵从一定的概率来进行，一般设置为0.0001~0.1之间，即以小概率进行基因突变。这符合自然规律。本文的变异方法直接采取基因位反转变异法，即0变为1，1变为0。要进行变异的基因位的选取也是随机的。
### 六、终止规则
遗传算法是要一代一代更替的，那么什么时候停止迭代呢？这个规则就叫终止规则。一般常用的终止规则有：若干代后终止，得到的解达到一定目标后终止，计算时间达到一定限度后终止等方法。本文采用迭代数来限制。
### 代码如下所示：
```java
/****************************************/
//功能：使用遗传算法求解y = -x^2 + 5的最大值
//遗传算法细节：
//编码：二进制
//选择：轮转赌轮
//交叉：单点交叉，交叉概率固定
//变异：平均随机变异，变异概率固定
//具体参数可以事先给定
/****************************************/
#include <stdio.h>  
#include <conio.h>  
#include <stdlib.h>  
#include <time.h>  
#include <iostream>  
using namespace std;
/*****初始化一些参数*****/
const int Population_size = 100;        //种群规模
const int Chromosome_length = 6;    //假定有64个网络节点，用64位表示每一个节点
double rate_crossover = 0.5;                //交叉率
double rate_mutation = 0.001;           //变异率
int iteration_num = 50;                     //进化50代                                     
/****************************************/
//将染色体定义为结构体类型
typedef struct Chromosome                          
{   
    short int bit[Chromosome_length];           //染色体二进制码串
    double value;                                           //二进制代码对应的实际值
    double fitness;                                     //适应值  
    double rate_fit;                                        //相对的fit值，即所占的百分比
    double cumu_fit;                                    //积累概率  
}chromosome;
/*****函数声明*****/
//初始化得到个体的二进制字符串
void population_initialize(chromosome (&population_current)[Population_size]);
//对染色体进行解码
void decode(chromosome &population_current) ;   
//计算染色体的适应度值
double objective_function(double x);
//更新种群内个体的属性值
void fresh_property(chromosome(&population_current)[Population_size]);
//基于旋转赌轮的选择操作    proportional roulette wheel selection
void seletc_prw(chromosome(&population_current)[Population_size], chromosome(&population_next_generation)[Population_size], chromosome &best_individual);
//交叉操作
void crossover(chromosome (&population_next_generation)[Population_size]);  
//突变操作
void mutation(chromosome (&population_next_generation)[Population_size]);
/****************************************/
// 主函数
void main()                                   
{
    /*****初始化定义的种群和个体*****/
    clock_t start, end;//开始计时,精确到秒
    start = clock();
    /****************************************/
    /*****初始化定义的种群和个体*****/
    chromosome population_current[Population_size];                    //当前种群  
    chromosome population_next_generation[Population_size];       //产生的下一代的种群                        
    chromosome best_individual;                                                 //记录适应度的最大值
    chromosome zeros_chromosome;                                                //定义一个全为0的个体，用于群体中某个个体的重置
    /****************************************/
    int i = 0,j = 0;//循环变量
    //*****初始化定义的种群和个体*****
    //首先初始化zeros_chromosome，后使用之初始化其他个体
    for (i = 0; i < Chromosome_length; i++)
        zeros_chromosome.bit[i] = 0;
    zeros_chromosome.fitness = 0.0;
    zeros_chromosome.value = 0.0;
    zeros_chromosome.rate_fit = 0.0;
    zeros_chromosome.cumu_fit = 0.0;
    best_individual = zeros_chromosome;
    for (i = 0; i < Population_size; i++)
    {
        population_current[i] = zeros_chromosome;
        population_next_generation[i] = zeros_chromosome;
    }
    /****************************************/
    printf("\nWelcome to the Genetic Algorithm！\n");  //   
    printf("The Algorithm is based on the function y = -x^2 + 5 to find the maximum value of the function.\n");
enter:printf("\nPlease enter the no. of iterations\n请输入您要设定的迭代数 : ");
    // 输入迭代次数，传送给参数 iteration_num
    scanf_s("%d", &iteration_num);                           
    // 判断输入的迭代次数是否为负或零，是的话重新输入
    if (iteration_num <1)
        goto enter;
    //种群初始化，得到个体的二进制字符串
    population_initialize(population_current); 
    //更新种群内个体的属性值
    fresh_property(population_current);
    // 开始迭代
    for (i = 0; i< iteration_num; i++)                            
    {
        // 输出当前迭代次数
        //printf("\ni = %d\n", i); 
        //挑选优秀个体组成新的种群
        seletc_prw(population_current,population_next_generation,best_individual);                 
        //对选择后的种群进行交叉操作
        crossover(population_next_generation);              
        //对交叉后的种群进行变异操作
        mutation(population_next_generation);                      
        //更新种群内个体的属性值
        fresh_property(population_next_generation);
        //将population_next_generation的值赋给population_current，并清除population_next_generation的值
        for (i = 0; i < Population_size; i++)
        {
            population_current[i] = population_next_generation[i];
            population_next_generation[i] = zeros_chromosome;
        }
        //检验时间是否到90s
        end = clock();
        if (double(end - start) / CLK_TCK> 89)
            break;
    } 
    //输出所用时间
    printf("\n 迭代%d次所用时间为： %f\n", iteration_num, double(end - start) / CLK_TCK);
    //输出结果
    printf("\n 函数得到最大值为： %f ,适应度为：%f \n", best_individual.value, best_individual.fitness);
    for (i = 0; i<Population_size; i++)
    {
        printf(" population_current[%d]=", i);
        for (j = 0; j < Chromosome_length; j++)
            printf(" %d", population_current[i].bit[j]);
        printf("        value=%f    fitness = %f\n", population_current[i].value, population_current[i].fitness);
    }
    printf("\nPress any key to end ! ");
    // 清除所有缓冲区
//  flushall();                                   
    system("pause");
}
//函数：种群初始化  
//输入是数组的引用
//调用时，只需输入数组名
void population_initialize(chromosome (&population_current)[Population_size])   
{
    int i = 0, j = 0;
    //产生随机数种子
    srand((unsigned)time(NULL));
    //遍历种群中的每个染色体
    for (j = 0; j<Population_size; j++)                              
    {
        //随机初始化染色体的每一位
        for (i = 0; i<Chromosome_length; i++)                       
        {
            // 随机产生染色体上每一个基因位的值，0或1
            population_current[j].bit[i] = rand()% 2;         
        }
    }
}
// 函数：将二进制换算为十进制 
void decode(chromosome &population_current)   
{//此处的染色体长度为，其中个表示符号位  
    int i = 0;
    population_current.value = 0;
    //地位在前，高位再后
    for( i = 0 ; i < Chromosome_length -1; i++ ) 
        population_current.value += (double)pow(2, i) * (double)population_current.bit[i];    //遍历染色体二进制编码, 
    //最高位为符号位，如果是1代表负数
    if (population_current.bit[Chromosome_length - 1] == 1)
        population_current.value = 0 - population_current.value;
}
//函数:计算适应度
double objective_function(double x)
{
    double y;
    // 目标函数：y= - ( (x-1)^ 2 ) +5
    y = -((x - 1) *(x - 1)) + 5;                                
    return(y);
}
//函数：更新种群内个体的属性值
//说明：当种群中个体的二进制串确定后，就可以计算每个个体fitness、value、rate_fit 、cumu_fit
//输入：
//chromosome (&population_current)[Population_size] 当前代种群的引用
void fresh_property(chromosome (&population_current)[Population_size])
{
    int j = 0;
    double sum = 0;
    for (j = 0; j < Population_size; j++)
    {
    //染色体解码，将二进制换算为十进制，得到一个整数值
        //计算二进制串对应的10进制数值
        decode(population_current[j]);                 
        //计算染色体的适应度
        population_current[j].fitness = objective_function(population_current[j].value); 
        sum = sum + population_current[j].fitness;
    }
    //计算每条染色体的适应值百分比及累计适应度值的百分比，在轮盘赌选择法时用它选择染色体  
    population_current[0].rate_fit = population_current[0].fitness / sum;
    population_current[0].cumu_fit = population_current[0].rate_fit;
    for (j = 1; j < Population_size; j++)
    {
        population_current[j].rate_fit = population_current[j].fitness / sum;
        population_current[j].cumu_fit = population_current[j].rate_fit + population_current[j-1].cumu_fit;
    }
}
//函数：基于轮盘赌选择方法，对种群中的染色体进行选择  
//输入：
//chromosome (&population_current)[Population_size] 当前代种群的引用
//chromosome (&population_next_generation)[Population_size] 选择出的下一代种群的引用
//chromosome &best_individual 当前代种群中的最优个体
void seletc_prw(chromosome (&population_current)[Population_size],chromosome (&population_next_generation)[Population_size],chromosome &best_individual)
{
    int i = 0, j = 0;
    double rate_rand = 0.0;
    best_individual = population_current[0];
    //产生随机数种子
    srand((unsigned)time(NULL));
    for (i = 0; i < Population_size; i++)
    {
        rate_rand = (float)rand() / (RAND_MAX);
        if (rate_rand < population_current[0].cumu_fit)
            population_next_generation[i] = population_current[0];      
        else
        {
            for (j = 0; j < Population_size; j++)
            {
                if (population_current[j].cumu_fit <= rate_rand && rate_rand < population_current[j + 1].cumu_fit)
                {
                    population_next_generation[i] = population_current[j + 1];
                    break;
                }
            }
        }
        //如果当前个体比目前的最有个体还要优秀，则将当前个体设为最优个体
        if(population_current[i].fitness > best_individual.fitness)
            best_individual = population_current[i];
    }
}
// 函数：交叉操作
void crossover(chromosome (&population_next_generation)[Population_size])          
{   
    int i = 0,j = 0;
    double rate_rand = 0.0;
    short int bit_temp = 0;
    int num1_rand = 0, num2_rand = 0, position_rand = 0;
    //产生随机数种子
    srand((unsigned)time(NULL));
    //应当交叉变异多少次呢？先设定为种群数量
    for (j = 0; j<Population_size; j++)
    {
        rate_rand = (float)rand()/(RAND_MAX);
        //如果大于交叉概率就进行交叉操作
        if(rate_rand <= rate_crossover)
        {
            //随机产生两个染色体
            num1_rand = (int)rand()%(Population_size);
            num2_rand = (int)rand()%(Population_size);
            //随机产生两个染色体的交叉位置
            position_rand = (int)rand()%(Chromosome_length - 1);
            //采用单点交叉，交叉点之后的位数交换
            for (i = position_rand; i<Chromosome_length; i++)
            {
                bit_temp = population_next_generation[num1_rand].bit[i];
                population_next_generation[num1_rand].bit[i] = population_next_generation[num2_rand].bit[i];     
                population_next_generation[num2_rand].bit[i] = bit_temp;     
            }
        }
    }
}
// 函数：变异操作
void mutation(chromosome (&population_next_generation)[Population_size])               
{
    int position_rand = 0;
    int i = 0;
    double rate_rand = 0.0;
    //产生随机数种子
    srand((unsigned)time(NULL));
    //变异次数设定为种群数量
    for (i = 0; i<Population_size; i++)
    {
        rate_rand = (float)rand()/(RAND_MAX);
        //如果大于交叉概率就进行变异操作
        if(rate_rand <= rate_mutation)
        {
            //随机产生突变位置
            position_rand = (int)rand()%(Chromosome_length);
            //突变
            if (population_next_generation[i].bit[position_rand] == 0)
                population_next_generation[i].bit[position_rand] = 1;
            else
                population_next_generation[i].bit[position_rand] = 0;
        }
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 84
- 85
- 86
- 87
- 88
- 89
- 90
- 91
- 92
- 93
- 94
- 95
- 96
- 97
- 98
- 99
- 100
- 101
- 102
- 103
- 104
- 105
- 106
- 107
- 108
- 109
- 110
- 111
- 112
- 113
- 114
- 115
- 116
- 117
- 118
- 119
- 120
- 121
- 122
- 123
- 124
- 125
- 126
- 127
- 128
- 129
- 130
- 131
- 132
- 133
- 134
- 135
- 136
- 137
- 138
- 139
- 140
- 141
- 142
- 143
- 144
- 145
- 146
- 147
- 148
- 149
- 150
- 151
- 152
- 153
- 154
- 155
- 156
- 157
- 158
- 159
- 160
- 161
- 162
- 163
- 164
- 165
- 166
- 167
- 168
- 169
- 170
- 171
- 172
- 173
- 174
- 175
- 176
- 177
- 178
- 179
- 180
- 181
- 182
- 183
- 184
- 185
- 186
- 187
- 188
- 189
- 190
- 191
- 192
- 193
- 194
- 195
- 196
- 197
- 198
- 199
- 200
- 201
- 202
- 203
- 204
- 205
- 206
- 207
- 208
- 209
- 210
- 211
- 212
- 213
- 214
- 215
- 216
- 217
- 218
- 219
- 220
- 221
- 222
- 223
- 224
- 225
- 226
- 227
- 228
- 229
- 230
- 231
- 232
- 233
- 234
- 235
- 236
- 237
- 238
- 239
- 240
- 241
- 242
- 243
- 244
- 245
- 246
- 247
- 248
- 249
- 250
- 251
- 252
- 253
- 254
- 255
- 256
- 257
- 258
- 259
- 260
- 261
- 262
- 263
- 264
- 265
- 266
- 267
- 268
- 269
- 270
- 271
- 272
- 273
- 274
- 275
- 276
- 277
- 278
- 279
- 280
- 281
- 282
- 283
- 284
- 285
- 286
- 287
- 288
- 289
- 290
- 291
- 292
- 293
- 294
- 295
- 296
- 297
- 298
- 299
- 300
- 301
- 302
- 303
- 304
- 305
- 306
- 307
- 308
- 309
- 310
- 311
- 312
- 313
- 314
- 315
- 316
- 317
- 318
- 319
- 320
- 321
- 322
- 323
- 324
- 325
- 326
- 327
- 328
- 329
- 330
- 331
- 332
- 333
同时需要注意的是在利用遗传算法过程中需要较大的种群数量或者设置较高的迭代次数，否则容易过早地到达稳态。
如在计算方程y=-x^2+5的最大值时，所选择的是以6bit来对个体进行编码，其中1bit为符号位。所以，基因型的范围为-32~+31。
而且选择的个体数量为4个，易于过早到达平衡。所设置的迭代次数为20，但是在迭代两次就已经使得个体无差异，无法再继续以交叉组合出更高适应性的基因型。唯一可以期望的是基因突变，但是突变所设置的是小概率事件，本文所设置的约为2%。那么需要增大迭代次数，以增大基因突变的次数，以避免过早陷入最优。
转自：https://blog.csdn.net/ljp1919/article/details/42425281
