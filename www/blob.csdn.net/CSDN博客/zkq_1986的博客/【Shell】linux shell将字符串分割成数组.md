# 【Shell】linux shell将字符串分割成数组 - zkq_1986的博客 - CSDN博客





2017年12月14日 09:51:40[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：846








s="a,b,c"

OLD_IFS="$IFS"                #IFS : Internal Field Separator，默认值为空格，tab,
 和新行

IFS=","                               #将IFS临时替换为","

arr=($s)

IFS="$OLD_IFS"              #将 IFS 替换回去  

for i in ${arr[@]}               

do

    print $i

done







输出：

a

b

c




