# csdn泄漏出来密码中被使用最多得密码 - Arthur的随笔 - CSDN博客
2011年12月21日 23:36:08[largetalk](https://me.csdn.net/largetalk)阅读数：1652
```
cat www.csdn.net.sql | awk -F# '{print $2}' | sort | uniq -c | sort -rn | head -n 10
```
 235012  123456789 
 212749  12345678 
  76346  11111111 
  46053  dearbook 
  34952  00000000 
  19986  123123123 
  17790  1234567890 
  15033  88888888 
   6995  111111111 
   5965  147258369 
