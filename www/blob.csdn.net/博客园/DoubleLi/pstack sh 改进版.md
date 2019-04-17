# pstack.sh 改进版 - DoubleLi - 博客园






pstack.sh 改进版本



```
![复制代码](https://common.cnblogs.com/images/copycode.gif)


#!/bin/bash
if (( $# < 1 )) ; then
    echo "usage: `basename $0` pid" 1>&2
    exit 1
fi  
if [[ ! -r /proc/$1 ]] ;  then
    echo "Process $1 not found." 1>&2
    exit 1
fi  
(gdb -quiet -nx /proc/$1/exe -p $1 <<EOF
define my_dump_all_stack
x \$pc
bt
end
thread apply all my_dump_all_stack
EOF
) | sed -n -e 's/^(gdb) //' -e '/^#/p' -e '/^Thread/p' -e's/^0x/#*  0x/p'


![复制代码](https://common.cnblogs.com/images/copycode.gif)
```









