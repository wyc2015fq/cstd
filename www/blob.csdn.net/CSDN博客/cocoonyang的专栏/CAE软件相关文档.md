# CAE软件相关文档 - cocoonyang的专栏 - CSDN博客





2015年12月13日 22:12:23[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1418








# **Nastran **

Simple Tips to Speed up NX Nastran Simulations[1]


# Abaqus 

Abaqus帮助文档

[2] 提供 Abaqus V6.12 英文版帮助文档



# Ls_Dyna 

[3] "The most common contact-related output file, RCFORC, is produced by including a *DATABASE_RCFORC command in the input deck. 

RCFORC is an ASCII file containing resultant contact forces for the slave and master sides of each contact interface. The forces are written in the global coordinate system. 

Note that RCFORC data is not written for single surface contacts as all the contact forces from such a contact come from the slave side (there is no master side) and thus the net contact forces are zero. To obtain RCFORC data when single surface contacts are used, one or more force transducers should be added via the *CONTACT_FORCE_TRANSDUCER_PENALTY command. A force transducer does not produce any contact forces and thus does not affect the results of the simulation. A force transducer simply measures contact forces produced by other contact interfaces defined in the model. One would typically assign a subset of the parts defined in a single surface contact to the slave side of a force transducer. No master side is defined. The RCFORC file would then report the resultant contact forces on that subset of parts. The ASCII output file NCFORC reports contact forces at each node. The command *DATABASE_NCFORC is required in the input deck to produce such a file. Further, one or more contact print flags must be set (see SPR and MPR on Card 1 of *CONTACT). Only those surfaces whose print flag is set to a value of 1 will have their nodal contact force output to the NCFORC file. By including a *DATABASE_SLEOUT command, contact interface energies are written to the ASCII ouput file SLEOUT. In cases where there are two or more contact interfaces in a model and the global statistics file (GLSTAT) indicates a problem with contact energy, such as a large negative value, the SLEOUT file is useful for isolating which contact interfaces are responsible. For general information on interpreting contact energies, see the LS-DYNA Theory Manual, Section 23.8.4. In some cases, it can be very useful to visualize contact surfaces and produce fringe plots of contact stress both in directions normal and tangential to the contact surface. To do this, a binary interface file must be written by (1) including a *DATABASE_BINARY_INTFOR command in the input deck, (2) setting one or more contact print flags as detailed above, and (3) including the option s=filename on the LS-DYNA execution line where filename is the intended name of the binary database. The database can be postprocessed using LS-POST."



美国的国家公路交通安全管理局(National Highway Traffic Safety Administration)提供LS_DYNA格式数值仿真碰撞试验用汽车有限元模型[4].

[5] 搜集的Ls_Dyna资源

LSTC公司：http://www.lstc.com/

LSTC公司ftp：http://ftp.lstc.com/user/

用户名：user

密码：computer


http://portal.ecadfem.com

http://www.dynaexamples.com/

http://www.ncac.gwu.edu/vml/models.html

http://www.dynalook.com/

http://www.dynamore.de/download/con_download_manuals.html

ECE法规的下载网址：http://www.unece.org/trans/main/wp29/wp29regs.html

http://www.oasys-software.com/dyna/en/downloads/tutorials.shtml 

altair公司ftp网址

ftp://ftp.altair.com.cn/pub/outgoing/HyperWorks_Turtorial_Video

ftp://ftp.altair.com.cn/priv/support/hw12.0/

altair公司的一些很有用的tcl程序分享。

http://www.altairhyperworks.com/ClientCenterLoginForm.aspx?top_nav_str=1&from_page=%2fScript-Exchange.aspx%3f



# 论文免费下载网站

1)  Sci-Hub 

sci-hub.rs

sci-hub.cc

sci-hub.bz

sci-hub.ac

sci-hub.io

sci-hub.cn

sci-hub.xyz

sci-hub.org.cn

2)  Library Genesis

[http://gen.lib.rus.ec/](http://gen.lib.rus.ec/)

3) BookSC 

[http://booksc.org/](http://booksc.org/)



# 学术搜索引擎

Google学术：你知道的...( [https://scholar.google.com.hk/](https://scholar.google.com.hk/) )

bing学术：[http://cn.bing.com/academic](http://cn.bing.com/academic)

百度学术：[http://xueshu.baidu.com/](http://xueshu.baidu.com/)





# Reference 

[1] http://www.engineering.com/DesignSoftware/DesignSoftwareArticles/ArticleID/9222/Simple-Tips-to-Speed-up-NX-Nastran-Simulations.aspx

[2] http://things.maths.cam.ac.uk/computing/software/abaqus_docs/docs/v6.12/pdf_books/

[3] http://www.dynasupport.com/tutorial/contact-modeling-in-ls-dyna/contact-output

[4] https://www.nhtsa.gov/crash-simulation-vehicle-models 

[5] http://test.caenet.cn/space/viewspacepost.aspx?postid=4188  

[6] Mechanics of Fibre-Reinforced Composites. https://www.doitpoms.ac.uk/tlplib/fibre_composites/index.php









