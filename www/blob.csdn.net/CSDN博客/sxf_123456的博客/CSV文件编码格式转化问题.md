# CSV文件编码格式转化问题 - sxf_123456的博客 - CSDN博客
2017年08月08日 14:16:36[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1145
                
iconv -f utf-8 -t utf-8 cy170730.csv > cy.txt
[root@znwg-app-db test]# iconv --help
用法： iconv [选项...] [文件...]
转换给定文件的编码。
 输入/输出格式规范：
  -f, --from-code=名称     原始文本编码
  -t, --to-code=名称       输出编码
 信息：
  -l, --list                 列举所有已知的字符集
 输出控制：
  -c                         从输出中忽略无效的字符
  -o, --output=FILE          输出文件
  -s, --silent               关闭警告
      --verbose              打印进度信息
  -?, --help                 给出该系统求助列表
      --usage                给出简要的用法信息
  -V, --version              打印程序版本号
            
