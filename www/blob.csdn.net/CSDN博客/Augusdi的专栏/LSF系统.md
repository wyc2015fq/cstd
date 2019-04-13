
# LSF系统 - Augusdi的专栏 - CSDN博客


2015年05月08日 23:22:38[Augusdi](https://me.csdn.net/Augusdi)阅读数：1971


﻿﻿
### 環境變數
基本上當您登入後，系統就已將LSF 相關的環境變數載入，因此您不需再做任何設定。 您可以透過以下的命令，來檢驗系統是否已幫您完成 LSF 的相關設定。
|$|env | grep LSF
|LSF_ENVDIR=/opt/lsf/conf
|LSF_BINDIR=/opt/lsf/6.2/hpuxia64/bin
|LSF_LIBDIR=/opt/lsf/6.2/hpuxia64/lib
|LSF_SERVERDIR=/opt/lsf/6.2/hpuxia64/etc
|XLSF_UIDDIR=/opt/lsf/6.2/hpuxia64/lib/uid
|$|echo $PATH
|...:/opt/lsf/6.2/hpuxia64/etc:/opt/lsf/6.2/hpuxia64/bin:…
|
### 基本指令
基本操作的幾個指令為：bsub、bqueues、bhosts、bjobs、bkill、bhist、bacct
|bsub|
|【|說明】
|bsub|為LSF 提交(submit) 計算工作的指令，常用的參數有：|-n|、|-q|、|-o|、|-e|、|-J
|

|-n
|指定計算工作所需的計算核心數目。可省略，預設值是：1
|
|-q
|指定執行計算工作的佇列的名稱。可省略，預設名稱是：xfer
|
|-o
|指定程式訊息(stdout) 的輸出檔案名稱。可省略，預設名稱是：|$job_id|.out
|
|-e
|指定錯誤訊息(stderr) 的輸出檔案名稱。可省略，預設名稱是：|$job_id|.err
|
|-J
|指定計算工作在佇列中的名稱。可省略，預設名稱為所執行的程式名稱
|

|【範例】
|1.
|非平行(serial ) 程式(|其中以紅色粗體標出的部分，是您要執行的程式名稱|)：
|

|$|bsub ./my_program
|

|$|bsub|-n 1 -q xfer -o output.txt -e error.txt -J TEST|./my_program
|

|上述兩例執行後產生的結果皆相同，都是提交到xfer 這個佇列裡。不同的是程式訊息輸出檔及錯誤訊息輸出檔，前例會以$job_id 做為檔案名稱。除此之外，後例在佇列中顯示的名稱為TEST，但前例則是my_program。
|

|2.
|平行程式(|其中以紅色粗體標出的部分，是您要執行的程式名稱|)：
|

|$|bsub|-n 4 -q xfer|/work1/my_small_job
|

|$|bsub|-n 16 -q mono|my_paralle_job
|

|上述兩例的差別在於指定執行程式的路徑位置，若未列出指定執行程式的完整路徑，則預設是認為執行程式位於目前的目錄下。
|


|3.
|MPI 的程式(|其中以紅色粗體標出的mpi_program，即是您在互動模式(interactive mode) 下所執行的程式名稱|)
|

|$|bsub|-n 16 -q mono|mpirun -np 4|mpi_program
|
|bqueues|
|【說明】
|查看各個佇列的排隊狀況
|
|【範例】
|$|bqueues
|QUEUE_NAME    PRIO   STATUS          MAX JL/U JL/P JL/H NJOBS PEND  RUN  SUSP
|mono                50     Open:Active      80     80    4     -     224     160   64        0
|xfer                  50     Open:Active       80    32     4     -       0         0     0        0
|

|bhosts|
|【說明】
|查看佇列系統中的各個計算主機(host) 的狀態
|
|【範例】|$|bhosts
|HOST_NAME    STATUS      JL/U   MAX NJOBS   RUN SSUSP USUSP   RSV
|hale                   ok             -          88     64        64        0        0         0
|halen                 ok             -            8       0          0        0        0         0
|

|bjobs|
|【說明】
|查看佇系統中各個計算工作的執行狀態。常用參數有：|-u|、|-q|、|-l|、|-p
|
|-u|指定要查詢的使用者帳號。可省略，預設值是目前使用的帳號。
|
|-q|指定要查詢的佇列名稱。
|
|-l|以長格式(long format) 的方式，顯示詳細的佇列資料。
|
|-p|只顯示暫停中(pending) 的計算工作|。
|
|不加任何參數時，只會顯示目前使用者執行中或暫停中的計算工作。若要查看其他使用者計算工作的執行狀態，可使用-u 參數指定要查詢的使用者帳號，或使用all 查詢所有使用者的執行狀態。(註：all 亦可套用在佇列名稱上。)
|
|【範例】
|$|bjobs -u all
|JOBID  USER    STAT  QUEUE     FROM_HOST  EXEC_HOST  JOB_NAME  SUBMIT_TIME
|4001   u11aaa0 RUN  mono       halen            32*hale       *igen-1010 Jan 1 02:03
|4002   u12bbb0 RUN  mono       halen            32*hale       *igen-1020 Jan 1 03:04
|4003   u13ccc0  RUN  xfer         halen             hale           *igen-1030 Jan 1 04:05
|4004   u14ddd0 PEND mono      halen                              *igen-1040 Jan 1 05:06
|4005   u15eee0 PEND mono      halen                              *igen-1050 Jan 1 06:07
|4006   u16fff0   PEND mono      halen                              *igen-1060 Jan 1 07:08
|4007   u17ggg0 PEND mono      halen                              *igen-1070 Jan 1 08:09
|
|bkill|
|【說明】
|終止或暫停計算工作。
|
|【範例】
|$|bkill 4001|


|bhist|
|【說明】
|用來查看(包括已執行完成的) 計算工作的執行歷程及結束的原因。常用的參數有：-b、-l
|
|-b簡短格式|
|-l 詳細資料|
|【|範例】|$|bhist -l4001|Job <4001>, User , Project , Command
|Sat Jan 1 21:31:06: Submitted from host , to Queue , CWD , Output File , 32 Processors Requested;
|Sun Jan 2 17:10:38: Dispatched to 32 Hosts/Processors <32*hale>;
|Sun Jan 2 17:10:38: Starting (Pid 18479);
|Sun Jan 2 17:10:38: Running with execution home , Execution CW|D , Execution Pid <10000>;
|Summary of time in seconds spent in various states by Sun Jan 2 16:04:18
|PEND    PSUSP       RUN         USUSP   SSUSP   UNKWN   TOTAL
|243572    0           168820     0           0           0            412392
|
|bacct|
|【說明】
|統計在佇列系統中執行的CPU 時間及相關資料。常用的參數有：|-C|、|-l|、|-q|、|-u
|
|-|C 指定時間。可省略，未指定則是從系統紀錄開始時間到現在時間。|
|-l 詳細資料。可省略，未指定則僅顯示統計數據不會顯示每一筆記錄。
|
|-q 指定佇列名稱。可省略，未指定則是所有佇列都算。
|
|-u 指定使用者帳號(可省略)。
|
|【|範例】|$|bacct|-u user -q mono -C 03/01,05/31|
|統計user 從今年3/1 日到5/31 間，提交到mono 這佇列中的使用資料。
|
以上各指令的詳細用法，皆可由man page 取得。例如：man bacct、man bjobs、…



