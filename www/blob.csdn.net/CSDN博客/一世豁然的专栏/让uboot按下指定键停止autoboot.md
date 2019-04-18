# 让uboot按下指定键停止autoboot - 一世豁然的专栏 - CSDN博客





2015年07月07日 10:28:01[一世豁然](https://me.csdn.net/Explorer_day)阅读数：2658








本博客转载于：[http://blog.chinaunix.net/uid-13889805-id-1641961.html](http://blog.chinaunix.net/uid-13889805-id-1641961.html)




默认uboot是按下任意键停止autoboot，有人觉得不爽，害怕终端操作用户误操作，要指定特殊按键
这个功能很简单，例如要按下ESC键，停止自动boot
只需要添加

- #define CONFIG_AUTOBOOT_KEYED 1

- 
#define CONFIG_AUTOBOOT_PROMPT "Press ESC to abort autoboot in %d seconds"

- 
#define CONFIG_AUTOBOOT_DELAY_STR "linux"

- 
#define CONFIG_AUTOBOOT_STOP_STR "\x1b"

即可，0x1B就是ESC的ascii码

在2008.10等高版本uboot上这样做就可以了，低版本的倒计时有些问题，以下是我在uboot1.3.4上的修改

- #if defined(CONFIG_BOOTDELAY)&&(CONFIG_BOOTDELAY >= 0)

- 
# if defined(CONFIG_AUTOBOOT_KEYED)

- 
static __inline__ int abortboot(int bootdelay)

- {

- int abort = 0;

- 
    unsigned int start_tick = get_ticks();

-   uint64_t t1=endtick(1);

- 

- 
    uint64_t etime = endtick(bootdelay);

- 
    struct {

- 
        char* str;

- 
        u_int len;

- int retry;

- }

- 
    delaykey []={

- { str: getenv ("bootdelaykey"), retry: 1 },

- { str: getenv ("bootdelaykey2"), retry: 1 },

- { str: getenv ("bootstopkey"), retry: 0 },

- { str: getenv ("bootstopkey2"), retry: 0 },

- };

- 

- 
    char presskey [MAX_DELAY_STOP_STR];

- 
    u_int presskey_len = 0;

- 
    u_int presskey_max = 0;

- 
    u_int i;

- 

- 
# ifdef CONFIG_AUTOBOOT_PROMPT

-     printf(CONFIG_AUTOBOOT_PROMPT,bootdelay);

-     printf("\b\b\b\b\b\b\b\b\b");

- 
# endif

- 

- 
# ifdef CONFIG_AUTOBOOT_DELAY_STR

- if(delaykey[0].str ==NULL)

- 
        delaykey[0].str = CONFIG_AUTOBOOT_DELAY_STR;

- 
# endif

- 
# ifdef CONFIG_AUTOBOOT_DELAY_STR2

- if(delaykey[1].str ==NULL)

- 
        delaykey[1].str = CONFIG_AUTOBOOT_DELAY_STR2;

- 
# endif

- 
# ifdef CONFIG_AUTOBOOT_STOP_STR

- if(delaykey[2].str ==NULL)

- 
        delaykey[2].str = CONFIG_AUTOBOOT_STOP_STR;

- 
# endif

- 
# ifdef CONFIG_AUTOBOOT_STOP_STR2

- if(delaykey[3].str ==NULL)

- 
        delaykey[3].str = CONFIG_AUTOBOOT_STOP_STR2;

- 
# endif

- 

- for(i = 0; i < sizeof(delaykey)/ sizeof(delaykey[0]); i ++){

- 
        delaykey[i].len= delaykey[i].str ==NULL?

- 
                 0 : strlen (delaykey[i].str);

- 
        delaykey[i].len= delaykey[i].len> MAX_DELAY_STOP_STR ?

- 
                 MAX_DELAY_STOP_STR : delaykey[i].len;

- 

- 
        presskey_max = presskey_max > delaykey[i].len?

- 
                 presskey_max : delaykey[i].len;

- 

- 
# if DEBUG_BOOTKEYS

- 
        printf("%s key:<%s>\n",

- 
         delaykey[i].retry ?"delay":"stop",

- 
         delaykey[i].str ? delaykey[i].str :"NULL");

- 
# endif

- }

- 

- /*In order to keep
 up with incoming data, check timeout only

- * when catch up.

- */

- 

- while(!abort && get_ticks()<= etime){

- for(i = 0; i < sizeof(delaykey)/ sizeof(delaykey[0]); i ++){

- if(delaykey[i].len> 0 &&

- 
             presskey_len >= delaykey[i].len&&

- 
             memcmp (presskey + presskey_len - delaykey[i].len,

- 
                 delaykey[i].str,

- 
                 delaykey[i].len)== 0){

- 
# if DEBUG_BOOTKEYS

- 
                printf("got %skey\n",

- 
                 delaykey[i].retry ?"delay":"stop");

- 
# endif

- 

- 
# ifdef CONFIG_BOOT_RETRY_TIME

- /* don't retry auto boot */

- if(! delaykey[i].retry)

- 
                    retry_time =-1;

- 
# endif

- 
                abort = 1;

-   printf("\n");

- }

- }

- 

- if(get_ticks()> t1)

- {

-             bootdelay--;

-             printf("%d\b",bootdelay);

- 

-              t1=endtick(1);

- }

- 

- if(tstc()){

- if(presskey_len < presskey_max){

- 

- 
                presskey [presskey_len ++]= getc();

- }

- else{

- for(i = 0; i < presskey_max - 1; i ++)

- 
                    presskey [i]= presskey [i + 1];

- 

- 
                presskey [i]= getc();

- }

- }

- }

- 
# if DEBUG_BOOTKEYS

- if(!abort)

- 
        puts("key timeout\n");

- 
# endif

- 

- 
#ifdef CONFIG_SILENT_CONSOLE

- if(abort)

- 
        gd->flags &=~GD_FLG_SILENT;

- 
#endif

- 

- 
    return abort;

- }

- 

- 
# else/*!defined(CONFIG_AUTOBOOT_KEYED)*/




