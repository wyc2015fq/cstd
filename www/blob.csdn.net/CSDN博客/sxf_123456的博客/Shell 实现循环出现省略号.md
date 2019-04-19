# Shell 实现循环出现省略号 - sxf_123456的博客 - CSDN博客
2017年04月24日 20:14:08[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：543
                
#!/bin/sh
function start_pro()
{
  min=1
  max=2
  while [ $min -le $max ]
  do
  for((i=0;i<6;i++))
  do
        printf "."
        #printf " "
        #printf "\b"
        sleep 1
  done
  tab
 min=`expr $min + 1`  #i++
 #printf $min
 done
 printf "\n"
}
function tab()
{
  for((i=0;i<6;i++))
  do
        printf "\b"  #退回
        sleep 1
  done
}
case $1 in
        start)
        start_pro
        ;;
        stop)
        echo "stop....."
        ;;
esac
            
