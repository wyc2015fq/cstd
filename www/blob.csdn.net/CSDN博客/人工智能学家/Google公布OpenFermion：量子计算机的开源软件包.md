# Google公布OpenFermion：量子计算机的开源软件包 - 人工智能学家 - CSDN博客
2017年10月25日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：341
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/waLJGrhJM0fhq6icNo7ia1SvnubDjgh7dNLe2rU89PInBrd1ZXe9DzEmvFbYPxvKAib1vrt1d6TfxDjzia5a9JPgCw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
*来源：转载自公众号「雷克世界」微信号：ROBO_AI*
*作者：Ryan Babbush、Jarrod McClean*
*编译：嗯~阿童木呀、多啦A亮*
*概要：OpenFermion是一个用于模拟电子（费米子）相互作用系统的库，它能够产生物质的性质。*
“对于大部分物理学和整个化学领域的数学理论来说，其所必需的基本物理定律是完全已知的，而困难仅是这些定律的精确应用所带来的方程式过于复杂，以致难以解决”
                                           ——保罗•狄拉克《多电子系统的量子力学》（1929）
物理学家保罗•狄拉克（Paul Dirac）哀叹道，尽管量子力学能够对所有的化学领域进行精确地建模，但想要精确地模拟相关的方程式似乎有着不可避免的复杂性。这个问题长期以来一直困扰着大家，直到1982年，理查德•费曼（Richard Feynman）提出这样一种理论：与其屈服于量子力学的复杂性而无可作为，我们倒不如可以将其作为一种计算资源。因此，量子计算最初的发展动机是：在遵循量子力学定律的基础上操作计算机，人们可以对自然进行高效精确的模拟。而这种模拟可能导致在诸如光电池、电池、新材料、药品和超导等领域带来新突破。虽然现在我们还没有一个足够大的量子计算机来解决这些领域的经典难题，但我们正在不断地取得快速发展。去年，谷歌发表了下面所展示的这篇论文，详细介绍了使用超导量子位的量子计算机所进行的分子的首次量子计算。在这项研究的基础上，IBM量子计算小组将实验扩展到了更大的分子上，并在上个月占据了《自然》杂志的封面。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0fhq6icNo7ia1SvnubDjgh7dNYmoSiaAo3uFT67Wl8WjNf6ToTRicY0q7Cbe9ns7NP4HFb2HNic2pXSBaA/640?wx_fmt=png)
*谷歌去年发表的论文《分子能量的可扩展性量子模拟》*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/waLJGrhJM0fhq6icNo7ia1SvnubDjgh7dNLe2rU89PInBrd1ZXe9DzEmvFbYPxvKAib1vrt1d6TfxDjzia5a9JPgCw/640?wx_fmt=jpeg)
9月13日，IBM宣布，在自己的超导量子设备上实现了一种新的量子算法，这种算法可以模拟真实分子，能够高效精确地计算出小分子电子的最低能态。此次他们就用量子计算机推导了氢化铍（BeH2）分子的最低能量状态。该研究成为了《自然》杂志最新一期的封面文章。
现在，我们宣布发布OpenFermion，这是第一个能够将化学和材料科学问题转化为可以在现有平台上执行的量子电路的开源平台。OpenFermion是一个用于模拟电子（费米子）相互作用系统的库，它能够产生物质的性质。在OpenFermion开发之前，量子算法开发人员需要学习大量的化学知识，并且要编写大量的代码，除了要将其他的代码组合在一起，甚至连最基本的量子模拟也涵盖在内。该项目起源于谷歌，在项目进行过程中，苏黎世联邦理工学院、美国劳伦斯伯克利国家实验室、美国密歇根大学、哈佛大学、牛津大学、达特茅斯大学、Rigetti
 Computing公司和美国国家航空航天局都为alpha版本的发布做出了贡献有关此版本发布的更多详情，可阅读此论文：《OpenFermion：量子计算机的电子结构软件包》。
一种常见的方法是将OpenFermion看做是生成和编译物理方程式的工具，能够将化学和物质系统描述成可以由量子计算机解释的表征。针对这些问题，最高效的量子算法得以建立，并扩展了由政府、工业界和学术界的研究化学家使用和开发的经典量子化学包的能力。因此，我们也将发布OpenFermion-Psi4和OpenFermion-PySCF，这是当OpenFermion和经典的电子结构包Psi4和PySCF协同使用时的插件。
核心OpenFermion库是以一种量子编程框架无关的方式设计的，以确保与社区开发的各种平台之间的兼容性。这使得OpenFermion能够支持外部包，而这些包可以为不同的硬件平台编译量子汇编语言规范。我们希望这一决定能够有助于建立OpenFermion成为将量子化学放置于量子计算机上的一个社区标准。要了解OpenFermion是如何与不同的量子编程框架协同使用的，请参阅OpenFermion-ProjectQ和Forest-OpenFermion-plugins——它们可以将OpenFermion与称为ProjectQ和Forest的外部开发的集成电路模拟和编译平台连接在一起。
以下工作流程描述了量子化学家是如何使用OpenFermion来模拟一个分子的能量表面的（例如，通过准备我们在过去的文章中所描述的量子计算种类）：
1.研究人员对OpenFermion计算进行初始化，具体规定如下：
•输入文件指定分子中核的坐标。
•基础集（例如:cc-pvtz）应该用来对分子进行离散化。
•系统的电荷和自旋多重性（如果已知）。
2.该研究人员使用openfermion-psi4插件或openfermion-pyscf插件来执行可扩展的经典计算，这些计算被用来优化量子计算。例如，一个人可能会执行经典的Hartree-Fock计算来为量子模拟选择一个好的初始状态。
3.然后，研究人员指定哪些电子是最适合在量子计算机上（称为活跃空间）进行研究的，并要求OpenFermion将这些电子的方程式映射到适用于量子位的表示，可以使用OpenFermion中可用的程序之一，例如Bravyi- Kitaev转换。
4.研究人员选择一种量子算法来解决某些特性，并用一种诸如OpenFermion-ProjectQ之类的量子编译框架，将量子电路以一种可以在量子计算机上运行汇编语言输出。如果研究人员可以访问量子计算机，那么他们就会执行这个实验。
关于我们能够用OpenFermion来做些什么呢？这里有一些可以在ipythonnotebook上演示了一些案例（https://github.com/quantumlib/OpenFermion/blob/master/examples/openfermion_demo.ipynb）。虽然量子模拟在短期内被广泛认为是量子计算中最重要的应用之一，但很少有量子计算机科学家熟知量子化学，而知道量子计算的化学家就更少了。我们的初衷就是希望OpenFermion能够帮助缩小这些群体之间的鸿沟，并为化学家和材料科学家呈现出量子计算的强大力量。
注：OpenFermion的主要功能是使用由各种基集和活动空间所定义的二次量子化方法对电子结构问题进行编码，然后使用量子比特与费米子代数之间的各种同构性将这些运算符转换为自旋哈密顿量（spin Hamiltonians）。
**OpenFermion**
OpenFermion是一个开放源代码，用于编译和分析量子算法来模拟费米子系统，包括量子化学。在其他功能中，当前版本的特征是数据结构和用于获取和操控费米子和量子位哈密顿算符的表示的工具。欲了解更多相关信息，请参阅我们发布的论文（https://arxiv.org/abs/1710.07629）和有关该项目的新闻稿（https://research.googleblog.com/2017/10/announcing-openfermion-open-source.html）。
**入门**
安装OpenFermion需要pip，确保你使用的是最新版本。有关入门其他信息，请参阅介绍：http://openfermion.readthedocs.io/en/latest/intro.html；
代码示例：http://openfermion.readthedocs.io/en/latest/examples.html；
ipython notebook演示：https://github.com/quantumlib/OpenFermion/blob/master/examples/openfermion_demo.ipynb；
详细的代码文件：http://openfermion.readthedocs.io/en/latest/openfermion.html。
目前，OpenFermion仅在Mac和Linux上进行测试，因为电子结构插件仅与Mac和Linux兼容。但是，对于那些想要使用Windows或者安装OpenFermion及其插件有困难的人来说，我们在docker文件夹（https://github.com/quantumlib/OpenFermion/tree/master/docker）中提供了一个Docker镜像和使用说明。Docker镜像为OpenFermion提供了一个虚拟环境，并选择了预先安装的插件。Docker安装系统可以在任何操作系统上运行。
**开发者安装**
要安装最新版本的OpenFermion（在开发模式下）：
git clone https://github.com/quantumlib/OpenFermion
cd OpenFermion
python -m pip install -e .
**库安装**
将最新的PyPI版本作为库（以用户模式）安装：
python -m pip install --user openfermion
**插件**
OpenFermion依赖于模块化插件库，以实现重要的功能。具体来说，插件用于模拟和编译量子电路并执行传统的电子结构计算。请查看下面的链接了解更多有用的插件信息。
**电路编译和仿真插件**
OpenFermion-ProjectQ：（http://github.com/quantumlib/OpenFermion-ProjectQ）支持与ProjectQ（https://projectq.ch/）的集成。
Forest-OpenFermion：（https://github.com/rigetticomputing/forestopenfermion）支持与Forest（https://www.rigetti.com/forest）的集成。
**电子结构包插件**
OpenFermion-Psi4：（http://github.com/quantumlib/OpenFermion-Psi4）支持与Psi4（http://psicode.org/）的集成（推荐）。
OpenFermion-PySCF：（http://github.com/quantumlib/OpenFermion-PySCF）支持与PySCF（https://github.com/sunqm/pyscf）的集成。
