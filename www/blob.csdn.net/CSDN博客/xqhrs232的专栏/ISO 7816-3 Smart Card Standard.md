# ISO 7816-3 Smart Card Standard - xqhrs232的专栏 - CSDN博客
2013年09月22日 09:27:20[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1503
原文地址::[http://blog.csdn.net/turui/article/details/4851082](http://blog.csdn.net/turui/article/details/4851082)
 ISO 7816-3 Smart Card Standard:
Part 3: Electronic Signals and Transmission Protocols  
ISO 7816 [Part 1] [Part 2] [Part 3] [Part4]
This part describes electronic signals and transmission protocols of integrated circuit cards. We copied it from a version that is available on the Internet. If you need the official version of this part, please contact ISO in Switzerland. If you have suggestions
 or material to include (tables, graphs etc) please contact us. The document will stay at this location for anyone that wants a direct link to this part of the standard. We will edit this document as necessary to bring it up to date and add comments.
Most of ISO7816-3 is important for reader manufacturers or developers who want to establish a communication with a smart card on a very low level (the signal level). Going through ISO 7816-3 you will see what's involved in writing your own I/O software. This
 can be either to communicate from a microcontroller or a PC's serial/parallel/USB/PCMCIA port. Even if you don't go that far, it is interesting to read about what you can get out of an Answer to Reset (ATR).
There are many tools out there to read an ATR. Even on this site we put a remote version of a free ATR probing tool that reads and interprets an ATR over the Internet. All you need is a PCSC compliant smart card reader attached to a PC with an Internet connection.
Electrical Signals Description 
I/O : Input or Output for serial data to the integrated circuit inside the card.
VPP : Programing voltage input (optional use by the card).
GND : Ground (reference voltage).
CLK : Clocking or timing signal (optional use by the card).
RST : Either used itself (reset signal supplied from the interface device) or in combination with an interal reset control circuit (optional use by the card). If internal reset is implemented, the voltage 
supply on Vcc is mandatory.
VCC : Power supply input (optional use by the card).
NOTE - The use of the two remaining contacts will be defined in the appropriate application standards.
ISO7816 3.1 Voltage and current values 
Abbreviations:
Vih : High level input voltage 
Vil : Low level input voltage 
Vcc : Power supply voltage at VCC 
Vpp : Programming voltage at VPP 
Voh : High level output voltage 
Vol : Low level output voltage 
tr : Rise time between 10% and 90% of signal amplitude 
tf : Fall time between 90% and 10% of signal amplitude 
Iih : High level input current 
Iil : Low level input current 
Icc : Supply current at VCC 
Ipp : Programming current at VPP 
Ioh : High level output current 
Iol : Low level output current 
Cin : Input capacitance 
Cout: Output capacitance
* I/O
This contact is used as input (reception mode) or output (transmission mode) for data exchange. Two possible states exist for I/O:
- mark or high state (State Z), if the card and the interface device are in reception mode or if the state is imposed by the transmitter.
- space or low state (State A), if this state is imposed by the 
transmitter.
When the two ends of the line are in reception mode, the line shall be maintained in state Z. When the two ends are in non-matced transmit mode, the logic state of the line may be indeterminate. During operations, the interface device and the card shall not
 both be in transmit mode.
Table 1 - Electrical characteristics of I/O under normal operation conditions.
,--------+--------------------------------+---------+---------+------,
| Symbol |          Conditions            | Minimum | Maximum | Unit |
+--------+--------+-----------------------+---------+---------+------+
|        | Either | Iih max = +/- 500uA   |    2    |    VCC  |   V  |
|  Vih   |   (1)  +-----------------------+---------+---------+------+
|        |   or   | Iih max = +/- 50uA    | 0.7 VCC | VCC (3) |   V  |
+--------+--------+-----------------------+---------+---------+------+
|  Vil   |          Iil max = 1mA         |    0    |    0.8  |   V  |
+--------+--------------------------------+---------+---------+------+
|        | Either | Iol max = +/- 100uA   |   2.4   |    VCC  |   V  |
|  Voh   |        +-----------------------+---------+---------+------+
|    (2) |   or   | Iol max = +/- 20uA    |   3.8   |    VCC  |   V  |
+--------+--------+-----------------------+---------+---------+------+
|  Vol   |          Iol max = 1mA         |    0    |    0.4  |   V  |
+--------+--------------------------------+---------+---------+------+
| tr, tf | Cin = 30pF;   Cout = 30pF      |         |      1  |   us |
+--------+--------------------------------+---------+---------+------+
| (1) For the interface device, take into account both conditions.   |
| (2) It is assumed that a pull up resistor is used in the interface |
|     device (recommended value 20k Ohm.                             |
| (3) The voltage on I/O shall remain between 0.3V and VCC+0.3V.     |
'--------------------------------------------------------------------'
* VPP 
This contact may be to supply the voltage required to program or to erase the internal non-volatile memory. Two possible states exists for VPP: Idle state and active state, as defined in table 2. The idle state shall be maintained by the interface device unless
 the active state is required.
Table 2 : Electrical characteristics of VPP under normal operation conditions.
,--------+--------------------------------+---------+---------+------,
| Symbol |          Conditions            | Minimum | Maximum | Unit |
+--------+--------------------------------+---------+---------+------+
|  Vpp   |         Idle State             | 0.95*Vcc| 1.05*Vcc|   V  |
|  Ipp   |   (programming non active)     |         |   20    |  mA  |
+--------+--------------------------------+---------+---------+------+
|  Vpp   |        Active State            | 0.975*P | 1.025*P |   V  |
|  Ipp   |    (programming the card)      |         |     I   |  mA  |
+--------+--------------------------------+---------+---------+------+
| The card provides the interface with the values of P and I         |
| (default values: P=5 and I=50)                                     |
'--------------------------------------------------------------------'
Rise of fall time : 200 us maximum. The rate of change of Vpp shall not exceed 2V/us. 
The maximum power Vpp*Ipp shall not exceed 1.5W when averaged over any period of 1s.
* CLK
The actual frequency, delivered by the interface device on CLK, is designated either by fi the initial frequency during the answer to reset, or by fs the subsequent frequency during subsequent transmission.
Duty cycle for asynchronous operations shall be between 45% and 55% of the period during stable operation. Care shall be taken when switching frequencies (from fi to fs) to ensure that no pulse is shorter than 45% of the shorter period.
Table 3 - Electrical characteristics of CLK under normal operation conditions.
     ,--------+--------------------------------+---------+---------+------,
     | Symbol |          Conditions            | Minimum | Maximum | Unit |
     +--------+--------+-----------------------+---------+---------+------+
     |        | Either | Iih max = +/- 200uA   |   2.4   | VCC (2) |   V  |
     |        |   (1)  +-----------------------+---------+---------+------+
     |  Vih   |   or   | Iih max = +/- 20uA    | 0.7*VCC | VCC (2) |   V  |
     |        |   (1)  +-----------------------+---------+---------+------+
     |        |   or   | Iih max = +/- 10uA    | VCC-0.7 | VCC (2) |   V  |
     +--------+--------+-----------------------+---------+---------+------+
     |  Vil   |          Iil max = +/-200 uA   |   0 (2) |    0.5  |   V  |
     +--------+--------------------------------+---------+---------+------+
     | tr, tf |          Cin = 30pF            |         |9% of the period|
     |        |                                |         |with a max:0.5us|
     +--------+--------------------------------+---------+---------+------+
     | (1) For the interface device, take into account three conditions.  |
     | (2) The voltage on CLK shall remain between 0.3V and Vcc+0.3V.     |
     '--------------------------------------------------------------------'
* RST 
Table 4 - Electrical characteristics of RST under normal operation conditions.
     ,--------+--------------------------------+---------+---------+------,
     | Symbol |          Conditions            | Minimum | Maximum | Unit |
     +--------+--------+-----------------------+---------+---------+------+
     |        | Either | Iih max = +/- 200uA   |    4    | VCC (2) |   V  |
     |  Vih   |   (1)  +-----------------------+---------+---------+------+
     |        |   or   | Iih max = +/- 10uA    | VCC-0.7 | VCC (2) |   V  |
     +--------+--------+-----------------------+---------+---------+------+
     |  Vil   |          Iil max = +/- 200uA   |   0 (2) |    0.6  |   V  |
     +--------+--------------------------------+---------+---------+------+
     | (1) For the interface device, take into account both conditions.   |
     | (2) The voltage on RST shall remain between 0.3V and VCC+0.3V.     |
     '--------------------------------------------------------------------'
* VCC 
This contact is used to supply the power voltage Vcc.
Table 5 - Electrical characteristics of VCC under normal operation conditions.
              ,--------+---------+---------+-------,
              | Symbol | Minimum | Maximum |  Unit |
              +--------+---------+---------+-------+
              |  Vcc   |   4.75  |   5.25  |   V   |
              |  Icc   |         |    200  |  mA   |
              '--------+---------+---------+-------'
ISO7816 3.2 Operating procedure for integrated circuit(s) cards 
This operating procedure applies to every integrated circuit(s) card with contacts:
The dialogue between the interface device and the the card shall be conducted through the consecutive operations:
- connection and activation of the contacts by the interface device. 
- reset of the card. 
- answer to reset by the card. 
- subsequent information exchange between the card and the interface device. 
- desactivation of the contacts by the interface device.
These operations are specified in the following subclauses.
NOTE : 
An active state on VPP should not only be provided and maintained when requested by the card.
ISO7816 3.2.a - Connection and activation of the contacts 
The electrical circuits shall not be activated until the contacts are connected to the interface device so as to avoid possible damage to any card meeting these standards.
The activation of the contacts by the interface device shall consist of the consecutive operations:
- RST is in state L; 
- VCC shall be powered; 
- I/O in the interface device shall be put in reception mode; 
- VPP shall be raised to idle state; 
- CLK shallbe provided with a suitable and stable clock.
ISO7816 3.2.b - Reset of the card 
A card reset is initiated by the interface device, whereupon the card shall respond with an Answer to Reset as describe in 2.4.
By the end of the activation of the contacts (RST is in L, VCC powered and stable, I/O in reception mode in the interface device, VPP stable at idle level, CLK provided with a suitable and stable clock), the card answering asynchronously is ready for reset.
The clock signal is applied to CLK at time T0. The I/O line shall be set to state Z within 200 clcok cycles of the clock signal (t2) being applied to CLK (time t2 after T0).
An internally reset card reset after a few cycles of clock signal. The Answer to Reset on I/O shall begin between 400 and 40 000 clock cycles (t1) after the clock signal is applied to CLK (time t1 after T0).
A card with an active low reset is reset by maintaining RST in state L for at least 40 000 clock cycles (t3) after the clock signal is applied on CLK (time t3 after T0). Thus if no Answer to Reset begind within 40 000 clock cycles (t3) with RST in state L,
 RST is put to state H (at time T1). The 
Answer to Reset on I/O shall begin between 400 and 40 000 clock cycles (t1) after the rising edge of the signal on RST (time t1 after T1).
If the Anwser to Reset does not begin within 40 000 clock cycles (t3) with RST in state H (t3 after T1), the signal on RST shall be returned to state L (at time T2) and the contacts shall be desactivated by the interface device.
GND ________________________________________________________________________
      __________________________________________________________________
VCC _| :                                                               :|___
       :_______________________________________________________________:
VPP __|:                                                               |____
       :             t3                            t3                  :
       :<--------------------------->:<------------------------------->:
       :                             :_________________________________:
RST ___:_____________________________|                                 |____
       :                             :                                 :
CLK ___|||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||____
       :       t1                    :                                 :
       :<-------------->:            :                                 :
       :      __________:____________:_________________________________:
I/O __XXXXXXXX          |____________:_______Answer____________________:XXXX
(IR)   :      :                      :                                 :
       :  t2  :                      :      t1                         :
       :<---->:                      :<---------->:                    :
       :      _______________________:_________________________________:
I/O __XXXXXXXX                       :            |______Answer________:XXXX
(AL)   :  t2  :                      :                                 :
       :<---->:                      :                                 :
       :                             :_________________________________:
I/O __XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX:                                 :XXXXX
(SH)   :                             :                                 :
       T0                            T1                                T2
       IR : Internal Reset                t2 <= 200/fi
  AL : Asynchronous Reset            400/fi <= t1 <= 40000/fi
  SH : Syncronous Reset              40000/fi <= t3
                      Figure1 : Reset of the card
With a card answering synchonously, the interface device sets all the lines to state L (See figure 2). VCC is the powered, VPP is set to idle state, CLK and RST remain in L state, I/O is put in reception mode in the interface device, RST shall be maintained
 in state H for at least 50 us (t12), before returning to state L again.
The clock pulse is applied after an interval (t10) from the rising edge of the reset signal. The duration of the state H of the clock pulse can be any value between 10 us and 50 us ; no more than one clock pulse during reset high is allowed. The time interval
 between the falling edges on CLK 
and RST is t11.
The first data bit is obtained as an answer to reset on I/O while CLK is in state L and is valid after an interval t13 from the falling edge on RST.
      ______________________________________________________________________
VCC__/
       _____________________________________________________________________
VPP___/
                 t12
         :<---------------->:
         :__________________:
RST_____/:                   /_______________________________________________
         :                    :
         :  t10           t11 :          t15      t16
         :<---->:      :<---->:  t14   :<---->: :<---->:
                : ____ :      :<---->: :______: :      : _______
CLK_____________:/  1 /:______:______:/   2    /:______:/   3   /_______
                              :                 :
                              : t13             :  t17
                              :<---->:          :<---->:
       _____________________________ :______________   :______________   ___
I/O___///////////////////////////////:_______1______X-X_______2_______X-X___
5us  <= t10                          10us <= t14 <= 100us Clock low after RST
5us  <= t11                          10us <= t15 <= 50us  Clock High
50us <= t12 ........ Reset High      10us <= t16 <= 100us Clock Low
t13  <= 10us  Propagation delay      t17 <= 10us  Propagation delay
           Figure2 : Reset of the card when a synchronous answer is expected.
NOTES:
1 - The internal state of the card is assumed not to be defined before reset. Therefore the design of the card has to avoid inproper operation.
2 - In order to continue the dialogue with the card, RST shall be maintained in the state where an answer occurs on I/O.
3 - Reset of a card can be initiated by the interface device at its discetion at any time.
4 - Interface devices may support one or more of these types of reset behaviour. The priority of testing for asynchronous or synchronous cards is not defined in this standard.
ISO7816 3.2.c - Deactivation of the contacts 
When informations exchange is terminated or aborted (unresponsive card or detection of card removal), the electrical contacts shall be desactivated.
The deactivation by the interface device shall consist of the consecutive operations:
- State L on RST; 
- State L on CLK; 
- Vpp inactive; 
- State A on I/O; 
- VCC inactive;
ISO7816 3.3 Answer to Reset 
Two types of transmissions are considered:
* Asynchronous transmission: 
In this type of transmission, characters are transmitted on the I/O line in an asynchronous half duplex mode. Each character includes an 8bit byte.
* Synchronous transmission: 
In this type of transmission, a series of bits is transmitted on the I/O line in half duplex mode in synchronisation with the clock signal on CLK.
ISO7816 3.1.a - Answer to Reset in asynchronous transmission 
* Bit duration 
"""""""""""" 
The nominal bit duration used on I/O is defined as one Elementary Time Unit (etu).
For cards having internal clock, the initial etu is 1/9600 s.
For cards using the external clock, there is a linear relationship between the Elementary Time Unit used on I/O and the period provided by the interface device on CLK.
The initial etu is 372/fi s where fi is in Hertz.
The initial frequency fi is provided by the interface device on CLK during the Answer to Reset.
In order to read the initial character (TS), all cards shall initially be operated with fi in the range of 1 MHz to 5 MHz.
* Character frame during answer to reset 
"""""""""""""""""""""""""""""""""""""" 
Prior to the transmission of a character, I/O shall be in state Z.
A character consists of ten consecutive bits:
- a start bit in state A; 
- eight bits of information, designated ba to bh and conveying a data byte; 
- a tenth bit bi used for even parity checking.
A data byte consists of 8 bits designated b1 to b8, from the least significant bit (lsb, b1) to the most significant bit (msb, b8).
Conventions (level coding, connecting levels Z/A to digits 1 or 0: and a bit significance, connecting ba...bh to b1...b8) are specified in the initial character, call TS, which is transmitted by the card in response to reset.
Parity is correct when the number of ONES is even in the sequence from ba to bi.
Whithin a character, the time from the leading edge of the start bit to the trailing edge of the nth bit shall equal (n+/-0.2) etu.
When searching for a start, the receiver samples I/O periodically. The time origin being the mean between last observation of level Z and first observation of level A, the start shall be verified before 0.7 etu, and then ba is received at (1.5 +/-0.2) etu.
 Parity is checked on the fly.
NOTE : When searching for a start, the sampling time shall be less than 0.2 etu so that all the test zones are distinct from the transition zones.
The delay between two consecutives characters (between start leading edges) is at least 12 etu, including a character duration (10+/-0.2) etu plus a guardtime, the interface device and the card reamain both in reception, so that I/O is in state Z.
            Start                          Parity             Next
             bit <----- 8 data bits -----> bit              Start bit
     Z   ____     ________________________________......______     __
             |   |  |  |  |  |  |  |  |  |  |                 |   |
     I/O     |   |ba|bb|bc|bd|be|bf|bg|bh|bi|     Guardtime   |   |
             |___|__|__|__|__|__|__|__|__|__|                 |___|_
     A       :   :                    :     :
             0   t1                   :    t10
             :                        :
             :<---- (n+/-0.2) etu --->:
                          Figure 3: Character frame
During the Answer to Reset, the delay between the start leading edges of two consecutives characters from the card shall not exeed 9600 etu. This maximum is named initial waiting time.
* Error detection and character repetition 
"""""""""""""""""""""""""""""""""""""""" 
During the answer to reset, the following characters repetition procedure depends on the protocol type. This procedure is mandatory for cards using the protocol type T=0; it is optional for the interface device and for the other cards.
The transmitter tests I/O (11+/-0.2) etu after the start leading edge: 
- If I/O is in state Z, the correct reception is assumed. 
- If I/O is in state A, the transmission is assumed to have been incorrect. The disputed character shall be repeated after a delay of at least 2 etu after detection of the error signal.
When parity is incorrect, from (10.5+/-0.2) etu, the receiver transmits an error signal at state A for 1 etu minimum and 2 etu maximum. The receiver then shall expect a repetition of the disputed character (see figure 8).
If no character repetition is provided by the card, - The card ignores and shall not suffer damage from the error signal coming from the interface device. 
- The interface device shall be able to initiate the reception and the whole Answer to Reset response sequence.
* Structures and content 
"""""""""""""""""""""" 
A reset operation results in the answer from the card consisting of the initial character TS followed by at most 32 characters in the following order:
         - T0 ................... Format character     (Mandatory)
         - TAi, TBi, TCi, TDi ... Interface characters  (Optional)
         - T1, T2, ... ,TK ...... Historical characters (Optional)
         - TCK .................. Check character    (Conditional)
Reset
 |
 |    _________________________________________         _______   _________
 |   |   |   |   |   |   |   |   |   |   |   |           |   |     |   |   |
 '-->| TS| T0|TA1|TB1|TC1|TD1|TA2|TB2|TC2|TD2| ......... | T1| ... | TK|TCK|
     |___|___|___|___|___|___|___|___|___|___|_         _|___|_   _|__ |___|
     TS  : Initial character
     TO  : Format character
     TAi : Interface character [ codes FI,DI ]
     TBi : Interface character [ codes II,PI1 ]
     TCi : Interface character [ codes N ]
     TDi : Interface character [ codes Yi+1, T ]
     T1, ... , TK : Historical characters (max,15)
     TCK : Check character
         Figure 4 : General configuration of the Answer to Reset
The interface characters specify physical parameters of the integrated circuit in the card and logical characteristics of the subsequent exchange protocol.
The historical characters designate general information, for example, the card manufacturer, the chip inserted in the card, the masked ROM in the chip, the state of the life of the card. The specification of the historical characters falls outside the scope
 of this part of ISO/IEC7816.
For national simplicity, T0, TAi, ... ,TCK will designate the bytes as well as the characters in which they are contained.
Structure of TS, the initial character 
-------------------------------------- 
The initial character TS provides a bit shynchronisation sequence and defines the conventions to code data bytes in all subsequent characters. These conventions refer to ISO1177.
I/O is initially in state Z. A bit synchronisation sequence (Z)AZZA is defined for the start bit and bits ba bb bc (see figure 5).
The last 3 bits bg bh bi shall be AAZ for checking parity.
NOTE : This allows the interface device to determinate the etu initially used by the card. An alternate measurement of etu is a third of the delay between the first two falling edges in TS. Transmission and reception mechanisms in the card shall be consistent
 with the alternate 
definition of etu.
The two possible values of TS (ten consecutive bits from start to bi and corresponding hexadecimal value) are
- Inverse convention : (Z)ZZAAAAAZ 
where logic level ONE is A, ba is b8 (msb is first), equal to $3F when decoded by inverse convention.
- Direct convention : (Z)ZZAZZZAAZ 
where logic level ONE is Z, ba is b1 (lsb first), equal to $3B when decoded by direct convention.
                 Start  ba  bb  bc  bd  be  bf  bg  bh  bi
          Z   ____     _______     ___________         ______
                  |   |   |   |   | Z   Z   Z |       |       |   |
               (Z)| A | Z   Z | A |     or    |       | Z  (Z)
          A       |___|       |___|_A___A___A_|___|___|
                    Figure 5 : Initial character TS
                    --------
Structure of the subsequent characters in the Answer to Reset 
------------------------------------------------------------- 
The initial character TS is followed by a variable number of subsequent characters in the following order: The format character T0 and, optionally the interface characters TAi, TBi, TCi, TDi and the 
historical characters T1, T2, ... , TK and conditionally, the check character TCK.
The presence of the interface characters is indicated by a bit map technique explained below.
The presence of the historical characters is indicated by the number of bytes as specified in the format character defined below.
The presence of the check character TCK depends on the protocol type(s) as defined as below.
- Format character T0 
------------------- 
The T0 character contains two parts:
- The most significant half byte (b5, b6, b7, b8) is named Y1 and indicates with a logic level ONE the presence of subsequent characters TA1, TB1, TC1, TD1 respectively.
- The least significant half byte (b4 to b1) is named K and indicates the number (0 to 15) of historical characters.
         ,----,----,----,----,----,----,----,----,
         | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
         '----'----'----'----'----'----'----'----'
         :<------- Y1 ------>:<-------- K ------>:
         Y1 : indicator for the presence of the interface characters
                TA1 is transmitted when b5=1
                TB1 is transmitted when b6=1
                TC1 is transmitted when b7=1
                TD1 is transmitted when b8=1
         K : number of hitorical characters
                    Figure 6 : Informations provided by T0
                    --------
- Interface characters TAi, TBi, TCi, TDi 
---------------------------------------
TAi, TBi, TCi (i=1, 2, 3, ... ) indicate the protocol parameters. 
TDi indicates the protocol type T and the presence of subsequent 
characters.
Bits b5, b6, b7, b8 of the byte containing Yi (T0 contains Y1; TDi contains Yi+1) state whelther character TAi for b5, character TBi for b6, character TCi for b7, character TDi for b8 are or are not (depending on whether the relevant bit is 1 or 0) transmitted
 subsequently in this 
order after the character containing Yi.
When needed, the interface device shall attribute a default value to information corresponding to a non transmitted interface character.
When TDi is not transmitted, the default value of Yi+1 is null, indicating that no further interface characters TAi+j, TBi+j, TCi+j, TDi+j will be transmitted.
         ,----,----,----,----,----,----,----,----,
         | b8 | b7 | b6 | b5 | b4 | b3 | b2 | b1 |
         '----'----'----'----'----'----'----'----'
         :<------ Yi+1 ----->:<------- T ------->:
         Yi+1 : indicator for the presence of the interface characters
                TAi+1 is transmitted when b5=1
                TBi+1 is transmitted when b6=1
                TCi+1 is transmitted when b7=1
                TDi+1 is transmitted when b8=1
         T : Protocol type for subsequent transmission.
                    Figure 7 : Informations provided by TDi
                    --------
- Historical characters T1, T2, ... ,TK 
-------------------------------------
When K is not null, the answer to reset is continued by transmitting K historical characters T1, T2, ... , TK.
- Check character TCK 
-------------------
The value of TCK shall be such that the exclusive-oring of all bytes from T0 to TCK included is null.
The answer to reset is complete 12 etu after the leading edge of the last character.
Protocol type T 
--------------- 
The four least significant bits of any interface character TDi indicate a protocol type T, specifying rules to be used to process transmission protocols. When TDi is not transmitted, T=0 is used.
T=0 is the asynchronous half duplex character transmission protocol. 
T=1 is the asynchronous half duplex block transmission protocol. 
T=2 and T=3 are reserved for future full duplex operations. 
T=4 is reserved for an enhanced asynchronous half duplex character 
transmission protocol. 
T=5 to T=13 are reserved for future use. 
T=14 is reserved for protocols standardized by ISO. 
T=15 is reserved for future extension.
NOTE : If only T=0 is indicated, TCK shall not be sent. In all other cases TCK shall be sent.
Specifications of the global interface bytes 
-------------------------------------------- 
Among the interface bytes possibly transmitted by the card in answering to reset, this subclaus defines only the global interface bytes TA1,TB1, TC1, TD1.
These global interface bytes convey information to determine parameters which the interface device shall take into account.
- Parameters F, D, I, P, N 
------------------------
This initial etu is used during answer to reset is replaced by the work etu during subsequent transmission. F is the clock rate conversion factor and D is the bit rate adjustment factor to determine the work etu in subsequent transmissions.
For internal clock cards:
initial etu = 1/9600 s work etu = (1/D)*(1/9600) s
For external clock cards:
initial etu = 372/fi s work etu = (1/D)*(F/fs) s
The minimum value of fs shall be 1MHz. 
The maximum value of fs is given by table 6.
I and P define the active state at VPP. 
- Maximum programming current : Ipp = 1mA 
- Programming voltage : Vpp = P.V
N is an extra guardtime requested by the card. Before receiving the next character, the card requires a delay of at least (12+N) etu from the start leading edge of the previous character. No extra guardtme is used to send characters from the card to the interface
 device.
The default values of these parameters are: 
F = 372 ; D = 1 ; I = 50 ; P = 5 ; N = 0
- Integer values in global interface bytes 
----------------------------------------
The global interface bytes, TA1, TB1, TC1, TB2 code integer values FI, DI II, PI1, N, PI2 which are either equal to or used to compute the values of the parameters F, D, I, P, N presented above.
TA1 codes FI over the most significant half byte (b8 to b5) and DI over the least significant half byte (b4 to b1).
TB1 codes II over the bits b7 and b6, and PI1 over the 5 least significant bits b5 to b1. The most significant bit b8 equals to 0.
NOTE : The interface device may ignore the bit b8 of TB1.
TC1 codes N over the eight bits (b8 to b1).
TB2 codes PI2 over the eight bits (b8 to b1).
Table 6: Clock rate conversion factor F
     -------
     ----------------------------------------------------------------------
            FI     |     0000      0001  0010  0011  0100  0101  0110  0111
     --------------+-------------------------------------------------------
            F      | Internal clk   372   558   744  1116  1488  1860  RFU
     --------------+-------------------------------------------------------
      fs (max) MHz |      -           5     6     8    12    16    20   -
     ----------------------------------------------------------------------
     ---------------------------------------------------------------
            FI     | 1000  1001  1010  1011  1100  1101  1110  1111
     --------------+------------------------------------------------
            F      |  RFU   512   768  1024  1536  2048   RFU  RFU
     --------------+------------------------------------------------
      fs (max) MHz |  -       5   7.5    10    15    20    -    -
     ---------------------------------------------------------------
      RFU : Reserved for Future Use
     Table 7: Bit rate afjustment factor D
     -------
     -------------------------------------------------------
        DI | 0000  0001  0010  0011  0100  0101  0110  0111
     ------+------------------------------------------------
        D  |  RFU     1     2     4     8    16   RFU   RFU
     -------------------------------------------------------
     -------------------------------------------------------
        DI | 1000  1001  1010  1011  1100  1101  1110  1111
     ------+------------------------------------------------
        D  |  RFU   RFU   1/2   1/4   1/8  1/16  1/32  1/64
     -------------------------------------------------------
      RFU : Reserved for Future Use
- Programming voltage factor P 
---------------------------- 
PI1 from 5 to 25 gives the value of P in volts. PI1=0 indicates that VPP is connected in the card which generates an internal programming voltage from VCC. Other values of PI1 are reserved for future use.
When PI2 is present, the indication of PI1 should be ignores. PI2 from 50 to 250 gives the value of P in 0.1V. Other values of PI2 are reserved for future use.
Table 8 : Maximum programming current factor I
     -------
     -------------------------------
      II  |   00    01    10    11
     -----+-------------------------
      I   |   25    50   100   RFU
     -------------------------------
- Extra guardtime N 
-----------------
N codes directly the extra guard time, from 0 to 254 etu. N=255 indicates that the minimum delay between the start edges of two consecutives characters is reduced to 11 etu.
b - Answer to Reset in synchronous transmission 
------------------------------------------- 
* Clock frequency and bit rate 
"""""""""""""""""""""""""""" 
There is a linear relationship between the bit rate on the I/O line and the clock frequency provided by the clock interface device on CLK.
Any clock frequency between 7kHz and 50kHz may be chosen for the reset sequence. A clock frequency of 7kHz corresponds to 7kbit/s, and values of the clock frequency up to 50kHz cause corresponding bit rates to be transmitted.
* Structure of the header of the Answer to Reset 
"""""""""""""""""""""""""""""""""""""""""""""" 
The reset operation results in an answer from the card containing a header transmitted from the card to the interface. The header has a fixed length of 32 bits and begins with two mandatory fields of 8 bits, H1 and H2.
The chronological order of transmission of information bits shall correcpond to bit identification b1 to b32 with the least significant bit transmitted first. The numerical meaning corresponding to each 
information bit considered in isolation is that of the digit.
- 0 for a unit corresponding to state A (space) 
- 1 for a unit corresponding to state Z (mark)
* Timing of the haeder 
"""""""""""""""""""" 
After the reset procedure, the output information is controlled by clock pulses. The first clock pulse is applied between 10us and 100us (t14) after the falling edge on RST to read the data bits from the card. State H of the clock pulses can be varied between
 10us and 50us (t15) and state L between 10us and 100us (t16).
The first data bit is obtained on I/O while the clock is low and is valid 10us (t13) at least after the falling edge on RST. The following data bits are valid 10us (t17) at least after the falling edge on CLK. Each data bit is valid until the next falling edge
 the following clock pulse on CLK. The data bits can therefore be sampled at the rising edge of the following clock pulses.
* Data content of the header 
"""""""""""""""""""""""""" 
The header allows a quick determination of whelther the card and the interface device are compatible. If there is no compatibility, the contacts shall be desactivated.
The first field H1 codes the protocol type. The values of the codes and the corresponding protocol type are
Hexadecimal value protocol type 
----------------------------------- 
00 and ff not to be used 
01 to FE each value is assigned 
by ISO/IEC JTC1/SC17 to 
one protocol type
The second field H2 codes parameters for the protocol type coded in field H1. The values of H2 are to be assigned by ISO/IEC JTC1/SC17.
ISO7816 3.4 Protocol type selection (PTS) 
If only one protocol type and FI=D=1 (default value of TA1) and N smaller than 255 is indicated in the answer to reset. The transmission protocol associated to the protocol type may be started immediately after the transmission of answer to reset.
If more than one protocol type and/or TA1 parameter values other than the default values and/or N equeal to 255 is/are indicated in the answer to reset, the card shall know unambiguously, after having sent the answer to reset, which protocol type or/and transmission
 parameter values (FI, D, N) will be used. Consequently a selection of the protocol type and/or the transmission parameters values shall be specified.
If the card is able to process more than one protocol type and if one of those protocol types is indicated as T=0, then the protocol type T=0 shall indicated in TD1 as the first offered protocol, and is assumed if no PTS is performed.
If a card offers more than one protocol and if the interface device supports only one of these protocols which is not T=0 and does not support PTS, the interface should reject or reset the card.
ISO7816 3.4.a - PTS protocol 
Only the interface device is permitted to start a PTS procedure:
- The interface device sends a PTS request to the card. 
- If the card receives a correct PTS request, it answers by sending a PTS confirm, if implemented or the initial waiting time will be exceeded. 
- After the succesfull exchange of PTS request and PTS confirm, data shall be transmitted from the interface device using the selected protocol type and/or transmission parameters. 
- If the card receives an erronous PTS request, it will not send a PTS confirm. 
- If the initial waiting time is exceeded, the interface device should reset or reject the card. 
- If the interface device receives an erroneous PTS confirm, it should reset or reject the card.
The parameters for the transmission of the PTS request and PTS confirm shall correspond to those used within the Answer to Reset regarding the bit rate and the convention detected by TS and possibly modified by TC1.
ISO7816 3.4.b - Structure and content of PTS request and PTS confirm 
The PTS request and PTS response each consist of one initial character PTSS, followed by a format character PTS0, three optional parameter characters PTS1 PTS2 PTS3, and a character check PCK at the last byte.
PTSS identifies the PTS request or PTS confirm and is coded FF.
PTS0 indicates by the bits b5, b6, b7 set to 1 the presence of the subsequently sent optional characters PTS1, PTS2, PTS3 respectively. It codes over the least significant bits b4 to b1 the selected protocol type T as coded in TD bytes. The most significant
 bit b8 (default b8=0) is 
reserved for future use.
PTS1 codes the parameter values FI and D as coded in TA1. The interface device may send PTS1 in order to indicate the selection FI and/or D values to the card. If PTS1 is not sent, FI=1 and D=1 are assumed as defaults. The card either acknowledges both the
 FI and D values by echoing PTS1 or 
does not send PTS1 indicating the use of the default values.
PTS2 indicates the support of N=255, when bit b1 is set to 1. Bit b1 set to 0 is the default and indicates that the 11 etu period is not used. If bit b2 is set to 1, the card shall use an extra guardtime of 12 etu for its transmssion of characters to the interface
 device. Bit b2 set to 0 is 
the default and indicates that no extra guardtime is required. Bit b3 to b8 are reserved for future use.
If PTS2 is sent by the interface device and is not echoed by the card, the interface device should reject or reset the card.
The coding and use of PTS3 is not defined.
The value of PCK shall be such that the exclusive-oring of all charcters from PTSS to PCK included is null.
ISO7816 3.5 Protocol type T=0, asynchronous half duplex character transmission 
protocol 
This clause defines the structure and processing of commands initiated by an interface device for transmission control and for card specific control in an asynchronous half duplex character transmission protocol.
This protocol uses the parameters indicated by the answer to reset, unless modified by the protocol type selection.
ISO7816 3.5.a - Specific interface parameters: the work waiting time 
In an answer to reset, the interface character TC2 codes the integer value WI over eight bits b8 to b1. When no TC2 appears in the answer to reset, the default value of WI is 10.
The interval between the start leading edge of any character sent by the card and the start leading edge of the previous character (sent either by the card or by the interface device) shall not exceed 960*OWI work etu. This maximum delay is named the work waiting
 time.
ISO7816 3.5.b - Structure and processing of commands 
A command is always initiated by the interface device. It tells the card what to do in a 5-byte header, and allow a transfer of data bytes under control of procedure bytes sent by the card.
It is assumed that the card and the interface device know a priori the direction of data, in order to ditinguish between instructions for incoming data transfer (where data enter the card during execution) and instructions for outgoing data transfers (where
 data leave the card during 
execution).
without parity error
--------------------
        Start                                    Start
   _____    _____________________________________    ___________
        |  |  |  |  Byte i   |  |  |P |          |  |  Byte i+1
        |__|__|__|__|__|__|__|__|__|__| guartime |__|___________
                                   Even
with a parity error               parity
-------------------                bit
        Start                                        Start
   _____    ______________________________ Error   __    ___________
        |  |  |  |  Byte i   |  |  |P |  | signal |  |  |  Byte i+1
        |__|__|__|__|__|__|__|__|__|__|  |________|  |__|___________
           Figure 8 : Byte transmission diagram
           --------
* Command header sent by the interface device 
""""""""""""""""""""""""""""""""""""""""""" 
The interface device transmits a header over five successive bytes designated CLA, INS, A1, A2, L.
- CLA is an instruction class. The value FF is reserved for PTS.
- INS is an instruction code in the instruction class. The instructioncode is valid only if the least significant bit is 0, and the most significant half byte is neither 6 nor 9.
- P1, P2 are a reference (e.g. an address) completing the instruction code
- P3 codes the number n of data bytes (D1, ... , Dn) which are to be transmitted during the command. The direction of movement of these data is a function of the instruction. In an outgoing data transfer command, P3=0 introduces a 256 byte data transfer from
 the card. In an 
incoming data transfer command, P3=0 introduces no transfer of data.
All remaining encoding possibilities for the header are specified in subsequent parts of ISO7816.
After transmission of such 5 byte header, the interface device waits for a procedure byte.
* Procedure bytes sent by the card 
"""""""""""""""""""""""""""""""" 
The value of the procedure bytes shall indicate the action requested by the interface device. Three types of procedure bytes are specified:
- ACK : (The seven most significant bits in an ACK byte are all equal or complementary to those in the INS byte, apart from the values 6x and 9x) The interface device control VPP state and exchanges data depending on ACK values.
- NULL : (=$60) This byte is sent by the card to restart the working time, end to anticipate a subsequent procedure byte. It requests no further action neither on VPP nor on Data.
- SW1 (= $6x or $9x, expect $60); The interface device maintains or sets VPP at idle and waits for a SW2 byte to complete the command.
Any transition of VPP state (active/idle) must occur within the guardtime of the procedure byte, or on the work waiting time overflow.
At each procedure byte, the card can proceed with the command by an ACK or NULL byte, or show its disaproval by becoming unresponsive, or conclude by an end sequence SW1-SW2.
Byte | Value |  Result
  -----+-------+------------------------------------------------------------
       | INS   | VPP is idle. All remaining data bytes are transferred
       |       | subsequently.
       |       |
       | INS+1 | VPP is active. All remaining data bytes are transferred
       |       | subsequently.
  ACK  | ___   |
       | INS   | VPP is idle. Next data byte is transferred subsequently.
       | _____ |
       | INS+1 | VPP is active. Newt data byte is transferred subsequently.
  -----+-------+------------------------------------------------------------
  NULL | $60   | No futher action on VPP. The interface device waits for a
       |       | new procedure byte
  -----+-------+------------------------------------------------------------
  SW1  | SW1   | VPP is idle. The interface device waits for a SW2 byte
Acknowledge bytes 
---------------- 
The ACK bytes are used to control VPP state and data transfer. 
- When exclusive-oring the ACK byte with the INS byte gives $00 or $FF, the interface device maintains or sets VPP as idle.
- When exclusive-oring the ACK byte with the INS byte gives $01 or $FE, the interface device maintains or sets VPP as active.
- When the seven most significant bits in the ACK byte have the same value as those in the INS byte, all remaining data bytes (Di, ..., Dn) if any remain, are transferred subsequently.
- When the seven most significant bits in the ACK byte are complementary to those in the INS byte, only the next data byte (Di), if one remains is transferred.
After these actions, the interface device waits for a new procedure.
Null byte (= $60) 
----------------- 
This byte is sent by the card to reset the workwaiting time and to anticipate a subsequent procedure byte.
Status bytes (SW1=$6x or $9x, expect $60; SW2 any value) 
-------------------------------------------------------- 
The end sequence SW1-SW2 gives the card status at the end of the command.
The normal ending is indicated by SW1-SW2 = $90-$00.
When the most significant half byte SW1 is $6, the meaning of SW1 is independant of the application. The following five values are defined:
$6E The card does not support the instruction class. 
$6D The instruction code is not programmed or is invalid. 
$6B The reference is incorrect. 
$67 The length is incorrect. 
$6F No precise diagnostic is given.
Other values are reserved for future use by ISO7816. When SW1 is neither $6E nor $6D, the card support the instruction. This part of ISO7816 does not interprets neither $9X SW1 bytes, nor SW2 
bytes; Their meaning relates to the application itself.
Supplement (were seen sometimes): 
--------------------------------- 
SW1 SW2 Meaning
62 81 Returned data may be corrupted. 
62 82 The end of the file has been reached before the end of reading. 
62 84 Selected file is not valid. 
65 01 Memory failure. There have been problems in writing or reading 
the EEPROM. Other hardware problems may also bring this error. 
68 00 The request function is not supported by the card. 
6A 00 Bytes P1 and/or P2 are incorrect. 
6A 80 The parameters in the data field are incorrect. 
6A 82 File not found. 
6A 83 Record not found. 
6A 84 There is insufficient memory space in record or file. 
6A 87 The P3 value is not consistent with the P1 and P2 values. 
6A 88 Referenced data not found. 
6C XX Incorrect P3 length.
//=================================================
备注：：
1>ISO/IES 7816协议总共15个部分，里面最重要的是3/4这两个协议了，7816-3定义了整个协议的大概，7816-4对所有的APDU命令进行了详细的解释，其它的协议基本是针对某一块进行的说明----比如机械尺寸/特别的命令什么的
2>最好找58页的英文版看看！
  1111----ISO/IEC 7816-3:2006(E)
[](http://download.csdn.net/download/MUYEXUANWO/2922474)
# ISO+IEC 7816-3-2006.pdf下载地址----
[http://download.csdn.net/download/MUYEXUANWO/2922474](http://download.csdn.net/download/MUYEXUANWO/2922474)
  2222----ISO/IEC 7816-4:2005(E)
# iso/iec 7816-4-2005.pdf下载地址----
[http://download.csdn.net/download/locust123/4128154](http://download.csdn.net/download/locust123/4128154)

