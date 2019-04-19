# Linux 查看CPU信息 - Koma Hub - CSDN博客
2017年09月08日 12:04:10[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：172
                
# 总核数 = 物理CPU个数 * 每颗物理CPU的核数 
# 总逻辑CPU数 = 物理CPU个数 * 每颗物理CPU的核数 * 超线程数
# 查看物理CPU个数
`cat /proc/cpuinfo| grep "physical id"| sort| uniq| wc -l`
# 查看每个物理CPU中core的个数(即核数)
`cat /proc/cpuinfo| grep "cpu cores"| uniq`
# 查看逻辑CPU的个数
`cat /proc/cpuinfo| grep "processor"| wc -l`
#查看CPU信息（型号）
`cat /proc/cpuinfo | grep name | cut -f2 -d: | uniq -c`
