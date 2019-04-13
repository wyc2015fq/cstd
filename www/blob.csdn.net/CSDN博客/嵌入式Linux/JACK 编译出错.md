
# JACK 编译出错 - 嵌入式Linux - CSDN博客

2017年12月29日 09:16:09[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：179



```python
FAILED:/bin/bash -c "(prebuilts/sdk/tools/jack-admin install-serverprebuilts/sdk/tools/jack-launcher.jar prebuilts/sdk/tools/jack-server-4.8.ALPHA.jar2>&1 || (exit 0) ) &&(JACK_SERVER_VM_ARGUMENTS=\"-Dfile.encoding=UTF-8-XX:+TieredCompilation\" prebuilts/sdk/tools/jack-admin start-server2>&1 || exit 0 ) && (prebuilts/sdk/tools/jack-admin updateserver prebuilts/sdk/tools/jack-server-4.8.ALPHA.jar 4.8.ALPHA 2>&1 ||exit 0 ) && (prebuilts/sdk/tools/jack-admin update jackprebuilts/sdk/tools/jacks/jack-2.28.RELEASE.jar 2.28.RELEASE || exit 47;prebuilts/sdk/tools/jack-admin update jackprebuilts/sdk/tools/jacks/jack-3.36.CANDIDATE.jar 3.36.CANDIDATE || exit 47;prebuilts/sdk/tools/jack-admin update jackprebuilts/sdk/tools/jacks/jack-4.7.BETA.jar 4.7.BETA || exit 47 )"
Jack server already installed in"/home/local/ACTIONS/songzhining/.jack-server"
Launching Jack server java -XX:MaxJavaStackTraceDepth=-1 -Djava.io.tmpdir=/tmp-Dfile.encoding=UTF-8 -XX:+TieredCompilation -cp/home/local/ACTIONS/songzhining/.jack-server/launcher.jarcom.android.jack.launcher.ServerLauncher
Jack server failed to (re)start, try 'jack-diagnose' or see Jack server log
No Jack server running. Try 'jack-admin start-server'
No Jack server running. Try 'jack-admin start-server'
ninja: build stopped: subcommand failed.
make[1]: *** [ninja_wrapper] Error 1
```

1、修改 weiqifa@ubuntu:~$ vim .jack-settings  把里面的两个端口修改掉
2、修改weiqifa@ubuntu:~$ vim .jack-server/config.properties 把里面的连个端口修改掉，对应要和上面的端口一样


