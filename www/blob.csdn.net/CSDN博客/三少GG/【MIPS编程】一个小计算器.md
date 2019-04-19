# 【MIPS编程】一个小计算器 - 三少GG - CSDN博客
2010年10月05日 00:52:00[三少GG](https://me.csdn.net/scut1135)阅读数：1732

## [mips汇编简单实例——一个小计算器](http://see-see.appspot.com/?p=14001)
[http://see-see.appspot.com/?p=14001](http://see-see.appspot.com/?p=14001)
其实开始的时候一直在看 mips的指令格式，看了、忘了，没什么效果。
现在通过这个例子，简单训练一下： mips汇编中：
常用寄存器的使用；
系统调用： 1(print_int),4(print_str),5(read_int),10(exit)；
算术运算： add,sub,mul,div；
逻辑运算及跳转；
上代码：(汇编写的少，代码书写、风格还很差劲，感谢请大虾指教)：
#sample example 'a small calculater’
.text # text section
.globl main# call main by SPIM
main:
la $t0, value# load address "value" into $t0
la $a0,msg0
    li $v0,4    
    syscall#print "please choose the operation:"
li$v0, 5
syscall
sw$v0, 8($t0)
la $a0,msg1
    li $v0,4    
    syscall#print "first num:"
li$v0, 5
syscall
sw$v0, 0($t0)
la $a0,msg2
    li $v0,4    
    syscall#print " second num:"
li$v0, 5
syscall
sw$v0, 4($t0)#read the other num
la $a0,newline
    li $v0,4    
    syscall#print "/n"
lw $t1, 0($t0)# load the first num
lw $t2, 4($t0)# load the second num
lw $t3, 8($t0)# load the operation
beq $t3,1,addOp# if +
beq $t3,2,subOp# if -
beq $t3,3,mulOp# if *
beq $t3,4,divOp# if /
addOp:
add $t4, $t1, $t2# $t1 + $t2 = $t4
sw $t4, 12($t0)# 
la $t5,addFlag
j printResult
subOp:
sub $t4, $t1, $t2# $t1 - $t2 = $t4
sw $t4, 12($t0)
la $t5,subFlag
j printResult
mulOp:
mul $t4, $t1, $t2# $t1 * $t2 = $t4
sw $t4, 12($t0)
la $t5,mulFlag
j printResult
divOp:
div $t4, $t1, $t2# $t1 / $t2 = $t4
sw $t4, 12($t0)
la $t5,divFlag
j printResult
printResult:
lw $a0,0($t0)
    li $v0,1    
    syscall#read first number
la $a0,0($t5)
    li $v0,4 
    syscall#print opflag
lw $a0,4($t0)
    li $v0,1    
    syscall#print second number
la $a0,equalStr
    li $v0,4 
    syscall#print " = "
lw $a0,12($t0)
    li $v0,1    
    syscall# print sum result
j exit
exit:
la $a0,newline
    li $v0,4 
    syscall#print " /n "
    li $v0,10    
    syscall# exit
# data section
.data
value:.word 0, 0, 0 ,0 ,0   # 0: first num ,4 : second num , 8 : operation , 12:result
msg0:.asciiz " please choose the operation(1~4):/n/t/t1 : +,addition /n/t/t2 : -,subtracter/n/t/t3 : * multiplication /n/t/t4 : /,division/n"
msg1:.asciiz "first num:"
msg2:.asciiz "second num:"
addFlag: .asciiz " + "
subFlag: .asciiz " - "
mulFlag: .asciiz " * "
divFlag: .asciiz " / "
equalStr: .asciiz " = "
newline:.asciiz "/n===============================/n"
运行效果截图如下：
![](https://p-blog.csdn.net/images/p_blog_csdn_net/oncoding/EntryImages/20090714/demo_run.gif)
