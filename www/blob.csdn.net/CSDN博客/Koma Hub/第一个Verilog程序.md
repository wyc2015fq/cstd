# 第一个Verilog程序 - Koma Hub - CSDN博客
2019年02月23日 22:15:47[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：48
个人分类：[Verilog](https://blog.csdn.net/Rong_Toa/article/category/8694186)
一个Verilog程序
```
/*-------------------------------------------------------------------------
CONFIDENTIAL IN CONFIDENCE
This confidential and proprietary software may be only used as authorized
by a licensing agreement from CrazyBingo (Thereturnofbingo).
In the event of publication, the following notice is applicable:
Copyright (C) 2011-2012 CrazyBingo Corporation
The entire notice above must be reproduced on all authorized copies.
Author				:		CrazyBingo
Technology blogs 	: 		http://blog.chinaaet.com/crazybingo
							http://www.cnblogs.com/crazybingo
Eamil Address 		: 		thereturnofbingo@gmail.com
Filename			:		mult_4bits.v
Data				:		2012-11-01
Version				:		1.0
Description			:		This module is mult of 3 bits data.
Modification History	:
Data			By			Version			Change Description
===========================================================================
12/11/01		CrazyBingo	1.0				Original
--------------------------------------------------------------------------*/
`timescale 1 ns / 1 ns
module	mult_4bits
(
	input	[3:0]	x,
	input	[3:0]	y,
	
	output	[7:0] mult_out
);
wire	[3:0]	temp0 = y[0] ? x : 4'd0;
wire	[3:0]	temp1 = y[1] ? x : 4'd0;
wire	[3:0]	temp2 = y[2] ? x : 4'd0;	
wire	[3:0]	temp3 = y[3] ? x : 4'd0;
assign mult_out = temp0 + (temp1 << 1) + (temp2 << 2) + (temp3 << 3);
endmodule
```
