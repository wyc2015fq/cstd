# FPGA入门实验 - situheyu的博客 - CSDN博客
2017年11月19日 21:55:35[司徒合羽](https://me.csdn.net/situheyu)阅读数：575
**实验一**
**用1个拨码开关控制所有的LED灯亮灭**
画原理图： 
![这里写图片描述](https://img-blog.csdn.net/20171119214554306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119214610143?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
定义管脚： 
![这里写图片描述](https://img-blog.csdn.net/20171119214634870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结果图： 
![这里写图片描述](https://img-blog.csdn.net/20171119214740553?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119214752064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**实验二**
**1、放置2个2-4译码器模块，则总共有2组SW，每组2个，2组LED，每组4个，每组SW分别控制其对应的LED组。 
•2、参照代码，设计一个3-8译码器，完成类似的拨码开关实验。注意代码中的信号宽度设定。 
•3、自行查阅手册中的7段译码器管脚对应关系，用4个拨码开关控制一个7段译码器的数字，从0-9-A-F，共16个数字和字母**
**1**
![这里写图片描述](https://img-blog.csdn.net/20171119215244954?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215257782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215311578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215337106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215348507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215403752?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215415698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119215424485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2**
**参照代码，设计一个3-8译码器，完成类似的拨码开关实验。注意代码中的信号宽度设定。**
![这里写图片描述](https://img-blog.csdn.net/20171119225508493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225528953?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225541695?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225601225?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225613368?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225624536?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225637483?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225707526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225718703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225732068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171119225745286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3**
**自行查阅手册中的7段译码器管脚对应关系，用4个拨码开关控制一个7段译码器的数字，从0-9-A-F，共16个数字和字母****
![这里写图片描述](https://img-blog.csdn.net/20171120113239700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120113249948?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120113259361?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140537165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140549859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**实验三**
**1、参照代码，设计一个0-17的计数器，当计数值为17的时候，OV输出1，其他输出0，注意设定合理的信号位宽。 
•2、针对以上计数器，修改输出逻辑，当计数值为0-8时，OV输出0,9-17时OV输出1 
•用SignalTap验证 
•尝试并思考，如果时钟是50MHz，把OV接到一个LED上，能看见什么现象，为什么？**
**1**
**参照代码，设计一个0-17的计数器，当计数值为17的时候，OV输出1，其他输出0，注意设定合理的信号位宽。**
![这里写图片描述](https://img-blog.csdn.net/20171120140617718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140626850?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140635936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140649055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140703330?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120140712511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2**
**针对以上计数器，修改输出逻辑，当计数值为0-8时，OV输出0,9-17时OV输出1 
•用SignalTap验证** 
![这里写图片描述](https://img-blog.csdn.net/20171120142627231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120142638200?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120142646318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**tools->netlist viewers->RTL viewer**
![这里写图片描述](https://img-blog.csdn.net/20171120142655793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120142705375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**tools->signaltap II logic analyzer**
![这里写图片描述](https://img-blog.csdn.net/20171120142714292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**•尝试并思考，如果时钟是50MHz，把OV接到一个LED上，能看见什么现象，为什么？**
**实验四**
**1、参照代码，把后级计数器的计数范围改为0-15。 
.2、把计数器的0-15计数值经过译码，在DE0 的 HEX LED上显示成0-9-A-F的十六 进制数 
.3、把计数器的0-15计数值经过译码，在DE0 的 HEX LED上显示成0-9-A-F的十六 进制数 
.4、自行设计上面计时器的附加控制功能（清零、暂停）。 
17**
**1、参照代码，把后级计数器的计数范围改为0-15。**
![这里写图片描述](https://img-blog.csdn.net/20171120214229369?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214239797?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214248278?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214256167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214304770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2**
**把计数器的0-15计数值经过译码，在DE0 的 HEX LED上显示成0-9-A-F的十六 进制数**
![这里写图片描述](https://img-blog.csdn.net/20171120214354625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214402170?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214409886?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214418381?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214427903?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214436970?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214444663?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214453211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214502699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3**
**把计数器的0-15计数值经过译码，在DE0 的 HEX LED上显示成0-9-A-F的十六 进制数**
module cnt_sync( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  OV    );  // overflow 
input CLK; 
output [32-1:0] CNTVAL; 
output OV; 
parameter MAX_VAL = 5_000_000; 
reg [32-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(CNTVAL >= MAX_VAL) 
    CNTVAL <= 0; 
  else 
    CNTVAL <= CNTVAL + 1’b1; 
end
always @ (CNTVAL) begin 
  if(CNTVAL == MAX_VAL) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end
endmodule 
////////////////////////////////////////////////////////////////////////////////
module cnt_en_0to99( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  EN    , 
  OV    );  // overflow 
input CLK; 
input EN; 
output [8-1:0] CNTVAL; 
output OV;
reg [8-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(EN) begin  // work enable 
    if(CNTVAL >= 99) 
      CNTVAL <= 0; 
    else 
      CNTVAL <= CNTVAL + 1’b1; 
  end 
  else 
    CNTVAL <= CNTVAL ;  // hold same value 
end
always @ (CNTVAL) begin 
  if(CNTVAL == 99) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end
endmodule   // module cnt_en_0to99
//////////////////////////////////////////////////////////////////////////////// 
module dec4_16(out,in); 
   output [8-1:0] out; 
   input [8-1:0] in; 
   reg [8-1:0] out; 
   always @(in) 
   begin 
     case(in / 10) 
     4’d0:  out=8’b01000000; 
     4’d1:  out=8’b01111001; 
     4’d2:  out=8’b00100100; 
     4’d3:  out=8’b00110000; 
     4’d4:  out=8’b00011001; 
     4’d5:  out=8’b00010010; 
     4’d6:  out=8’b00000010; 
     4’d7:  out=8’b01111000; 
     4’d8:  out=8’b00000000; 
     4’d9:  out=8’b00010000; 
     endcase 
   end 
 endmodule 
//////////////////////////////////////////////////////////////////////////////// 
module dec4_16L(outl,in); 
   output [8-1:0] outl; 
   input [8-1:0] in; 
   reg [8-1:0] outl; 
   always @(in) 
   begin 
     case(in % 10) 
     4’d0:  outl=8’b11000000; 
     4’d1:  outl=8’b11111001; 
     4’d2:  outl=8’b10100100; 
     4’d3:  outl=8’b10110000; 
     4’d4:  outl=8’b10011001; 
     4’d5:  outl=8’b10010010; 
     4’d6:  outl=8’b10000010; 
     4’d7:  outl=8’b11111000; 
     4’d8:  outl=8’b10000000; 
     4’d9:  outl=8’b10010000; 
     endcase 
   end 
 endmodule
![这里写图片描述](https://img-blog.csdn.net/20171120214608071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214616667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214625256?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214633735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171120214643047?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**4**
**自行设计上面计时器的附加控制功能（清零、暂停）**
//0.1S 
module cnt_sync_1( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  OV    );  // overflow 
input CLK; 
output [32-1:0] CNTVAL; 
output OV; 
parameter MAX_VAL = 5_000_000; 
reg [32-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(CNTVAL >= MAX_VAL) 
    CNTVAL <= 0; 
  else 
    CNTVAL <= CNTVAL + 1’b1; 
end
always @ (CNTVAL) begin 
  if(CNTVAL == MAX_VAL) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end 
endmodule 
//1S 
module cnt_sync_2( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  OV    );  // overflow 
input CLK; 
output [32-1:0] CNTVAL; 
output OV; 
parameter MAX_VAL = 50_000_000; 
reg [32-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(CNTVAL >= MAX_VAL) 
    CNTVAL <= 0; 
  else 
    CNTVAL <= CNTVAL + 1’b1; 
end
always @ (CNTVAL) begin 
  if(CNTVAL == MAX_VAL) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end 
endmodule 
// module cnt_en_0to9 
////////////////////////////////////////////////////////////////////////////////
module cnt_en_0to9( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  reset,    //clear to 0 
  EN    , 
  OV    );  // overflow 
input CLK,reset; 
input EN; 
output [4-1:0] CNTVAL; 
output OV;
reg [4-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
    if(reset) 
    CNTVAL <= 0; 
    else begin
if(EN==0) begin  // work enable 
    if(CNTVAL >= 9) 
      CNTVAL <= 0; 
    else 
      CNTVAL <= CNTVAL + 1’b1; 
  end 
  else 
    CNTVAL <= CNTVAL ;  // hold same value 
end 
end
always @ (CNTVAL) begin 
  if(CNTVAL == 9) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end
endmodule   // module cnt_en_0to9
//////////////////////////////////////////////////////////////////////////////// 
module bcd7seg (Y,A);  
 input [3:0] A ; 
 wire [3:0] A ; 
 output [6:0] Y ; 
 wire [6:0] Y ; 
 assign Y =~Y_r; 
 reg [6:0] Y_r;  
     always @(A)begin 
         Y_r = 7’b1111111; 
         case (A ) 
                 4’b0000: Y_r = 7’b0111111; // 0 
                 4’b0001: Y_r = 7’b0000110; // 1 
                 4’b0010: Y_r = 7’b1011011; // 2 
                 4’b0011: Y_r = 7’b1001111; // 3 
                 4’b0100: Y_r = 7’b1100110; // 4 
                                4’b0101: Y_r = 7’b1101101; // 5 
                 4’b0110: Y_r = 7’b1111101; // 6 
                 4’b0111: Y_r = 7’b0000111; // 7 
                 4’b1000: Y_r = 7’b1111111; // 8 
                 4’b1001: Y_r = 7’b1101111; // 9 
                 4’b1010: Y_r = 7’b1110111; // A 
                 4’b1011: Y_r = 7’b1111100; // b 
                 4’b1100: Y_r = 7’b0111001; // c 
                 4’b1101: Y_r = 7’b1011110; // d 
                 4’b1110: Y_r = 7’b1111001; // E 
                 4’b1111: Y_r = 7’b1110001; // F 
                 default: Y_r = 7’b0000000; 
             endcase 
     end 
 endmodule 
  // bcd7seg (Y,A)
![这里写图片描述](https://img-blog.csdn.net/20171121200455108?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171121200507089?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171121200516393?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171121200527039?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171121200544449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171121200552992?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**实验五**
**编译下载电路观察现象 • 拨动SW0开关，观察LED的闪烁变换情况 • 设计新的功能 – 在原有的电路基础上，添加方向选择功能。 – SW0仍然是移位寄存器组的输入 – 使用SW1开关，控制移位寄存的方向 – 从实验现象上应当能够看到，SW1可以控制LED闪烁的移动方向，以及控制SW0值的对LED组的输入位置（即从LED组的最左边或是最右边）**
//////////////////// 串入并出移位寄存器  ///////////////////////// 
module shift_reg_SIPO( 
  RST   ,   // 异步复位， 高有效 
  CLK   ,   // 时钟，上升沿有效 
  EN    ,   // 输入数据串行移位使能 
  IN    ,   // 输入串行数据 
  OUT   , 
  CHOSS );  // 并行输出数据 
parameter SHLEN = 6; 
input RST, CLK, EN,CHOSS; 
input IN; 
output[SHLEN-1:0] OUT; 
reg [SHLEN-1:0] shift_R;
assign OUT[SHLEN-1:0] = shift_R[SHLEN-1:0]; 
// 时序逻辑 根据输入使能进行串行移位 
// shift_R 会被编译为D触发器 
always @ (posedge CLK or posedge RST) begin 
  if(RST)  
    shift_R[SHLEN-1:0] <= 0; 
  else 
    if(EN&CHOSS) begin // 串行移位的使能有效 
      shift_R[SHLEN-1:1] <= shift_R[SHLEN-2:0]; 
      shift_R[0]   <= IN; 
    end
```
else if(EN&CHOSS==0) begin // 串行移位的使能有效
  shift_R[SHLEN-2:0] <= shift_R[SHLEN-1:1];
  shift_R[SHLEN-1]   <= IN;
end
else begin // 使能无效保持不动
  shift_R[SHLEN-1:0] <= shift_R[SHLEN-1:0];
end
```
end // always 
endmodule
//////////////////// 时间基准计数器  ///////////////////////// 
module cnt_sync( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  OV    );  // overflow 
input CLK; 
output [32-1:0] CNTVAL; 
output OV; 
parameter MAX_VAL = 25_000_000; 
reg [32-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(CNTVAL >= MAX_VAL) 
    CNTVAL <= 0; 
  else 
    CNTVAL <= CNTVAL + 1’b1; 
end
always @ (CNTVAL) begin 
  if(CNTVAL == MAX_VAL) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end
endmodule   // module cnt_en_0to9 
////////////////////////////////////////////////////////////////////////////////
module cnt_en_0to9( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  EN    , 
  OV    );  // overflow 
input CLK; 
input EN; 
output [4-1:0] CNTVAL; 
output OV;
reg [4-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(EN) begin  // work enable 
    if(CNTVAL >= 9) 
      CNTVAL <= 0; 
    else 
      CNTVAL <= CNTVAL + 1’b1; 
  end 
  else 
    CNTVAL <= CNTVAL ;  // hold same value 
end
always @ (CNTVAL) begin 
  if(CNTVAL == 9) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end
endmodule   // module cnt_en_0to9 
//////////////////////////////////////////////////////////////////////////////// 
module cnt_0to9( 
  CLK   ,   // clock 
  CNTVAL,   // counter value 
  OV    );  // overflow 
input CLK; 
output [4-1:0] CNTVAL; 
output OV;
reg [4-1:0] CNTVAL; 
reg OV;
always @ (posedge CLK) begin 
  if(CNTVAL >= 9) 
    CNTVAL <= 0; 
  else 
    CNTVAL <= CNTVAL + 1’b1; 
end
always @ (CNTVAL) begin 
  if(CNTVAL == 9) 
    OV = 1’b1; 
  else 
    OV = 1’b0; 
end
endmodule   // module cnt_0to9 
//////////////////////////////////////////////////////////////////////////////// 
module dec_2to4( 
IN  , 
OUT); 
input  [2-1:0] IN  ; 
output [4-1:0] OUT ;
reg [4-1:0] OUT ;
always @ (IN) begin 
  case(IN) 
    2’b00: OUT = 4’b 0001; 
    2’b01: OUT = 4’b 0010; 
    2’b10: OUT = 4’b 0100; 
    2’b11: OUT = 4’b 1000; 
  endcase 
end
endmodule // module dec_2to4;
![这里写图片描述](https://img-blog.csdn.net/20171123184654114?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171123184702032?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171123184709367?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**结果是，由于不能拍视频演示，用文字描述如下 
当不拨动sw1时，拨上开关sw0,LED灯从左到右依次亮起，LEDG5->LEDG0 
                            把开关sw0拨下来，灯从左到右依次熄灭。 
当把Sw0拨上去时，情况恰好相反。保持Sw0拨上去的状态，把sw1拨上去，LED灯从右到左依次亮起，把开 
                                           关sw1拨下来，LED灯从右到左依次熄灭。** 
**另附上几张结果图****
![这里写图片描述](https://img-blog.csdn.net/20171123193030277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171123193040712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171123193049251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171123193058580?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**实验六**
**设计直接数字频率合成（DDS）  
要求：  
–用计数器生成地址、读取ROM数据  
–理解二进制补码和无符号数  
–修改计数增量值，观察波形变化，思考输出频率  
和计数器增量值的关系。**
根据参考代码，没有改动，新建工程等等，这里不再赘述。 
![这里写图片描述](https://img-blog.csdn.net/20171125140425956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125140443479?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125140452793?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125140500521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125140510228?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**计数器模块内部电路结构：**
![这里写图片描述](https://img-blog.csdn.net/20171125140544277?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**ROM内部电路结构：**
![这里写图片描述](https://img-blog.csdn.net/20171125140608581?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
【思考】：如果加法器溢出了，输出结果会怎样？  
【答】：当相位累加器的溢出时，—个完整的阶梯正弦信号就从ROM查询表的输出端输出。因此，相位累加器的溢出率就是正弦波信号的输出频率。
**不同的计数增量值对应的波形**
**1  计数增量为1时，正弦波频率约0.39 MHZ**
![这里写图片描述](https://img-blog.csdn.net/20171125140747352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**2  计数增量为2时，正弦波频率约0.78 MHZ**
![这里写图片描述](https://img-blog.csdn.net/20171125140849811?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**3  计数增量为8时，正弦波频率3.125MHZ**
![这里写图片描述](https://img-blog.csdn.net/20171125141015731?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**(注：计数器的增量由开关决定，共有128种组合)**
**问题一**    拨动开关，观察不同频率的正弦波， 电路的工作时钟是50MHz  
（1） 请回答，你能得到的正弦波频率和计数器增量值的对应关系是什么？  
（2）请回答，你能得到的最低频率的正弦波是多少？设该频率为f1  
（3）请思考，能否有什么方法能够得到比f1频率还低的正弦波  
【答】：  
（1）正弦波频率和计数器增量值成 线性关系。  
（2）正弦波的最低频率f1约0.39 MHZ。  
（3）增加计数器增量或减小计数器数据位宽。
**问题二**
直接数字频率合成（DDS） 
– 请给出：输出信号频率 和 电路工作时钟频率，计数器增量值，以及计数器数据位宽 
之间的表达式关系。** 
![这里写图片描述](https://img-blog.csdn.net/20171125120853317?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**学生实验：*****设计直接数字频率合成（DDS）*  
**要求：  
–把计数增量输入信号分配到10个拨码开关上  
–把计数值信号的高7位分配为ROM的地址，低3位悬空不使用。  
–编译、下载电路，拨动拨码开关，观察最低频率有什么变化。**
**由于要求是把计数增量输入信号分配到10个拨码开关上 ，所以把计数器的输入端口即开关从7位改成10位即可，其余代码不变，这里也不再赘述。见下图。**
![这里写图片描述](https://img-blog.csdn.net/20171125152026852?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125152225129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125152237713?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125152245214?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20171125152253673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**计数器模块内部电路结构：**
![这里写图片描述](https://img-blog.csdn.net/20171125152304118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**ROM内部电路结构：**
![这里写图片描述](https://img-blog.csdn.net/20171125152313517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**最终结果图：计数增量为03Fh（十六进制）时，即为63时，根据公式，正弦波频率约3.076MHZ**
![这里写图片描述](https://img-blog.csdn.net/20171125152321316?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**以上为FPGA的六个基础实验，望批评指正。**
以上实验采用的芯片均为Altera公司的Cyclone III  芯片。 
2015年6月2日，Altera公司被英特尔以167亿美元价格收购。总之，英特尔认为FPGA会无处不在。在网络世界，FPGA已经在蜂窝基站中亮相，英特尔可能希望借助FPGA在这一领域获得更多的市场占有率。
![这里写图片描述](https://img-blog.csdn.net/20171125153821926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2l0dWhleXU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
