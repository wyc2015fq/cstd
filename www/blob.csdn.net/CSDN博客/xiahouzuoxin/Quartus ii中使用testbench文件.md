# Quartus ii中使用testbench文件 - xiahouzuoxin - CSDN博客





2014年02月18日 14:20:29[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：17904








*Quartus ii版本是13.01，原工程文件名为**ex**，Quartus要求最顶层.v文件名要与工程名相同，因此顶层.v文件名为**ex.v***



**==== Step1 ====**

Processing -> Start -> Start Testbench Template Writer生成针对工程的Testbench模板文件。

使用File -> Open打开在工程的simulation目录下的Testbench文件**ex.vt**，其内容如下：

`timescale 1 ps/ 1 ps
**module ex_vlg_tst();**

// constants                                           

// general purpose registers

reg eachvec;

// test vector input registers

reg a;

reg b;

// wires                                               

wire equal;



// assign statements (if any)                          
**ex i1 **(

// port map - connection between master ports and signals/registers   
.a(a),
.b(b),
.equal(equal)

);

initial                                                

begin                                                  

// code that executes only once                        

// insert code here --> begin                          


// --> end                                             

$display("Running testbench");                       

end                                                    

always                                                 

// optional sensitivity list                           

// @(event1 or event2 or .... eventn)                  

begin                                                  

// code executes for every event on sensitivity list   

// insert code here --> begin                          


@eachvec;                                              

// --> end                                             

end                                                    

endmodule


修改.vt文件满足自己需求。




**==== Step2 ====**


执行Assigments -> Settings -> EDA Tool Settings -> Simulation设置仿真参数

![](https://img-blog.csdn.net/20140218141302812)


![](https://img-blog.csdn.net/20140218141750484)


一路OK之后，执行Tools -> Run Simulation Tools -> RTL simulation则弹出Modelsim的仿真界面，自此就可以进行仿真分析了。




关于设置仿真工具路径的方法参见[关于Quartus
 ii无法识别Modelsim路径的问题](http://blog.csdn.net/xiahouzuoxin/article/details/19406855)






