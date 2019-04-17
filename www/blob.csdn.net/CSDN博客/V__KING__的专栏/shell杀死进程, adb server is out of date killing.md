# shell杀死进程, adb server is out of date. killing - V__KING__的专栏 - CSDN博客





2017年09月01日 21:32:48[v__king__](https://me.csdn.net/V__KING__)阅读数：297标签：[shell																[android驱动人生](https://so.csdn.net/so/search/s.do?q=android驱动人生&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)







## 解决问题adb server is out of date. killing

Android在用运行logcat的时候，会出现上面错误，是因为adb端口5037被占用了，运行如下脚本之后，再也不会担心出现同样的错误了。 

原理是通过netstat命令找到占用5037端口的进程号，然后kill该进程号，最后在运行logcat命令就ok了。

```bash
### Solve the problem: "adb server is out of date. killing"

### Must input a string
### Example: ./logcat.sh adb

function help_print(){
    echo "################################################"
    echo ""
    echo "########   Please Input a String For Search.." 
    echo "########   Usege: $0 Logcat_string" 
    echo ""
    echo "################################################"
}

##### main
### If have a Input String for logcat
#if [ ! -n "$1" ]; then
#if [[ "$1" == "" ]]; then
# Be carefull, have a space
if [[ ! $1 ]]; then
    help_print
    exit
fi

adb_pids=`netstat -anp|grep '5037'|awk '{print $7}'`
echo $adb_pids

### get adb's pid num
for pid in $adb_pids; do
    echo ${pid%/*}   # trunck the left string
    if [[ "$pid" == +([0-9]) ]]; then
        PID_ADB=${pid%/*}
        echo "killing adb pid=$pid"
        ### if have pid num ,kill adb's pid, else logcat
        if [[ $PID_ADB != '' ]]; then
            kill $PID_ADB
        fi
    fi
done

adb logcat|egrep $1 --color
```




