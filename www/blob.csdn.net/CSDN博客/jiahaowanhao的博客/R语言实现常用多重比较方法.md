
# R语言实现常用多重比较方法 - jiahaowanhao的博客 - CSDN博客


2018年03月03日 20:19:44[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1256标签：[R语言																](https://so.csdn.net/so/search/s.do?q=R语言&t=blog)


[R语言实现常用多重比较方法](http://cda.pinggu.org/view/24889.html)
在单因素方差分析ANOVA中，如果该因素影响比较显著，那么需要进一步利用多重比较方法比较该因素不同水平的影响，确定不同水平下该因素的影响是否显著。常见的多重比较方法主要有两种，LSD法和Tukey HSD法。下面对R语言中，这两种多重比较方法的实现进行举例。
前期数据如下，影响因素为group，指标为value：
> head(tarD)
value  group      sample   time
A0522W11NC1 0.0002053745 normal A0522W11NC1 11week
A0522W11NC2 0.0031773712 normal A0522W11NC2 11week
A0522W11NC3 0.0060378288 normal A0522W11NC3 11week
A0522W11NC4 0.0017626931 normal A0522W11NC4 11week
A0522W11NC5 0.0018035261 normal A0522W11NC5 11week
A0522W11NC6 0.0036690067 normal A0522W11NC6 11week
> tmp <- aov(value ~ group, tarD)
最小显著差数检验法(LSD法)
> res <- LSD.test(tmp, 'group', p.adj = 'bonferroni')
> print(res$groups)
trt        means M
1   normal 2.576910e-03 a
2    drug3 7.552555e-04 b
3    drug2 7.269247e-05 b
4 high_fat 6.220610e-05 b
5    drug1 2.954733e-05 b
Tukey氏固定差距检验法(Tukey HSD)
> TukeyHSD(tmp)
Tukey multiple comparisons of means
95% family-wise confidence level
Fit: aov(formula = value ~ group, data = tarD)
$group
diff           lwr          upr     p adj
drug2-drug1      4.314514e-05 -0.0015468705 0.0016331608 0.9999916
drug3-drug1      7.257082e-04 -0.0008643074 0.0023157239 0.6929965
high_fat-drug1   3.265877e-05 -0.0015149488 0.0015802664 0.9999969
normal-drug1     2.547362e-03  0.0009997549 0.0040949700 0.0002613
drug3-drug2      6.825631e-04 -0.0009487586 0.0023138847 0.7563196
high_fat-drug2  -1.048637e-05 -0.0016005020 0.0015795293 1.0000000
normal-drug2     2.504217e-03  0.0009142017 0.0040942330 0.0004945
high_fat-drug3  -6.930494e-04 -0.0022830651 0.0008969662 0.7277757
normal-drug3     1.821654e-03  0.0002316386 0.0034116699 0.0175538
normal-high_fat  2.514704e-03  0.0009670961 0.0040623113 0.0003161
> TukeyHSD(tmp)$group
diff           lwr          upr        p adj
drug2-drug1      4.314514e-05 -0.0015468705 0.0016331608 0.9999915820
drug3-drug1      7.257082e-04 -0.0008643074 0.0023157239 0.6929965170
high_fat-drug1   3.265877e-05 -0.0015149488 0.0015802664 0.9999969171
normal-drug1     2.547362e-03  0.0009997549 0.0040949700 0.0002612744
drug3-drug2      6.825631e-04 -0.0009487586 0.0023138847 0.7563195891
high_fat-drug2  -1.048637e-05 -0.0016005020 0.0015795293 0.9999999705
normal-drug2     2.504217e-03  0.0009142017 0.0040942330 0.0004944674
high_fat-drug3  -6.930494e-04 -0.0022830651 0.0008969662 0.7277757202
normal-drug3     1.821654e-03  0.0002316386 0.0034116699 0.0175537862
normal-high_fat  2.514704e-03  0.0009670961 0.0040623113 0.0003161003

