
# 向HTCondor集群提交LS-DYNA任务 - cocoonyang的专栏 - CSDN博客


2015年11月18日 08:52:54[cocoonyang](https://me.csdn.net/cocoonyang)阅读数：1970



submit文件
```python
####################                                                    
  # 
  # Example 1 - LS-DYNA                                                            
  # Simple HTCondor submit description file                                    
  #                                                                       
  ####################                                                    
 
                                                                         
  Executable    =  E:\\LSDYNAR7\\program\\ls-dyna_smp_d_R700_winx64_ifort101.exe   
  universe      = vanilla 
 
  input        = input.txt 
  should_transfer_files = YES
  transfer_input_files = LSTC_FILE
  when_to_transfer_output = ON_EXIT
  output       = output.$(cluster).$(process).txt
  log          = log.$(cluster).$(process).txt
  queue
```
input文件内容
```python
I=I:\\HTCondor\\foo\\foo-airbag.k   O=I:\\HTCondor\\foo\\result.txt
```


或者上述两个文件的内容合并为一,   则submit文件为：

```python
####################                                                    
  # 
  # Example 1 - LS-DYNA                                                            
  # Simple HTCondor submit description file                                    
  #                                                                       
  ####################                                                    
 
                                                                         
  Executable    =  E:\\LSDYNAR7\\program\\ls-dyna_smp_d_R700_winx64_ifort101.exe   
  universe      = vanilla 
 
  input        = 'I=I:\\HTCondor\\foo\\foo-airbag.k   O=I:\\HTCondor\\foo\\result.txt'
  should_transfer_files = YES
  transfer_input_files = LSTC_FILE
  when_to_transfer_output = ON_EXIT
  output       = output.$(cluster).$(process).txt
  log          = log.$(cluster).$(process).txt
  queue
```


抑或将foo-airbag.k作为transfer files，则submit文件为：

```python
####################                                                    
  # 
  # Example 1 - LS-DYNA                                                            
  # Simple HTCondor submit description file                                    
  #                                                                       
  ####################                                                    
 
                                                                         
  Executable    =  E:\\LSDYNAR7\\program\\ls-dyna_smp_d_R700_winx64_ifort101.exe   
  universe      = vanilla 
 
  input        = 'I=foo-airbag.k   O=I:\\HTCondor\\foo\\result.txt'
  should_transfer_files = YES
  transfer_input_files = LSTC_FILE, foo-airbag.k
  when_to_transfer_output = ON_EXIT
  output       = output.$(cluster).$(process).txt
  log          = log.$(cluster).$(process).txt
  queue
```





运行结果 result.txt 文件内容
```python
Executing with LSTC local workstation license
...
```
```python
*** termination time reached ***
   19862 t 3.0000E-02 dt 1.51E-06 write d3dump01 file          11/18/15 08:24:40
   19862 t 3.0000E-02 dt 1.51E-06 write d3plot file            11/18/15 08:24:40
 N o r m a l    t e r m i n a t i o n                          11/18/15 08:24:40
 Memory required to complete solution   :       760534
 Linear Alg dynamically allocated memory:           42
 Additional dynamically allocated memory:      1826921
                                   Total:      2587497
 T i m i n g   i n f o r m a t i o n
                        CPU(seconds)   %CPU  Clock(seconds) %Clock
  ----------------------------------------------------------------
  Keyword Processing ... 0.0000E+00    0.00     6.0000E-02    0.12
    KW Reading ......... 0.0000E+00    0.00     2.2000E-02    0.04
    KW Writing ......... 0.0000E+00    0.00     2.4000E-02    0.05
  Initialization ....... 0.0000E+00    0.00     1.6800E-01    0.33
    Init Proc Phase 1 .. 0.0000E+00    0.00     7.3000E-02    0.14
    Init Proc Phase 2 .. 0.0000E+00    0.00     1.8000E-02    0.04
  Element processing ... 3.5000E+01   68.63     2.9185E+01   56.83
    Shells ............. 3.5000E+01   68.63     2.9129E+01   56.72
  Binary databases ..... 0.0000E+00    0.00     2.5900E-01    0.50
  ASCII database ....... 0.0000E+00    0.00     3.5000E-02    0.07
  Contact algorithm .... 1.1000E+01   21.57     1.5327E+01   29.84
    Interf. ID         1 1.1000E+01   21.57     1.5270E+01   29.73
  Rigid Bodies ......... 0.0000E+00    0.00     4.4500E-01    0.87
  Other ................ 5.0000E+00    9.80     5.8790E+00   11.45
  ----------------------------------------------------------------
  T o t a l s            5.1000E+01  100.00     5.1358E+01  100.00
 Problem time       =    3.0000E-02
 Problem cycle      =     19862
 Total CPU time     =        51 seconds (   0 hours  0 minutes 51 seconds)
 CPU time per zone cycle  =       676 nanoseconds
 Clock time per zone cycle=       678 nanoseconds
 Number of CPU's    1
 NLQ used/max     136/  136
 Start time   11/18/2015 08:23:49  
 End time     11/18/2015 08:24:40  
 Elapsed time      51 seconds(   0 hours  0 min. 51 sec.) for   19862 cycles
 N o r m a l    t e r m i n a t i o n
```







