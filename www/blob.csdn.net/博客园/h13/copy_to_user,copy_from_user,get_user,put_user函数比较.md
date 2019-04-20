# copy_to_user,copy_from_user,get_user,put_user函数比较 - h13 - 博客园
copy_to_user --  Copy a block of data into user space. 
copy_from_user --  Copy a block of data from user space.
get_user --  Get a simple variable from user space. 
put_user --  Write a simple value into user space. 
copy_from_user
Name
copy_from_user --  Copy a block of data from user space. 
Synopsis
unsigned long copy_from_user (void * to, const void __user * from, unsigned long n);
Arguments
to 
Destination address, in kernel space. 
from 
Source address, in user space. 
n 
Number of bytes to copy. 
Context
User context only. This function may sleep. 
Description
Copy data from user space to kernel space. 
Returns number of bytes that could not be copied. On success, this will be zero. 
If some data could not be copied, this function will pad the copied data to the requested size using zero bytes. 
copy_to_user
Name
copy_to_user --  Copy a block of data into user space. 
Synopsis
unsigned long copy_to_user (void __user * to, const void * from, unsigned long n);
Arguments
to 
Destination address, in user space. 
from 
Source address, in kernel space. 
n 
Number of bytes to copy. 
Context
User context only. This function may sleep. 
Description
Copy data from kernel space to user space. 
Returns number of bytes that could not be copied. On success, this will be zero. 
put_user
Name
put_user --  Write a simple value into user space. 
Synopsis
put_user ( x, ptr);
Arguments
x 
Value to copy to user space. 
ptr 
Destination address, in user space. 
Context
User context only. This function may sleep. 
Description
This macro copies a single simple value from kernel space to user space. It supports simple types like char and int, but not larger data types like structures or arrays. 
ptr must have pointer-to-simple-variable type, and x must be assignable to the result of dereferencing ptr. 
Returns zero on success, or -EFAULT on error. 
get_user
Name
get_user --  Get a simple variable from user space. 
Synopsis
get_user ( x, ptr);
Arguments
x 
Variable to store result. 
ptr 
Source address, in user space. 
Context
User context only. This function may sleep. 
Description
This macro copies a single simple variable from user space to kernel space. It supports simple types like char and int, but not larger data types like structures or arrays. 
ptr must have pointer-to-simple-variable type, and the result of dereferencing ptr must be assignable to x without a cast. 
Returns zero on success, or -EFAULT on error. On error, the variable x is set to zero.
本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/do2jiang/archive/2010/04/02/5445960.aspx](http://blog.csdn.net/do2jiang/archive/2010/04/02/5445960.aspx)
