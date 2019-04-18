# matlab/simulink中自定义c-s函数作为simulink模块使用实例 - 心纯净，行致远 - CSDN博客





2018年03月09日 22:32:54[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：2070








    一般而言matlab/simulink能满足多数使用要求，但是往往在研究中会使用User-difinition s-function或者调用c,c++的库函数，这时候就需要matlab能进行c，c++以及fortran语言的编译。

    本文是我第一次参考资料，利用c语言编写s函数的实例。User-Definition的s-fun可以作为一个simulink模块使用，这样在工程和科研中就可以自己开发需要的模块，并入simulink仿真之中，不必受制于simulink自带的有限模块。**使用c语言写s-fun需要mingw编译器。**

**    与m-sfun不同的是用c语言编写的sfun需要先经过mingw编译，生成mexw可执行文件（有资料也说是dll文件），然后才可以在自定义的s-fun模型中使用。**

题目：编写User-Definition的s-fun，使得正弦函数的波形加倍2倍，然后输出到scope。



实现步骤：

1. 新建simulink模型，拖入User-Definition的s-fun，然后双击s-fun，点击edit，进入编辑模块；

![](https://img-blog.csdn.net/2018030921572926)


2. 编写m-sfun代码，并另存为sin2.m，可以参照c-sfun的格式，具体代码如下：

```
/*
 * sfuntmpl_basic.c: Basic 'C' template for a level 2 S-function.
 *
 * Copyright 1990-2013 The MathWorks, Inc.
 */


/*
 * You must specify the S_FUNCTION_NAME as the name of your S-function
 * (i.e. replace sfuntmpl_basic with the name of your S-function).
 */

#define S_FUNCTION_NAME  sin2   //这里的sin2一定要与c文件的文件名一致
#define S_FUNCTION_LEVEL 2

/*Level 2 M文件S函数----支持访问更多地S函数API，支持代码生成。当你要使用M文件
 * 来实现一个S函数的时候，选择Level 2 M文件S函数。
 */

/*
 * Need to include simstruc.h for the definition of the SimStruct and
 * its associated macro definitions.
 */
#include "simstruc.h"
//程序里用到的头文件在这里引用

/*====================*
 * S-function methods *
 *====================*/

/* Function: mdlInitializeSizes ===============================================
 * Abstract:
 *    The sizes information is used by Simulink to determine the S-function
 *    block's characteristics (number of inputs, outputs, states, etc.).
 */
static void mdlInitializeSizes(SimStruct *S)
{
 //这个函数用来设置输入、输出和参数
    ssSetNumSFcnParams(S, 0);  /* Number of expected parameters */
    //设置参数个数，这里为0
    if (ssGetNumSFcnParams(S) != ssGetSFcnParamsCount(S)) {
        /* Return if number of expected != number of actual parameters */
        return;
    }

   // ssSetNumContStates(S, 0);   //设置连续状态的个数，缺省为0
  //  ssSetNumDiscStates(S, 0);   //设置离散状态的个数，缺省为0

    if (!ssSetNumInputPorts(S, 1)) return;  //设置输入变量的个数，这里为1
    ssSetInputPortWidth(S, 0, DYNAMICALLY_SIZED);   //设置输入变量0的维数为动态的
   // ssSetInputPortRequiredContiguous(S, 0, 1); /*direct input signal access*/
    /*
     * Set direct feedthrough flag (1=yes, 0=no).
     * A port has direct feedthrough if the input is used in either
     * the mdlOutputs or mdlGetTimeOfNextVarHit functions.
     */
    /*
     *设置input0的访问方式，true就是临近访问，这样指针的增量后
     * 就可以直接访问下个input端口
     */
    ssSetInputPortDirectFeedThrough(S, 0, 1);
    //设置输入端口的信号是否在mdlOutputs函数中使用，这里设置为true
    if (!ssSetNumOutputPorts(S, 1)) return; 
    //设置输出变量的个数，这里为1
    ssSetOutputPortWidth(S, 0, DYNAMICALLY_SIZED);    //设置输出变量0的维数为动态
   // ssSetOutputPortWidth(S, 1, 1);    //设置输出变量1的维数为1
    
    ssSetNumSampleTimes(S, 1);  //设置采样时间1s
    //ssSetNumRWork(S, 0);
   // ssSetNumIWork(S, 0);
    //ssSetNumPWork(S, 0);
    //ssSetNumModes(S, 0);
   // ssSetNumNonsampledZCs(S, 0);

    /* Specify the sim state compliance to be same as a built-in block */
   // ssSetSimStateCompliance(S, USE_DEFAULT_SIM_STATE);

    ssSetOptions(S, SS_OPTION_EXCEPTION_FREE_CODE);
 
}

/* Function: mdlInitializeSampleTimes =========================================
 * Abstract:
 *    This function is used to specify the sample time(s) for your
 *    S-function. You must register the same number of sample times as
 *    specified in ssSetNumSampleTimes.
 */
static void mdlInitializeSampleTimes(SimStruct *S)
{
    ssSetSampleTime(S, 0, INHERITED_SAMPLE_TIME);
    ssSetOffsetTime(S, 0, 0.0);

}

//#define MDL_INITIALIZE_CONDITIONS   /* Change to #undef to remove function */
//#if defined(MDL_INITIALIZE_CONDITIONS)
  /* Function: mdlInitializeConditions ========================================
   * Abstract:
   *    In this function, you should initialize the continuous and discrete
   *    states for your S-function block.  The initial states are placed
   *    in the state vector, ssGetContStates(S) or ssGetRealDiscStates(S).
   *    You can also perform any other initialization activities that your
   *    S-function may require. Note, this routine will be called at the
   *    start of simulation and if it is present in an enabled subsystem
   *    configured to reset states, it will be call when the enabled subsystem
   *    restarts execution to reset the states.
   */
 
static void mdlOutputs(SimStruct *S, int_T tid)
{
   int_T i;
    InputRealPtrsType uPtrs = ssGetInputPortRealSignalPtrs ( S,0 );
    real_T *y = ssGetOutputPortRealSignal (S,0 );
    int_T width = ssGetOutputPortWidth ( S,0 );
    for ( i=0;i<width;i++ ) {
    *y++ = 2.0 *( *uPtrs[ i ] );
                             }
}

static void mdlTerminate(SimStruct *S)
{
}

/*=============================*
 * Required S-function trailer *
 *=============================*/

#ifdef  MATLAB_MEX_FILE    /* Is this file being compiled as a MEX-file? */
#include "simulink.c"      /* MEX-file interface mechanism */
#else
#include "cg_sfun.h"       /* Code generation registration function */
#endif
```

3. sin2.c完成之后，需要生成sin2.mexw，在命令行键入 mex sin2.c即可

```
>> mex sin2.c
使用 'MinGW64 Compiler (C)' 编译。
MEX 已成功完成。
```


4. 将matlab工作目录更改为sin2.mexw所在的文件夹，然后双击simulink模块，将sin2填入s-function name，由于本例不需要输入参数，故其他不需要填写。

![](https://img-blog.csdn.net/20180309223036127)


5. 运行。




![](https://img-blog.csdn.net/20180309223159555)







