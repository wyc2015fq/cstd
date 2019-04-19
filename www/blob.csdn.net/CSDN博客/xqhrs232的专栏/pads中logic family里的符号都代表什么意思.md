# pads中logic family里的符号都代表什么意思 - xqhrs232的专栏 - CSDN博客
2013年07月14日 23:55:24[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：2569
原文地址::[http://zhidao.baidu.com/question/450764306.html](http://zhidao.baidu.com/question/450764306.html)
相关网帖
1、Pads的Logic Family----[http://padslayout.com/2011/09/pads-logic-family/](http://padslayout.com/2011/09/pads-logic-family/)

对于相同的Part type的要求：必须使用标准库中的Part type以保证有相同的Part type Name。
特别指出：相同的Part type包括相同的Logic Family。Logic Family信息在Part Eidtor的环境下，Part Information for Part的对话框的General的面板中。
Family的信息有：ANA、BGA、BPF、BQF、CAP、CFP、CLC、CMO、CON、CQF、DIO、DIP、ECL、EDG、FUS、 HMO、HOL、IND、LCC、MOS、OSC、PFP、PGA、PLC、POT、PQF、PSO、QFJ、QFP、QSO、RES、RLY、SCR、 SKT、SOI、SOJ、SOP、SSO、SWI、TQF、TRX、TSO、TTL、VSO、XFR、ZEN。如果一个器件的Logic  Family的信息不同，在不同的PCB文件之间是无法reuse的。造成很多的工作无法重复利用.
Family(逻辑族)    中英文大意   
ANA          
BGA   ball grid array球栅阵列封装      
BPF      
BQF            
CAP   CAPACITOR电容器     
CFP   CFP(陶瓷扁平封装     
CLC       
CMO          
CON   CONIN连接器   
CQF           
DIO   DIODE(二极管      
DIP   Dual In-line Package双列直插式组件        
ECL           
EDG          
FUS   FUSE保险丝        
HMO         
HOL         
IND   INDUCTANCE电感      
LCC   Leadless chip carrier无引脚片式载体   
MOS   Metal Oxide Semiconductor金属氧化物半导体   
OSC   Open Source Commerce振荡器      
PFP            
PGA   butt joint pin grid array碰焊 (pin grid array)         
PLC           
POT   POTENTIOMETER可变电阻器        
PQF            
PSO           
QFJ   CLCC(ceramic leaded chip carrier)也称QFJ,QFJ-G         
QFP   quad flat package四侧引脚扁平封装        
QSO            
RES   Resistor电阻器         
RLY   RLY继电器         
SCR   Silicon Controlled Rectifier可控硅         
SKT            
SOI   small out-line I-leaded packageI形引脚小外型封装         
SOJ   Small Out-Line J-Leaded Package(J形引脚小外型封装)         
SOP   small Out-Line package(小外形封装)         
SSO            
SWI   SWITCH开关        
TQF           
TRX   Transistor(三极管        
TSO           
TTL   Transistor-Transistor Logic(BJT-BJT逻辑门      
VSO           
XFR   XFMR变压器         
ZEN   ZENER齐纳二极管       
UND           
增加           
SIP   single in-line package直插式组件  
        photoelectric coupler(光电耦合器       
LED   Light Emitting Diode发光二极管  
TVS   Transient Voltage Suppressor(瞬态电压抑制二极管)   
FB   Ferrite bead(磁珠)        
TP   TEST POINT(测试点       
MIC   MICROPHONE (麦克风)        
BQFP   BQFP(quad flat package with bumper)带缓冲垫的四侧引脚扁平封装   
CLCC   ceramic leaded chip carrier(带引脚的陶瓷芯片载体)   
COB   chip on board(板上芯片封装)         
DFP   dual flat package(双侧引脚扁平封装)(是SOP 的别称)         
FP   flat package(扁平封装)         
FQFP   fine pitch quad flat package(小引脚中心距QFP)       
CQFP   quad fiat package with guard ring(带保护环的四侧引脚扁平封装)        
HSOP   H-(with heat sink)HSOP 表示带散热器的SOP         
LQFP   low profile quad flat package(薄型QFP)        
SMD   surface mount devices(表面贴装器件)         
CPGA    Ceramic Pin Grid Array       
ZIP   Zig-Zag Inline Package 之字型直插式封装        
TSOP    Thin Small Outline Package       
TSSOP    TSOP II Thin Shrink Outline  Package//=====================================================================备注：：1》怎么查看元器件在原理图里面被引用的前缀----logic family/Families按钮会显示2》也可以增加自己的家族及其被引用的前缀
