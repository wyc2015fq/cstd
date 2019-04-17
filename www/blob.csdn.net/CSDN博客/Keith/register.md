# register - Keith - CSDN博客





2014年04月06日 11:29:14[ke1th](https://me.csdn.net/u012436149)阅读数：721








### Registers
In 16-bit mode, such as provided by the Pentium processor when operating as a Virtual 8086 (this is the mode used when Windows 95 displays a DOS prompt), the processor provides the programmer with
 14 internal registers, each 16 bits wide. They are grouped into several categories as follows:- Four general-purpose registers, AX, BX, CX, and DX. Each of these is a combination of two 8-bit registers which are separately accessible as AL, BL, CL, DL (the "low'' bytes) and AH, BH, CH, and DH (the "high'' bytes). For example, if AX contains the 16-bit
 number 1234h, then AL contains 34h and AH contains 12h.
- Four special-purpose registers, SP, BP, SI, and DI.
- Four segment registers, CS, DS, ES, and SS.
- The instruction pointer, IP (sometimes referred to as the program counter).
- The status flag register, FLAGS.

Although I refer to the first four registers as "general-purpose'', each of them is designed to play a particular role in common use:
- AX is the "accumulator''; some of the operations, such as MUL and DIV, require that one of the operands be in the accumulator. Some other operations, such as ADD and SUB, may be applied to any of the registers (that is,
 any of the eight general- and special-purpose registers) but are more efficient when working with the accumulator.
- BX is the "base'' register; it is the only general-purpose register which may be used for indirect addressing. For example, the instruction MOV [BX], AX causes the contents of AX to be stored in the memory location whose address is given in BX.
- CX is the "count'' register. The looping instructions (LOOP, LOOPE, and LOOPNE), the shift and rotate instructions (RCL, RCR, ROL, ROR, SHL, SHR, and SAR), and the
 string instructions (with the prefixes REP, REPE, andREPNE) all use the count register to determine how many times they will repeat.
- DX is the "data'' register; it is used together with AX for the word-size MUL and DIV operations, and it can also hold the port number for the IN and OUT instructions, but it is mostly available as a convenient place
 to store data, as are all of the other general-purpose registers.

Here are brief descriptions of the four special-purpose registers:
- SP is the stack pointer, indicating the current position of the top of the stack. You should generally never modify this directly, since the subroutine and interrupt call-and-return mechanisms depend on the contents of the stack.
- BP is the base pointer, which can be used for indirect addressing similar to BX.
- SI is the source index, used as a pointer to the current character being read in a string instruction (LODS, MOVS, or CMPS). It is also available as an offset to add to BX or BP when doing indirect addressing; for example, the
 instruction MOV [BX+SI], AX copies the contents of AX into the memory location whose address is the sum of the contents of BX and SI.
- DI is the destination index, used as a pointer to the current character being written or compared in a string instruction (MOVS, STOS, CMPS, or SCAS). It is also available as an offset, just like SI.

Since all of these registers are 16 bits wide, they can only contain addresses for memory within a range of 64K (=2^16) bytes. To support machines with more than 64K of physical memory, Intel implemented
 the concept of *segmented* memory. At any given time, a 16-bit address will be interpreted as an offset within a 64K segment determined by one of the four segment registers (CS, DS, ES, and SS).

As an example, in the instruction MOV [BX], AX mentioned above, the BX register really provides the offset of a location in the current data segment; to find the true physical address into which
 the contents of the accumulator will be stored, you have to add the value in BX to the address of the start of the data segment. This segment start address is determined by taking the 16-bit number in DS and multiplying by 16. Therefore, if DS contains 1234h and
 BX contains 0017h, then the physical address will be 1234h TIMES 16+0017h=12340h+0017h=12357h. (This computation illustrates one reason why hexadecimal is so useful; multiplication by 16 corresponds to
 shifting the hex digits left one place and appending a zero.) We refer to this combined address as 1234:0017 or, more generally, as DS:BX.

Since segment starts are computed by multiplying a 16-bit number by 16=2^4, the effect is that physical addresses have a 20-bit range, so that a total of 1M (=2^20) of memory may be used. Intel considered
 that this would be enough for applications of the 8086 over its projected lifetime of about five years from its introduction in 1978; by the time microcomputers were needing more than a meg of main memory, the next Intel processor (the iAPX432) was due to
 be available, with a 32-bit address space (able to address 4G---over 4 billion memory locations). However, the IBM PC's debut in 1981 and subsequent popularity has forced Intel to continue the 80x86 family of backward-compatible processors to the present,
 including support for a mode in which only 1M of memory is accessible. Processors since the 80286 have also provided the "protected'' mode of operation, which in the Pentium gives each process a flat 32-bit address space of up to 4G.

You might think that a segment register would only need to provide the uppermost 4 bits to extend an address out to 20 bits, but consider one of the implications of only having 16 different, non-overlapping
 segments: every segment would have to occupy a full 64K of memory, even if only a small fraction of this space were needed. By allowing a segment to start at any address divisible by 16, the memory may be allocated much more efficiently---if one program only
 needs 4K for its code segment, then theoretically the operating system could load another program into a segment starting just 4K above the start of the first. Of course, MS-DOS is not really this sophisticated, but the Intel designers wanted it to be possible.

Each segment register has its own special uses:
- CS determines the "code'' segment; this is where the executable code of a program is located. It is not directly modifiable by the programmer, except by executing one of the branching instructions. One of the reasons for separating the code segment from
 other segments is that well-behaved programs never modify their code while executing; therefore, the code segment can be identified as "read-only''. This simplifies the work of a cache, since no effort is required to maintain consistency between the cache
 and main memory. It also permits several instances of a single program to run at once (in a multitasking operating system), all sharing the same code segment in memory; each instance has its own data and stack segments where the information specific to the
 instance is kept. Picture multiple windows, each running Word on a different document; each one needs its own data segment to store its document, but they can all execute the same loaded copy of Word.



- DS determines the "data'' segment; it is the default segment for most memory accesses.



- ES determines the "extra'' segment; it can be used instead of DS when data from two segments need to be accessed at once. In particular, the DI register gives an offset relative to ES when used in the string instructions; for example, the MOVSB instruction
 copies a byte from DS:SI to ES:DI (and also causes SI and DI to be incremented or decremented, ready to copy the next byte).



- SS determines the "stack'' segment; the stack pointer SP gives the offset of the current top-of-stack within the stack segment. The BP register also gives an offset relative to the stack segment by default, for convenient access to data further down in
 the stack without having to modify SP. Just as with SP, you should not modify SS unless you know exactly what you are doing.

The instruction pointer, IP, gives the address of the *next* instruction to be executed, relative to the code segment. The only way to modify this is with a branch instruction.

The status register, FLAGS, is a collection of 1-bit values which reflect the current state of the processor and the results of recent operations. Nine of the sixteen bits are used in the 8086:
- Carry (bit 0): set if the last arithmetic operation ended with a leftover carry bit coming off the left end of the result. This signals an overflow on unsigned numbers.
- Parity (bit 2): set if the low-order byte of the last data operation contained an even number of 1 bits (that is, it signals an even parity condition).
- Auxiliary Carry (bit 4): used when working with binary coded decimal (BCD) numbers.
- Zero (bit 6): set if the last computation had a zero result. After a comparison (CMP, CMPS, or SCAS), this indicates that the values compared were equal (since their difference was zero).
- Sign (bit 7): set if the last computation had a negative result (a 1 in the leftmost bit).
- Trace (bit 8): when set, this puts the CPU into single-step mode, as used by debuggers.
- Interrupt (bit 9): when set, interrupts are enabled. This bit should be cleared while the processor is executing a critical section of code that should not be interrupted (for example, when processing another interrupt).
- Direction (bit 10): when clear, the string operations move from low addresses to high (the SI and DI registers are incremented after each character). When set, the direction is reversed (SI and DI are decremented).
- Overflow (bit 11): set if the last arithmetic operation caused a signed overflow (for example, after adding 0001h to 7FFFh, resulting in 8000h; read as two's complement numbers, this corresponds to adding 1 to 32767 and ending
 up with -32768).
There are numerous operations that will test and manipulate various of these flags, but to get the contents of the entire FLAGS register one has to push the flags onto the stack (with PUSHF or
 by calling an appropriate interrupt handler with INT) and then pop them off into another register. To set the entire FLAGS register, the sequence is reversed (with POPF or IRET).
 For example, one way to set the carry flag (there are much better ways, including the STC instruction) is the following:        PUSHF
        POP     AX
        OR      AX, 1
        PUSH    AX
        POPF
Most of the time you will not have to deal with the FLAGS register explicitly; instead, you will execute one of the conditional branch instructions, Jcc,
 where cc is one of the following mnemonic condition codes:- O, Overflow
- NO, Not Overflow
- B, Below; C, Carry; NAE, Not Above or Equal
- NB, Not Below; NC, Not Carry; AE, Above or Equal
- E, Equal; Z, Zero
- NE, Not Equal; NZ, Not Zero
- BE, Below or Equal; NA, Not Above (true if either Carry or Zero is set)
- NBE, Not Below or Equal; A, Above
- S, Sign
- NS, Not Sign
- P, Parity; PE, Parity Even
- NP, Not Parity; PO, Parity Odd
- L, Less; NGE, Not Greater or Equal (true if Sign and Overflow are different)
- NL, Not Less; GE, Greater or Equal
- LE, Less or Equal; NG, Not Greater (true if Sign and Overflow are different, or Zero is set)
- NLE, Not Less or Equal; G, Greater
All of the conditions on the same line are synonyms. The Above and Below conditions refer to comparisons of unsigned numbers, and the Less and Greater conditions refer to comparisons of signed (two's
 complement) numbers.


