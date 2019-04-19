# RTP payload type - maopig的专栏 - CSDN博客
2011年08月07日 21:23:29[maopig](https://me.csdn.net/maopig)阅读数：15893
RTP payload type
## RTP payload type
payload type:
PT       encoding    audio/video clock rate channels
name       (A/V)       (Hz)       (audio)
_______________________________________________________________
0       PCMU       A              8000       1     [RFC3551]
1       Reserved 
2       Reserved
3          GSM            A              8000       1     [RFC3551]
4       G723          A              8000       1     [Kumar]
5       DVI4       A              8000       1     [RFC3551]
6       DVI4       A              16000       1     [RFC3551]
7       LPC           A              8000       1     [RFC3551]
8       PCMA          A              8000       1     [RFC3551]
9       G722       A              8000       1     [RFC3551]
10       L16           A              44100       2     [RFC3551]
11       L16           A              44100       1     [RFC3551]
12       QCELP        A              8000       1 
13       CN          A              8000       1     [RFC3389]
14       MPA           A              90000     [RFC3551][RFC2250]
15       G728       A              8000       1     [RFC3551]
16       DVI4       A              11025       1     [DiPol]
17       DVI4       A              22050       1     [DiPol]
18       G729       A              8000       1
19       reserved    A
20       unassigned A
21       unassigned A
22       unassigned A
23       unassigned A
24       unassigned V
25       CelB       V              90000             [RFC2029]
26       JPEG       V              90000             [RFC2435]
27       unassigned V
28       nv             V              90000             [RFC3551]
29       unassigned V
30       unassigned V
31       H261       V              90000             [RFC2032]
32       MPV           V              90000             [RFC2250]
33       MP2T       AV          90000             [RFC2250]
34       H263          V              90000             [Zhu]
35--71     unassigned ?
72--76     reserved for RTCP conflict avoidance          [RFC3550]
77--95     unassigned ?
96--127 dynamic    ?                               [RFC3551] 
                                   clock rate     channels
media type     subtype    (Hz)       (audio)
_______________________________________________________________
application parityfec                                 [RFC3009]
application rtx                                         [RFC4588]
audio       AMR           8000                      [RFC4867][RFC3267]
audio       AMR-WB        16000                   [RFC4867][RFC3267]
audio       DAT12                                      [RFC3190]
audio       dsr-es201108                         [RFC3557]
audio       EVRC          8000       1       [RFC4788] 
audio       EVRC0       8000       1       [RFC4788] 
audio       EVRC1       8000       1       [RFC4788] 
audio       EVRCB       8000       1       [RFC4788] 
audio       EVRCB0        8000       1       [RFC4788] 
audio       EVRCB1        8000       1       [RFC4788] 
audio       G7221       16000       1       [RFC3047]
audio       G726-16    8000       1       [RFC3551][RFC4856]
audio       G726-24    8000       1       [RFC3551][RFC4856]
audio       G726-32    8000       1       [RFC3551][RFC4856]
audio       G726-40    8000       1       [RFC3551][RFC4856]
audio       G729D       8000          1       [RFC3551][RFC4856]
audio       G729E       8000           1       [RFC3551][RFC4856]
audio       GSM-EFR    8000       1       [RFC3551][RFC4856]
audio       L8                                             [RFC3551][RFC4856]
audio       RED                                           [RFC2198][RFC3555]
audio       rtx                                             [RFC4588]
audio       VDVI                              1       [RFC3551][RFC4856]
audio       L20                                  [RFC3190]
audio       L24                                  [RFC3190]
audio       MP4A-LATM                               [RFC3016]
audio       mpa-robust 90000                    [RFC3119]
audio       parityfec                            [RFC3009]
audio       SMV           8000       1       [RFC3558]
audio       SMV0       8000       1       [RFC3558]
audio       t140c                               [RFC4351]
audio       t38                                  [RFC4612]
audio       telephone-event                      [RFC4733]
audio       tone                                   [RFC4733]
audio       DVI4                                   [RFC4856]
audio       G722                                   [RFC4856]
audio       G723                                   [RFC4856]
audio       G728                                   [RFC4856]
audio       G729                                   [RFC4856]
audio       GSM                                  [RFC4856]
audio       L16                                  [RFC4856]
audio       LPC                                  [RFC4856]
audio       PCMA                                   [RFC4856]
audio       PCMU                                   [RFC4856]
text           parityfec                            [RFC3009]
text           red           1000                   [RFC4102]
text           rtx                                  [RFC4588]
text           t140       1000                   [RFC4103]
video       BMPEG       90000                    [RFC2343][RFC3555]
video       BT656       90000                    [RFC2431][RFC3555]
video       DV          90000                    [RFC3189]
video       H263-1998     90000                    [RFC2429][RFC3555]
video       H263-2000     90000                    [RFC2429][RFC3555]
video       MP1S       90000                    [RFC2250][RFC3555]
video       MP2P       90000                    [RFC2250][RFC3555]
video       MP4V-ES    90000                    [RFC3016]
video       parityfec                            [RFC3009]
video       pointer    90000                    [RFC2862]
video       raw           90000                    [RFC4175]
video       rtx                                  [RFC4588]
video       SMPTE292M                            [RFC3497]
video       vc1           90000                    [RFC4425]
