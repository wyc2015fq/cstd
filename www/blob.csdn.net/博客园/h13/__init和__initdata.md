# __init和__initdata - h13 - 博客园
   Linux在arch/$(ARCH)/kernel/vmlinux.lds中定义了.init段。__init和__initdata属性的数据都在这个段中，当内核启动完毕后，这个段中的内存会被释放掉供其他使用。
__init和__initdata宏定义如下：
/* include/linux/init.c */
```
#define __init __attribute__ ((__section__ (".init.text")))
#define __initdata __attribute__ ((__section__ (".init.data")))
```
vmlinux.lds内容如下：
/* arch/arm/kernel/vmlinux.lds */
```
/* ld script to make ARM Linux kernel
 * taken from the i386 version by Russell King
 * Written by Martin Mares <mj@atrey.karlin.mff.cuni.cz>
 */
#include <asm-generic/vmlinux.lds.h>
#include <asm/thread_info.h>
#include <asm/memory.h>
    
OUTPUT_ARCH(arm)
ENTRY(stext)
#ifndef __ARMEB__
jiffies = jiffies_64;
#else
jiffies = jiffies_64 + 4;
#endif
SECTIONS
{
#ifdef CONFIG_XIP_KERNEL
    . = XIP_VIRT_ADDR(CONFIG_XIP_PHYS_ADDR);
#else
    . = PAGE_OFFSET + TEXT_OFFSET;
#endif
    .text.head : {
        _stext = .;
        _sinittext = .;
        *(.text.head)
    }
    .init : {            /* Init code and data        */
            INIT_TEXT
        _einittext = .;
        __proc_info_begin = .;
            *(.proc.info.init)
        __proc_info_end = .;
        __arch_info_begin = .;
            *(.arch.info.init)
        __arch_info_end = .;
        __tagtable_begin = .;
            *(.taglist.init)
        __tagtable_end = .;
        . = ALIGN(16);
        __setup_start = .;
            *(.init.setup)
        __setup_end = .;
        __early_begin = .;
            *(.early_param.init)
        __early_end = .;
        __initcall_start = .;
            INITCALLS
        __initcall_end = .;
        __con_initcall_start = .;
            *(.con_initcall.init)
        __con_initcall_end = .;
        __security_initcall_start = .;
            *(.security_initcall.init)
        __security_initcall_end = .;
#ifdef CONFIG_BLK_DEV_INITRD
        . = ALIGN(32);
        __initramfs_start = .;
            usr/built-in.o(.init.ramfs)
        __initramfs_end = .;
#endif
        . = ALIGN(4096);
        __per_cpu_load = .;
        __per_cpu_start = .;
            *(.data.percpu.page_aligned)
            *(.data.percpu)
            *(.data.percpu.shared_aligned)
        __per_cpu_end = .;
#ifndef CONFIG_XIP_KERNEL
        __init_begin = _stext;
        INIT_DATA
        . = ALIGN(4096);
        __init_end = .;
#endif
    }
    /DISCARD/ : {            /* Exit code and data        */
        EXIT_TEXT
        EXIT_DATA
        *(.exitcall.exit)
        *(.ARM.exidx.exit.text)
        *(.ARM.extab.exit.text)
#ifndef CONFIG_MMU
        *(.fixup)
        *(__ex_table)
#endif
    }
    .text : {            /* Real text segment        */
        _text = .;        /* Text and read-only data    */
            __exception_text_start = .;
            *(.exception.text)
            __exception_text_end = .;
            TEXT_TEXT
            SCHED_TEXT
            LOCK_TEXT
            KPROBES_TEXT
#ifdef CONFIG_MMU
            *(.fixup)
#endif
            *(.gnu.warning)
            *(.rodata)
            *(.rodata.*)
            *(.glue_7)
            *(.glue_7t)
        *(.got)            /* Global offset table        */
    }
    RODATA
    _etext = .;            /* End of text and rodata section */
#ifdef CONFIG_ARM_UNWIND
    /*
     * Stack unwinding tables
     */
    . = ALIGN(8);
    .ARM.unwind_idx : {
        __start_unwind_idx = .;
        *(.ARM.exidx*)
        __stop_unwind_idx = .;
    }
    .ARM.unwind_tab : {
        __start_unwind_tab = .;
        *(.ARM.extab*)
        __stop_unwind_tab = .;
    }
#endif
#ifdef CONFIG_XIP_KERNEL
    __data_loc = ALIGN(4);        /* location in binary */
    . = PAGE_OFFSET + TEXT_OFFSET;
#else
    . = ALIGN(THREAD_SIZE);
    __data_loc = .;
#endif
    .data : AT(__data_loc) {
        _data = .;        /* address in memory */
        /*
         * first, the init task union, aligned
         * to an 8192 byte boundary.
         */
        *(.data.init_task)
#ifdef CONFIG_XIP_KERNEL
        . = ALIGN(4096);
        __init_begin = .;
        INIT_DATA
        . = ALIGN(4096);
        __init_end = .;
#endif
        . = ALIGN(4096);
        __nosave_begin = .;
        *(.data.nosave)
        . = ALIGN(4096);
        __nosave_end = .;
        /*
         * then the cacheline aligned data
         */
        . = ALIGN(32);
        *(.data.cacheline_aligned)
        /*
         * The exception fixup table (might need resorting at runtime)
         */
        . = ALIGN(32);
        __start___ex_table = .;
#ifdef CONFIG_MMU
        *(__ex_table)
#endif
        __stop___ex_table = .;
        /*
         * and the usual data section
         */
        DATA_DATA
        CONSTRUCTORS
        _edata = .;
    }
    _edata_loc = __data_loc + SIZEOF(.data);
    .bss : {
        __bss_start = .;    /* BSS                */
        *(.bss)
        *(COMMON)
        _end = .;
    }
                    /* Stabs debugging sections.    */
    .stab 0 : { *(.stab) }
    .stabstr 0 : { *(.stabstr) }
    .stab.excl 0 : { *(.stab.excl) }
    .stab.exclstr 0 : { *(.stab.exclstr) }
    .stab.index 0 : { *(.stab.index) }
    .stab.indexstr 0 : { *(.stab.indexstr) }
    .comment 0 : { *(.comment) }
}
/*
 * These must never be empty
 * If you have to comment these two assert statements out, your
 * binutils is too old (for other reasons as well)
 */
ASSERT((__proc_info_end - __proc_info_begin), "missing CPU support")
ASSERT((__arch_info_end - __arch_info_begin), "no machine record defined")
```
可以发现__init对应的section(.init.text)和__initdata对应的section(.init.data)都在.init段中。同样,这里定义的其他一些section也都会在使用完后被释放，如.init.setup，.initcall1.init等。
释放memory的大小会在系统启动过程中打印出来：
```csharp;gutter
eth0: link up
IP-Config: Complete:
device=eth0, addr=192.168.167.15, mask=255.255.255.0, gw=192.168.167.254,
host=192.168.167.15, domain=, nis-domain=(none),
bootserver=192.168.167.170, rootserver=192.168.167.170, rootpath=
Looking up port of RPC 100003/2 on 192.168.167.170
Looking up port of RPC 100005/1 on 192.168.167.170
VFS: Mounted root (nfs filesystem).
Freeing init memory: 128K
```

