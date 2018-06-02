clc
clear all
img=imread('test.bmp');
minWaveLength       =  4;
sigmaOnf            =  0.64;
mult                =  1.7;
region_num          =  8;
nscale              =  3;
orientWrap          =  0;
bh_n                =  4;
bw_n                =  5;
sh_n                =  1;
sw_n                =  1;
neigh               =  8;
radius              =  3;
NumTarget           =  10;
  [f1, h1f1, h2f1, A1,theta1, psi1] = monofilt(img, ...
            nscale, minWaveLength, mult, sigmaOnf, orientWrap);
        