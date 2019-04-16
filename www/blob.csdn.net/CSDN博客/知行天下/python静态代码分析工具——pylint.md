# python静态代码分析工具——pylint - 知行天下 - CSDN博客





2014年08月16日 21:38:39[小尼人00](https://me.csdn.net/robby_chan)阅读数：956







|1·下载地址：[http://www.logilab.org/project/pylint](http://www.logilab.org/project/pylint)2·安装：pylint-0.22.0.tar.gz解压，然后python setup.py install安装过程会自动下载并安装logilab-astng-0.21.0.tar.gz  logilab-common-0.53.0.tar.gz这两个工具。3·使用语法：pylint 参数 py脚本如果不加任何参数，会有一个"No config file found, using default configuration"这样的提示，解决办法，可以生成一个默认的配置文件pylint --generate-rcfile > pylint.conf然后再执行pylint --rcfile=pylint.conf ……4·参考> http://www.ibm.com/developerworks/cn/linux/l-cn-pylint/index.html(C) 惯例。违反了编码风格标准(R) 重构。写得非常糟糕的代码。(W) 警告。某些 Python 特定的问题。(E) 错误。很可能是代码中的错误。(F) 致命错误。阻止 Pylint 进一步运行的错误。5·总结：    操作符的左右要加空格    自定义的变量要大写并简短（([A-Z_][A-Z0-9_]*)|(__.*__))$）    每行的代码不要太长----------------------------------------------------------附上：Pylint 的常用命令行参数    * -h,--help      显示所有帮助信息。    * --generate-rcfile      可以使用 pylint --generate-rcfile 来生成一个配置文件示例。可以使用重定向把这个配置文件保存下来用做以后使用。也可以在前面加上其它选项，使这些选项的值被包含在这个产生的配置文件里。如：pylint --persistent=n --generate-rcfile > pylint.conf，查看 pylint.conf，可以看到 persistent=no，而不再是其默认值 yes。    * --rcfile=<file>      指定一个配置文件。把使用的配置放在配置文件中，这样不仅规范了自己代码，也可以方便地和别人共享这些规范。    * -i <y_or_n>, --include-ids=<y_or_n>      在输出中包含 message 的 id, 然后通过 pylint --help-msg=<msg-id>来查看这个错误的详细信息，这样可以具体地定位错误。    * -r <y_or_n>, --reports=<y_or_n>      默认是 y, 表示 Pylint 的输出中除了包含源代码分析部分，也包含报告部分。    * --files-output=<y_or_n>      将每个 module /package 的 message 输出到一个以 pylint_module/package. [txt|html] 命名的文件中，如果有 report 的话，输出到名为 pylint_global.[txt|html] 的文件中。默认是输出到屏幕上不输出到文件里。    * -f <format>, --output-format=<format>      设置输出格式。可以选择的格式有 text, parseable, colorized, msvs (visual studio) 和 html, 默认的输出格式是 text。    * --disable-msg= <msg ids>      禁止指定 id 的 message. 比如说输出中包含了 W0402 这个 warning 的 message, 如果不希望它在输出中出现，可以使用 --disable-msg= W0402|
|----|



