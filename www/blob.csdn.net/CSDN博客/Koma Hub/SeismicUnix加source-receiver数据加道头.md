# SeismicUnix加source-receiver数据加道头 - Koma Hub - CSDN博客
2017年09月07日 19:52:26[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：367
```
#!/bin/sh
suaddhead <   shot.dat    ns=$nt   |
sushw   key=sx   a=$fs   b=0       c=$ds   j=$nx  |
sushw   key=gx  a=$fg   b=$dg  c=0       j=$nx  |
suchw   key1=offset    key2=sx    key3=gx    b=-1    c=1    d=1  |
suchw   key1=cdp    key2=sx    key3=gx    b=1    c=1    d=$2*dx  |
sushw   key=dt       a=$dt  |
sushw   key=scalco     a=1   >   shot.su
```
