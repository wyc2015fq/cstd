# 柔性电子 --Conductive polymers: Creating their niche in thermoelectric domain - 墨流觞的博客 - CSDN博客





2018年11月29日 11:09:35[墨氲](https://me.csdn.net/dss_dssssd)阅读数：65








关键字： Thermoelectric Power Generator(TEG)

讨论内容：
- (i) factors affecting thermoelectric performance
- (ii) strategies required for improvement of the power factor(due to inherent low thermal conductivity) 提高功率因子的策略
- (iii) challenges

figure-of-merit: 品质因数

低的转换效率（一般低于6%）高花费，极少有商业应用。寻求替换材料，比如一些有机材料，

conducting polymers(CPs)  ：free-standing films， ease of patternizing on various types of large-area  flexible substrates，and compatibility to blend with inorganic materials

[3, 6-12]

[7]给出电荷传输的基本概念。

Thermoelectric efficiency($\eta$)与材料品质因数($ZT$)直接相关：

**ZT值，又叫热电优值**（thermoelectric figure of merit）。它是衡量热电材料热电性能的指标和量度，Z是材料的热电系数（单位是/k），有量纲，T是热力学温度,单位是k。ZT乘积来表示热电性能的高低（ZT值越高，热电性能越好）[ZT值（热电优值）怎么换算成转换效率？](https://www.zhihu.com/question/20895956)

$$\eta = \frac{T_h - T_c}{T_h}\left[ \frac{\sqrt{1 + ZT_{avg}} -1 }{ \sqrt{1 + ZT_{avg}}  + \frac{T_c}{T_h} }\right]$$

$T_c和T_h$是冷端和热端的温度，$T_{avg} =\frac {T_c +T_h}2$, $ZT_{avg}$通过对$T_c和T_h$之间所有的峰值ZTs积分获得，在特定温度$T$下,材料的$ZT$:
$$ZT=\alpha^2\sigma T / \kappa$$
$\alpha$是Seebeck coefficient， $\sigma$是导电率， $\kappa$是材料的热导率，$\kappa$有两部分构成，载流子(charge-carriers $\kappa_e$)和晶格(lattice, $\kappa_l$)
要实现高的ZT和转换效率 ($\eta$),需要满足：
- (i) 材料有高的塞贝克系数 从而有高的电输出
- (ii) 高的导电率，从而有大的短路电流和低的焦耳热
- (iii) 低的热导率，从而有大的温差

掺杂(doping)可以增加导电率，但是会降低塞贝克系数。

以下要讨论一些方法，优化$\alpha和\sigma$而不改变材料的$\kappa$来强化转化效率。

从转换效率$\eta$的式子中可以看出，$\eta$与卡诺循环效率($\Delta T/ T$)和热电优值(ZT)有关,而 $\Delta T/ T$在室温下应用中非常小，这表明在利用热能方面，与现在昂贵的TE模块相比，基于CP的TEGs会更为实用

在150摄氏度的情况下，考虑CPs的稳定性， 假设热端温度为100摄氏度，冷端温度约为27摄氏度， $ZT_{avg}$~1.如图1所示，有机热电效率(organic thermoelectric generators OTEGs)的转换效率约为4%

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018112910564563.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

室温下高的ZT值， 0.42[72]和0.2[91]对于P-和n-type的CPs已经实现。

### 2. CPs中热和电转换原理

绝缘聚合物有共键价原子组成(类似于Si)，$sp^3$杂化的碳原子，共享电子被束缚在链形分子的**低能量**轨道上，导致导电率很低。但是在一些共轭聚合物(conjugated polymers), 比如聚乙炔(trans-polyacetylene), 碳原子$sp^2$杂化，在垂直于$\sigma$键的方向形成$\pi$键。由于形成交叠区域，相邻C原子的$sp^2$杂化的$p_z$轨道，导致沿着聚合物主体方向，离域电子的移动， 在这种交替的$\sigma$和$\pi$分布的C-atoms中，在激发光谱中，由于Peierls instability， $\pi-band$会被分成$\pi-band(电子填充满)和\pi^*band(无电子)$并有一个能量为$E_g$的gap。与传统的半导体相比，结合的轨道($\pi$)对应于半导体中的价带， 而未结合的轨道 ($\pi^*$)对应于半导体中的导带。相似的，在Highest Occupied Molecular Orbital (HOMO) 和 Lowest Unoccupied Molecular Orbital (LUMO)之间的能量$E_g$，类似于半导体中的禁带宽度$E_g$。因此，共轭聚合物的导电率仍然很低，所以说 ，仅靠共轭聚合是不够的，然而，导电率可以通过掺杂增强，但是由于CPs的structure-property relationships，掺杂有副作用。这将在section 4中导论。

##### 2.1 Electrical conductivity 导电率

一般来说，可以使用电化学或化学方法对聚合物进行掺杂。两种方法处理流程：
- (i) 一个单元结构(monomer)氧化的开始是从$\pi-system$中除去电子，产生一个自由基(free radical)和无自旋的正电荷(spinless positive charge),减少的掺杂剂转化为平衡离子，中和引入到$\pi-electron$系统的正电荷。
- (ii) 电荷与自由基的局部振动引起电荷和自由基的耦合，而这种耦合称为极化(polaron)
- (iii)极化形式可以为自由基阳离子或自由基阴离子(radical cation or radical anion)，在带隙内形成新的局部电子态，低能态被单电子占据，
- (iv)从极化子从除去或添加新的电子会进一步引起新的无旋缺陷，称为双极化，是一个与极化畸变(polaronic distortion)有关的自由基离子对,根据不同的聚合物的化学结构，可以受到其他3到4个单元结构(monomer)的影响。

[22]双极化子的产生在热力学上要比两个极化子的产生更有用，因此，在高的掺杂水平下，两个极化子变为一个双极化子的可能性越大。

**其实会联系到半导体中重掺杂下的能级交叠**

但是如果掺杂浓度过高，双极化能级会交叠，最终形成连续的带。如下图，并且双极化之间的间隙也会增加因为新形成的双极化子是以牺牲带边为代价而形成的，差杂浓度进一步增加，会导致双极化带的上边界和下边界分别和导带和价带结合，形成类似金属的带[7, 22]
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129163233170.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
基态退化的共轭聚合物， 比如聚乙炔(polyacetylene)的导电机理可以使用稍微不同的机理解释，由于氧化差杂导致极化子/双极化子的产生 [20-21]。由于基态的退化，带电的阳离子受束缚较弱，可以沿着链自由移动，这种电荷位置相互独立，并隔离基态的两相(自旋相反但能量相同)，称为孤子(soliton)，有时是中性的。例如，

假设聚乙炔产生的孤子可以离域化的距离为12个C-H单元，并且最大的电荷密度与掺杂离子浓度相同。孤子的形成会在带隙之间形成新的局部态，随着掺杂浓度的增加，就像双极化子一样，带电的孤子可会与其他的孤子交叠形成孤子带，最终会与导带/价带接触，导致类似金属的导电性。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181129170004681.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
这种导电机理会受到CPs中的能量障碍和空间障碍影响。通过掺杂引入的额外载流子由于与反离子(掺杂剂)的静电吸引而滞留在链中。[27, 86]在低介电常数(~3)中 ，低掺杂会产生大尺寸的陷阱，而这些陷阱会阻碍载流子的移动， 随着掺杂浓度的增加，陷阱会交叠，他们之间的能量势垒会降低。这种在$\pi-轨道$上能量势垒的降低会增强电子移动[11].**通过增加差杂浓度实现**，然而，空间障碍也会在原子核微观尺度上影响你电荷传输，比如counter ions position,interchain distance, configuration of chains, size of crystalline and amorphous domains, orientation of crystallite。

在CPs中，电子传输是通过跳动过程(hopping process[2, 11-12])实现的,在非掺杂或低掺杂聚合物中， 在外电场作用下，极化子会跳跃到临近的聚合物链的中性区域，

另一方面，在掺杂聚合物中，载流子(一对极化子和反离子)从一个极化子位点跳到另一个极化子位点，而构成跳跃位点的聚合物链不发生任何畸变。在无序或非晶的CPs中，温度与导电率的关系使用VRH模型描述(variable range hopping),假设 constant density of states（定态密度）。

根据VRH模型， 温度与导电率的关系如下:
$$\sigma(T) = \sigma(0)exp(\lfloor -(T_0 / T)^{\lambda}\rfloor)$$
$T_0$为莫特温度值，
$$T_0 = \frac{18}{L{_c^3N(E_F)K_B}}$$
$L_c$为定域长度， $N(E_F)$为费米能级的态密度，$K_B$为玻尔兹曼常数。
适度掺杂和结构有序的CPs， 随着链之间定域长度的增加，链之间的连接性更好，会导致很好的电转移特性，

例如，高掺杂和高有序的CPs，比如樟脑磺酸掺杂的PANI(聚苯胺),在低温下表现很好的金属温度依赖性。

##### 2.2 seebeck  cofficient  塞贝克系数

塞贝克系数是开环电压(温度热端和冷端两端之间的电压 )和温差之比，塞贝克系数有三种不同的贡献因子：电子(electronic) 声子(phonon)和电-声子(electron-phonon),在金属系统中存在大量的自由电子，电子主要贡献塞贝克系数。电子从热端移动到 冷端形成电势，产生电子贡献。在这种情况下，塞贝克系数与温差成线性关系[14]

声子贡献主要认为声子曳引效应(Phonon drag), 一般在低温(<200k)比较明显，在低温时声子平均自由程(phonon mean free path)比较大[7]。当声子和电子的平均自由程可比拟时，电子-声子散射变得重要[7, 11].好的晶体结构的高导电聚合物在低温时有较好的电子-声子散射[11, 12]。**重掺杂的聚合物**，比如polyacetylene(聚乙炔),polyaniline(聚苯胺),polypyrrole (聚吡咯)存在很小的正塞贝克系数($&lt;14uV/K$在接近室温时)，并且随温度有线性下降[7]。然而，由于电子-声子作用可能会导致线性偏差。**在轻掺杂的情况下**，塞贝克系数要大于重掺杂[7, 12]，对于这种CPs，塞贝克系数随着温度可能非线性增加或降低。有时， 塞贝克系数非线性随着$T^{1/2}$非线性下降，这可以用局部状态之间的莫特变化范围跳变传输解释。局部状态内最邻近跳跃使得塞贝克系数与温度有$1/T$的依赖关系，但是在重掺杂下，塞贝克系数随着温度线性增加[12, 24]

##### 2.3 Thermal  conductivity  热导率

在无机材料中，热导率中的电子部分与电导率耦合，而在有机材料中，由于更强的电荷-晶格耦合效应，这种协同效应无效（违反了Wiedemann-Franz law）。除此之外，由于与无机材料 相比，有机材料的电导率更低,因而电子对$\kappa$的贡献更低[16, 17]。在有机材料中，热传导主要依靠声子(phonon)（比如，量子晶格振动）实现而不是电荷载流子(例如 $\kappa_l&gt;\kappa_e$)。对于CPs，$\kappa$不仅依赖于分子的重量和形状，还依赖于链结构的不同。对于聚合物的主要链状结构：
- Linear polymers
- Branch polymers
- Crosslinked polymers
- Network polymers

如下图(a)：
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018113010033359.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
聚合物链有多种结构，结合会形成非晶(amorphous)或半晶结构(semi-crystalline), 如上图(b)。不同的结构在热导率和热扩散上有各向异性[18-19]， 对于一个特定的CP，在更有序的链上，各向异性更明显。

尽管这种各向异性(anisotropy)是排列整齐，有序的聚合物链的特点，但是在非晶结构上也存在。这可以解释为热传输机制在两个方向上不同[18]。平面内导热系数值高于横向导热系数，这是由于在聚合物链上的强C-C共价键作用，而不是通过垂直方向上的弱范德华分子相互作用传递热能。如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181130101600243.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
机械形变(在拉伸力作用下)和沉积到基板上的不同方式(spin coatinng, screen printing)也可以将热传输的各向异性引入到聚合物材料中.旋涂过程中的**离心力**导致平面内和平面间导热性的不同[18]。值得注意的是，对于大多数CPs，即使$\sigma$可以有三个数量级的增加，$\kappa$的变化也很小(比如， 从0.1W/mK到1.0W/mK)。由于CPs的$\kappa$很小，所以测量过程更复杂。同时，导热率的各向异性需要考虑在内[18, 19]。当$\sigma$增加（>=100S/cm）时，也会导致$\kappa_e$的增加，当$\kappa_e$可以与$\kappa_l$相比拟甚至超出时，在估计热导率的时候也要考虑$\kappa_e$了[11]。$\kappa$与CPs的结晶程度也有关系， 比如在低结晶的CPs中，热导率单调增加；而对于高结晶CPs，热导率很高，随着温度升高（up to 100k）先增加后减小。低温时热导率的偶然上升可以解释为声子-声子散射的抑制导致了声子平均自由增加[11]。

### 3.导电聚合物热点性能总结

需要通过优化$\sigma , \alpha 和 \kappa$来实现高的$ZT_{acg}$，从而实现高的转换效率。CPs在高温下的不稳定形也限制了最大的工作温度，$&lt; = 150摄氏度$ ,在PEDOT:PSS中获得了室温下，0.42的ZT，也达到了常规无机材料下的最佳效果($Bi_2Te_3$【11， 13】)，尽管基于$Bi_2Te_3$的混合物可以在$47^。C$下ZT达到1.86，$\kappa$低达0.7W/mK, $\kappa$仍然比CPs高4~5倍。因此，在优化三个参数达到高的ZT时，CPs不需要考虑$\kappa$,因为其已经很低了【8】，【48,81】研究了一些将有机和无机材料优点结合起来的混合材料。结构操作方法也被提出强调。拉伸和PEDOT:PSS and PANI复合聚合物材料的多层处理【 113】将粘合剂【77】和填充剂【80】添加到聚合物基体中会通过增加链连接性，加速电荷传输【112】来动态的增加热点性能，

table1给出一些聚合物的性能比价：

dopant 掺杂剂

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201170456871.png)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201170212828.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201170232309.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201170242630.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201170253106.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120117030655.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201170319296.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018120117033192.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

##### 3.1 乙炔 polyacetylene PA

通过添加卤素，碘，溴，arsenic pentafluoride (AsF5)等，在电导率上有极大的增强，变化范围从绝缘体到半导体再到导体。

##### 3.2  polycarbazole及其衍生物

##### 3.3 Poly-pyrrole及其复合物

##### 3.4 Poly(3-hexylthiophene) (P3HT) and its composites

##### 3.5 Polyaniline (PANI) and its composites

##### 3.6 PEDOT (3-4 ethylenedioxythiophene)

意思是这个比较好了，是polythiophene,的衍生物PEDOT使用polystyrenesulphonic (PSS) acid来掺杂，使溶液可溶，广泛的用于发光二极管和太阳能电池等有机电子器材的空穴注入层， 高导电性的PEDOT:PSS水溶液的商业可用性和易于掺杂能在不影响塞贝克系数的情况下控制载流子浓度。使得**PEDOT**为一个比较吸引人的热电应用的替代品。【6】
**值得注意啊**

ZT ~ 2.17 在17K 【61】  PANI

ZT ~ 0.5 在420K 【55】 P3HT:PCBM。

ZT ~ 0.42 在室温下， DMSO-mixed PEDOT:PSS 【72】 主要是链结构更加有序 使得$\sigma$增加，下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181201220721880.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

在**PEDOT：PSS**中那个加入山梨醇(sorbitol)【73】

deep eutectic solvents(DES) 低共熔溶剂代替传统有机溶剂(conventional organic solvents)

### 4. 导电聚合物中一些关键问题 Key issues with conducting polymers

事实上没有一组通用的规则来优化TE 参数【106】，**感觉在瞎搞**，我们需要针对特定的CP找到特定的方法，依赖于许多因子比如chemical structure化学结构，feasibility of carrying out research with compatible dopants,研究兼容掺杂的可行性 desirable device geometry (like in-plane and out of plane)理想的器件几何结构，因而，在特定的TE应用中，充分利用CPs需要注意以下几个问题：
- 影响热点性能的因子 factors affecting thermoelectric properties
- 在研究中遇到的问题 challenges encountered during research
- 优化热点性能的策略 strategies required to optimize thermoelectric performances

##### 4.1  Factors influencing thermoelectric properties 因子

CPs的电和热性能有结构和化学设计 决定。在形态学morphology,链结构 chain structure和电荷传输 charge transport之间的联系尚未完全理解。需要氧化掺杂 oxidative doping, 导致难溶性infusibility和链主干很低的环境稳定性。然而添加取代基或官能团来增加溶解性会使处理流程更复杂。一下讨论影响TE性能的因素

###### a> polymer structure 聚合物结构

###### b> Polymer concentration 聚合物浓度

在合成过程中聚合物的浓度会导致链的长度的分子质量，这会影响导电聚合物的溶解度和粘度(solubility and viscosity)

###### c> Polymer Molecular Weight and Chain Length 聚合物分子量和 链长度

载流子移动速度受聚合物的分子量的影响。低分子量聚合物比高分子量聚合物具有更短的链长和更大的结晶性

链越长会增加载流子移动速率和热导率
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181202142350758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
###### d> 温度 Temperature

都受工作温度影响[13]单个性能的叠加

###### humidity 潮湿度

PEDOT：PSS的报道实在高潮时环境下的，

###### e> 聚合物链的对齐 Alignment of the polymer chains

增加电导率

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181202143112790.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

通过拉伸来对齐，几乎不高变塞贝克系数，但是导电率增加了两个数量级。
##### 4.2 Challenges with the organic materials for thermoelectric applications 有机材料在热电应用领域的挑战

###### a>长期的稳定性

掺杂虽然会增加导电率，但是导致很坏的加工性能和很差的环境稳定性。p-type的自然下

###### b>Sample Preparation and Measurement Techniques样本制造和测量技术

统的制备无机热电材料的方法，如球磨法和热压法，不适用于溶液处理的有机热电材料。Spin-coating and drop-casting常用来沉积有机热电薄膜，但是大规模制造方法仍在开发。

四探针技术

####### 4.3 Adherence of films to substrates薄膜与基底的粘附性

使用粘合剂

Interfacial polymerization technique界面聚合技术

掺杂也会增加脆性，因而要选择合适的掺杂浓度
###### d>在接触区域有良好的电气性能

###### 4.3 优化导电聚合物电热性能的策略

###### a> 聚合物掺杂

使用掺杂剂对聚合物进行掺杂能增加导电性，

在聚合物浓度和掺杂俩调节电器属性是常用的方法。载流子浓度增加会使得到导电率增加，但是b会yin塞贝克系数的降低，因而ZT值很小的增加，

CPs处于绝缘或临界状态(insulating or critical regime)，

【72】
###### b>Polymers - Carbon nanotubes blends

聚合物和碳纳米管混合

[112]

将纳米管添加到聚合物基体上【112】

###### c> Organic-Inorganic composites有机无机复合材料

在聚合物中引入无机材料提高热点性能【82】

###### d>Nanoscaled organic/inorganic materials 有机无机纳米材料

将纳米尺寸的碳纳米管/无机材料添加到导电聚合物中会增加热点性能。

###### e>Energy Filtering effect

###### f>Bilayer/multilayer approach双层或多层处理方法

【113】通过多层处理方法来优化TE性能。

Multilayer deposition 多层沉积【113】

###### g>一些外部控制因素

光激发会增强ZT值【55】
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181202151305796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
在电聚合过程中改变电势也会增加性能【79】.改变电势和粒子直径也会影响【69】，【71】

### 5.导电聚合物的设计和发展
















