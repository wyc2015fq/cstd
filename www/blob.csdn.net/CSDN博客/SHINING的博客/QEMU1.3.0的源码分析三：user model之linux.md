# QEMU1.3.0的源码分析三：user model之linux - SHINING的博客 - CSDN博客
2012年12月11日 11:23:07[snsn1984](https://me.csdn.net/snsn1984)阅读数：3284
                
从源码目录来看，user model有两块内容bsd-user和linux-user。我主要研究了下linux-user这种情况。
首先要提一下通常容易关注的焦点，linux-user下的函数入口点：/源码目录/linux-user/main.c中的
Line:3388    int main(int argc, char **argv, char **envp).
找到了入口函数，就可以根据这个main函数中的调用关系来看看这个情况下的主要执行流程和动作了。
int main(int argc, char **argv, char **envp)
{
    module_call_init(MODULE_INIT_QOM);
    qemu_cache_utils_init(envp);
    /*初始化了tcg的相关部分，包含了cpu动态转化的一些初始化操作。*/
    tcg_exec_init(0);
    cpu_exec_init_all();
    /*包含了虚拟cpu的初始化*/
    env = cpu_init(cpu_model);
    /*加载可执行程序，即Guest code*/
    ret = loader_exec(filename, target_argv, target_environ, regs,
        info, &bprm);
    target_set_brk(info->brk);
    /*系统调用初始化*/
    syscall_init();
    /*信号初始化*/
    signal_init();
   /*此函数是主要的循环体，通过这个函数来实现对指令的动态翻译，并且执行翻译之后的Host Code。
   通过最终调用cpu_gen_code()函数（位于translate-all.c文件中）来实现
    动态翻译，其中调用了两个关键函数。一个关键函数是gen_intermediate_code()
   函数（位于target-arm/translate.c，此处以guest指令集为arm为例，其他的可以自行替换），
   这个函数的主要功能是根据Guest Code生成TCG Operations。另外一个重要的函数是
  tcg_gen_code()函数(位于tcg/tcg.c)，这个函数主要是把TCG Operations转化成Host code。*/
    cpu_loop(env);
    /* never exits */
    return 0;
}
下面来分析下刚才介绍的重要函数cpu_loop(). cpu_loop()函数在linux-user/main.c中有多个版本，区别在于参数，参数是不同的cpu state，下面举例仍然以arm为主。
void cpu_loop(CPUARMState *env)
{
    int trapnr;
    unsigned int n, insn;
    target_siginfo_t info;
    uint32_t addr;
    for(;;) {
        cpu_exec_start(env);
        trapnr = cpu_arm_exec(env);
        cpu_exec_end(env);
   ...............
}

可以看到for循环里有三个函数调用，分别是cpu_exec_start，cpu_arm_exec，cpu_exec_end。其中最重要的
cpu_arm_exec函数，通过target-arm/cpu.h中的宏定义#define cpu_exec cpu_arm_exec调用了cpu-exec.c文件
中的cpu_exec()函数。
cpu_exec()是整个qemu中的一个重要函数，它负责整个核心的从guest code 到host code的翻译和执行。
cpu_exec()首先会去调用tb_find_fast()，tb_find_fast()会判断取回来的tb是否合法，如果不合法会去调用tb_find_slow()函数。
tb_find_slow()会试图通过物理mapping去寻找tb，如果寻找失败则会调用tb_gen_code()去翻译代码。
cpu_exec()函数调用tb_find_fast()之后会调用tcg_qemu_tb_exec()去执行所找到的tb。最后再调用cpu_exec_nocache()去执行剩下的代码。

