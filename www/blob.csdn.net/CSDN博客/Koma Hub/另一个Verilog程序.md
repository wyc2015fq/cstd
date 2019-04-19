# 另一个Verilog程序 - Koma Hub - CSDN博客
2019年02月23日 22:17:43[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：42
```
/*-------------------------------------------------------------------------
This confidential and proprietary software may be only used as authorized
by a licensing agreement from CrazyBingo.
(C) COPYRIGHT 2012 CrazyBingo. ALL RIGHTS RESERVED
Filename			:		Seg7_lut.v
Author				:		CrazyBingo
Data				:		2012-05-13
Version				:		1.0
Description			:		sdram test with uart interface.
Modification History	:
Data			By			Version			Change Description
===========================================================================
12/05/13		CrazyBingo	1.3				Modification
--------------------------------------------------------------------------*/
module Seg7_lut	
(	
	input		[3:0]	iDIG,
	output	reg	[6:0]	oSEG
);
always @(iDIG)
begin
		case(iDIG)
		4'h1: oSEG = 7'b1111001;	// ---t----
		4'h2: oSEG = 7'b0100100; 	// |	  |
		4'h3: oSEG = 7'b0110000; 	// lt	 rt
		4'h4: oSEG = 7'b0011001; 	// |	  |
		4'h5: oSEG = 7'b0010010; 	// ---m----
		4'h6: oSEG = 7'b0000010; 	// |	  |
		4'h7: oSEG = 7'b1111000; 	// lb	 rb
		4'h8: oSEG = 7'b0000000; 	// |	  |
		4'h9: oSEG = 7'b0011000; 	// ---b----
		4'ha: oSEG = 7'b0001000;
		4'hb: oSEG = 7'b0000011;
		4'hc: oSEG = 7'b1000110;
		4'hd: oSEG = 7'b0100001;
		4'he: oSEG = 7'b0000110;
		4'hf: oSEG = 7'b0001110;
		4'h0: oSEG = 7'b1000000;
		endcase
end
endmodule
```
