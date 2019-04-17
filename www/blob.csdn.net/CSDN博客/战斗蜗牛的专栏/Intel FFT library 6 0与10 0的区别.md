# Intel FFT library 6.0与10.0的区别 - 战斗蜗牛的专栏 - CSDN博客





2011年11月13日 19:36:51[vbskj](https://me.csdn.net/vbskj)阅读数：1763








10.0的话，语法有相当大的改变，此外相对于6.0在做IFFT的时候还要提前设置scale因子，一般为1/n.

1.   在6.0中，申请3*n大小的变量wsave,然后进行傅里叶变换即可：

      CFFT1DC(rr, ri, n, 0, wsave);

      CFFT1DC(rr, ri, n, isign, wsave);  //isign =-1为FFT， isign = 1为 IFFT





2.  在10.0中，傅里叶变换的步骤是

  1） 变量名称，mkl_float_complex*

  2）/* Create Dfti descriptor for 1D single precision  transform
*/
Status = DftiCreateDescriptor( &Desc_Handle, DFTI_SINGLE,
DFTI_COMPLEX, 1, n );

  3）/*   Commit Dfti descriptor
*/
Status = DftiCommitDescriptor( Desc_Handle );
if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
dfti_example_status_print(Status);
printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
}

   4）/*   Compute Forward transform
*/

Status = DftiComputeForward( Desc_Handle, x_in);
if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
dfti_example_status_print(Status);
printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
}

   5）/*   Set Scale number for Backward transform
*/
Scale = 1.0/(float)n;
Status = DftiSetValue(Desc_Handle, DFTI_BACKWARD_SCALE, Scale);
if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
dfti_example_status_print(Status);
printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
}

   6）/*
/   Commit Dfti descriptor
*/
Status = DftiCommitDescriptor( Desc_Handle );
if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
dfti_example_status_print(Status);
printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
}
/*
/   Compute Backward transform
*/
printf("\n Compute DftiComputeBackward\n\n");
Status = DftiComputeBackward( Desc_Handle, x_in);
if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
dfti_example_status_print(Status);
printf(" TEST FAIL\n"); goto FREE_DESCRIPTOR;
}




   7）释放描述器资源

       Status = DftiFreeDescriptor(&Desc_Handle);
if(! DftiErrorClass(Status, DFTI_NO_ERROR)){
dfti_example_status_print(Status);
printf(" TEST FAIL\n");
}



