# psimage生成eps格式图片参数 - Koma Hub - CSDN博客
2017年09月07日 19:53:48[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：478
```
psimage  <  **.dat n1=**  width=** height=** perc=99 cmap=rgb1 \
                     wrgb=1.0,0,0 grgb=1.0,1.0,1.0 brgb=0,0,1.0  title="***"  > ***.eps
```
```
psimage  <  vel101101final1.dat n1=101 d1=20 n2=101 d2=20 \
                    label1='depth(m)' label2='width(m)'  width=5 height=5 \
                    cmap=rgb1 wrgb=1.0,0,0 grgb=1.0,1.0,1.0 brgb=0,0,1.0 > vel_f1.eps
```
```
#############
## 完整的成图 ##                                                    
#############
#!/bin/sh
nx=601
nxa=455
nz=301
dx=5
dz=5
width=12
depth=6
cchar='cmap=rgb1 wrgb=1.0,0,0 grgb=1.0,1.0,1.0 brgb=0,0,1.0'
labelsize=30
perc=99
psimage  <  migration.dat n1=$nz d1=$dz n2=$nx d2=$dx \ 
                   label1='depth[m]' label2='width[m]'  \
                   $cchar  width=$width height=$depth  labelsize=$labelsize \ 
                   perc=$perc > eps_migration.eps
psimage  <  illumination.dat n1=$nz d1=$dz n2=$nx d2=$dx \
                   label1='depth[m]' label2='width[m]'  \
                  $cchar  width=$width height=$depth  labelsize=$labelsize \ 
                  perc=$perc > eps_illumination.eps
psimage  <  adcigs.dat n1=$nz d1=$dz n2=$nxa  label1='depth[m]' \ 
                  label2='cdp-angle'  \
                 $cchar  width=$width height=$depth labelsize=$labelsize \
                 perc=$perc > eps_adcigs.eps
#psimage  <  thrust_vel_711_300.bin n1=$nz d1=$dz n2=$nx d2=$dx \
                  label1='depth[m]' label2='width[m]'  \
                 # $cchar  width=$width height=$depth legend=1 lstyle=vertright \
                 lheight=5 labelsize=$labelsize perc=$perc > eps_thrust_vel_711_300.eps
#psimage  <  thrust_epsilon_711_300.bin n1=$nz d1=$dz n2=$nx d2=$dx \
                  label1='depth[m]' label2='width[m]'  \
                  #$cchar  width=$width height=$depth legend=1 lstyle=vertright \
                  lheight=5 labelsize=$labelsize perc=$perc > eps_thrust_epsilon_711_300.eps
#psimage  <  thrust_delta_711_300.bin n1=$nz d1=$dz n2=$nx d2=$dx \
                 label1='depth[m]' label2='width[m]'  \
                 #$cchar  width=$width height=$depth legend=1 lstyle=vertright \
                 lheight=5 labelsize=$labelsize perc=$perc > eps_thrust_delta_711_300.eps
```
