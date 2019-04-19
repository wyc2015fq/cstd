# shell 求和的方法 - sxf_123456的博客 - CSDN博客
2017年04月26日 10:41:46[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：944
                
#!/bin/sh
#sum.sh
#1
total=0
for((i=0;i<10;i++))  #直接数值求和
do
        total=$(($total+$i))
done
echo $total
#2
total2=0
while [ $# -gt 0 ]   #参数求和
do
        total2=$(($total2+$1))
        shift
done
echo $total2
#3
sum=0
for i in `cat data.txt`  #数值存放文件中
do
        #echo $i
        sum=$(($sum+$i))
done
echo "result:"$sum;
#4
function add()   #函数
{
# echo $1
 num=$1
 total3=0
 for((i=1;i<=$num;i++))
 do
        total3=$(($total3+$i))
 done
        echo $total3
}
add 5
add 10
add 100
