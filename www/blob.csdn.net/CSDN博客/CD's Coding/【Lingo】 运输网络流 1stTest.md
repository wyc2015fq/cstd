# 【Lingo】 运输网络流 1stTest - CD's Coding - CSDN博客





2013年12月11日 16:41:09[糖果天王](https://me.csdn.net/okcd00)阅读数：733








```
MODEL:
Title Location Problem;
sets:
	demand/1..6/:a,b,d;
	supply/1..2/:x,y,e;
	link(demand,supply):c;  !'()' Not '{}' 
! 'link' is a combination of two or more sets;
endsets

data:
!locations for the demand();
a=1.25,8.75,0.5 ,5.75,3   ,7.25;
b=1.25,0.75,4.75,5   ,6.5 ,7.75;
!quantities of the demand and supply;
d=3, 5, 4, 7, 6, 11; e=20, 20;
enddata

init:
!initial locations for the supply;
x,y=5,1,2,7; ! P(5,1)	Q(2,7);
endinit	!give values by column;

!Objective function;
[OBJ] min=@sum(link(i,j):c(i,j)*((x(j)-a(i))^2+(y(j)-b(i))^2)^(1/2));
!Demand limit(constraints);
@for(demand(i):[DEMAND_CON] @sum(supply(j):c(i,j))=d(i);); ![DEMAND_CON]为添加关注名，在LingoSolveReport里将专门列出数据;
!Supply limit(constraints);
@for(supply(i):[SUPPLY_CON] @sum(demand(j):c(j,i))<=e(i););
@for(supply:@bnd(0.5,X,8.75); @bnd(0.75,Y,7.75););  !bnd represent range 0.5<=x<=8.75;
END
```




Solve 全局最优解：



```
Local optimal solution found.
  Objective value:                              85.26604
  Infeasibilities:                              0.000000
  Total solver iterations:                            61


  Model Title: Location Problem

                       Variable           Value        Reduced Cost
                          A( 1)        1.250000            0.000000
                          A( 2)        8.750000            0.000000
                          A( 3)       0.5000000            0.000000
                          A( 4)        5.750000            0.000000
                          A( 5)        3.000000            0.000000
                          A( 6)        7.250000            0.000000
                          B( 1)        1.250000            0.000000
                          B( 2)       0.7500000            0.000000
                          B( 3)        4.750000            0.000000
                          B( 4)        5.000000            0.000000
                          B( 5)        6.500000            0.000000
                          B( 6)        7.750000            0.000000
                          D( 1)        3.000000            0.000000
                          D( 2)        5.000000            0.000000
                          D( 3)        4.000000            0.000000
                          D( 4)        7.000000            0.000000
                          D( 5)        6.000000            0.000000
                          D( 6)        11.00000            0.000000
                          X( 1)        3.254883            0.000000
                          X( 2)        7.250000           0.6335133E-06
                          Y( 1)        5.652332            0.000000
                          Y( 2)        7.750000           0.5438639E-06
                          E( 1)        20.00000            0.000000
                          E( 2)        20.00000            0.000000
                       C( 1, 1)        3.000000            0.000000
                       C( 1, 2)        0.000000            4.008540
                       C( 2, 1)        0.000000           0.2051358
                       C( 2, 2)        5.000000            0.000000
                       C( 3, 1)        4.000000            0.000000
                       C( 3, 2)        0.000000            4.487750
                       C( 4, 1)        7.000000            0.000000
                       C( 4, 2)        0.000000           0.5535090
                       C( 5, 1)        6.000000            0.000000
                       C( 5, 2)        0.000000            3.544853
                       C( 6, 1)        0.000000            4.512336
                       C( 6, 2)        11.00000            0.000000

                            Row    Slack or Surplus      Dual Price
                            OBJ        85.26604           -1.000000
                 DEMAND_CON( 1)        0.000000           -4.837363
                 DEMAND_CON( 2)        0.000000           -7.158911
                 DEMAND_CON( 3)        0.000000           -2.898893
                 DEMAND_CON( 4)        0.000000           -2.578982
                 DEMAND_CON( 5)        0.000000          -0.8851584
                 DEMAND_CON( 6)        0.000000            0.000000
                 SUPPLY_CON( 1)        0.000000            0.000000
                 SUPPLY_CON( 2)        4.000000            0.000000
```





