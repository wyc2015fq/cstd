# Oracle等待事件kfk:async disk IO - weixin_33985507的博客 - CSDN博客
2013年03月19日 00:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
kfk: async disk IO等待事件是ASM下异步的System I/O等待事件，kfk内核层面在disk_asynch_io=true时被激活。当rbal或其他ASM相关后台进程在维护ASM磁盘组时可能进入kfk: async disk IO等待。
SQL> col name for a20
SQL> col PARAMETER1 for a10
SQL> col PARAMETER2 for a10
SQL> col PARAMETER3 for a10
SQL> col WAIT_CLASS for a15
SQL> select name,parameter1,parameter2,parameter3,wait_class from v$event_name where name='kfk: async disk IO';
NAME                 PARAMETER1 PARAMETER2 PARAMETER3 WAIT_CLASS
-------------------- ---------- ---------- ---------- ---------------
kfk: async disk IO   count      intr       timeout    System I/O
SQL> select * from v$version;    
BANNER
--------------------------------------------------------------------------------
Oracle Database 11g Enterprise Edition Release 11.2.0.2.0 - 64bit Production
PL/SQL Release 11.2.0.2.0 - Production
CORE    11.2.0.2.0      Production
TNS for Linux: Version 11.2.0.2.0 - Production
NLSRTL Version 11.2.0.2.0 - Production
SQL> select name,value from v$system_parameter where name in ('instance_type','asm_power_limit');
NAME                 VALUE
-------------------- ----------
instance_type        asm
asm_power_limit      10
SQL> conn / as sysasm
Connected.
SQL> oradebug setmypid;
Statement processed.
SQL> oradebug event 10046 trace name context forever,level 8;
Statement processed.
SQL> alter diskgroup data check all;
Diskgroup altered.
SQL> oradebug event 10046 trace name context off;
Statement processed.
SQL> oradebug tracefile_name;
/s01/orabase/diag/asm/+asm/+ASM1/trace/+ASM1_ora_29405.trc
=====================trace=====================
PARSING IN CURSOR #140442102181424 len=30 dep=0 uid=0 oct=193 
lid=0 tim=1313673029551496 hv=2849532521 ad='6bd58b50' sqlid='ft5h7dunxhum9'
alter diskgroup data check all
END OF STMT
PARSE #140442102181424:c=1999,e=14171,p=0,cr=0,cu=0,mis=1,r=0,dep=0,og=1,plh=0,tim=1313673029551493
WAIT #140442102181424: nam='Disk file operations I/O' ela= 573 FileOperation=2 fileno=0 filetype=15 obj#=-1 
WAIT #140442102181424: nam='Disk file operations I/O' ela= 33 FileOperation=2 fileno=0 filetype=15 obj#=-1 
WAIT #140442102181424: nam='Disk file operations I/O' ela= 29 FileOperation=2 fileno=0 filetype=15 obj#=-1 
WAIT #140442102181424: nam='kfk: async disk IO' ela= 941 count=1 intr=0 timeout=4294967295 obj#=-1
fdp_checkDsk(): 20
----- Abridged Call Stack Trace -----
ksedsts()+461<-kfdp_checkDsk()+476<-kfdCheck()+1649<-kfgCheck()+477<-kfxdrvAl
terOne()+5976<-kfxdrvAlter()+2287<-kfxdrvEntry()+1306<-opiexe()+20028<-opiosq
0()+3993<-kpooprx()+274<-kpoal8()+800<-opiodr()+910<-ttcpip()+2289<-opitsk()+
1670<-opiino()+966<-opiodr()+910
<-opidrv()+570<-sou2o()+103<-opimai_real()+133<-ssthrdmain()+252<-main()+201<
-__libc_start_main()+244<-_start()+36
----- End of Abridged Call Stack Trace -----
WAIT #140442102181424: nam='rdbms ipc reply' ela= 1610 from_process=19 timeou
t=2147483647 p3=0 obj#=-1 tim=1313673029798048
kfdp_checkDsk(): 21
----- Abridged Call Stack Trace -----
ksedsts()+461<-kfdp_checkDsk()+476<-kfdCheck()+1649<-kfgCheck()+477<-kfxdrvAl
terOne()+5976<-kfxdrvAlter()+2287<-kfxdrvEntry()+1306<-opiexe()+20028<-opiosq
0()+3993<-kpooprx()+274<-kpoal8()+800<-opiodr()+910<-ttcpip()+2289<-opitsk()+
1670<-opiino()+966<-opiodr()+910
<-opidrv()+570<-sou2o()+103<-opimai_real()+133<-ssthrdmain()+252<-main()+201<
-__libc_start_main()+244<-_start()+36
----- End of Abridged Call Stack Trace -----
WAIT #140442102181424: nam='rdbms ipc reply' ela= 1677 from_process=19 timeou
t=2147483647 p3=0 obj#=-1 tim=1313673029885645
kfdp_checkDsk(): 22
----- Abridged Call Stack Trace -----
ksedsts()+461<-kfdp_checkDsk()+476<-kfdCheck()+1649<-kfgCheck()+477<-kfxdrvAl
terOne()+5976<-kfxdrvAlter()+2287<-kfxdrvEntry()+1306<-opiexe()+20028<-opiosq
0()+3993<-kpooprx()+274<-kpoal8()+800<-opiodr()+910<-ttcpip()+2289<-opitsk()+
1670<-opiino()+966<-opiodr()+910
<-opidrv()+570<-sou2o()+103<-opimai_real()+133<-ssthrdmain()+252<-main()+201<
-__libc_start_main()+244<-_start()+36
----- End of Abridged Call Stack Trace -----
WAIT #140442102181424: nam='rdbms ipc reply' ela= 1350 from_process=19 timeou
t=2147483647 p3=0 obj#=-1 tim=1313673029970397
kfdp_checkDsk(): 23
----- Abridged Call Stack Trace -----
kfdp_checkDsk(): 24
----- Abridged Call Stack Trace -----
ksedsts()+461<-kfdp_checkDsk()+476<-kfdCheck()+1649<-kfgCheck()+477<-kfxdrvAl
terOne()+5976<-kfxdrvAlter()+2287<-kfxdrvEntry()+1306<-opiexe()+20028<-opiosq
0()+3993<-kpooprx()+274<-kpoal8()+800<-opiodr()+910<-ttcpip()+2289<-opitsk()+
1670<-opiino()+966<-opiodr()+910
<-opidrv()+570<-sou2o()+103<-opimai_real()+133<-ssthrdmain()+252<-main()+201<
-__libc_start_main()+244<-_start()+36
----- End of Abridged Call Stack Trace -----
