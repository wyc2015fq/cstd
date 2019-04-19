# logstash如何将kakfa合并的数据拆分然后写入ES - sxf_123456的博客 - CSDN博客
2017年08月03日 11:12:50[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：1304
kafka数据：{
{"cluster":"qy_api_v2_pool","body_bytes_sent":"8579","http_versioncode":"Android_32"}\n
{"cluster":"qy_api_v2_pool","body_bytes_sent":"8579","http_versioncode":"Android_33"}\n
{"cluster":"qy_api_v2_pool","body_bytes_sent":"8579","http_versioncode":"Android_34"}\n
....
}
kafka团队因考虑性能问题，将原始日志多条合并一条发送（每一条用换行符分割），这样我读的kafka就必须拆成一条一条的写入到ES，不然数据就不准确了，请问这种需求该如何处理呢？
已解决，开始走了弯路，用的下列方法导致还在一条数据
filter {
      mutate {
              split=>["message","
"]
      }
> 
正解方案
filter {
        split {
                        field => "message"
               }
还有一个小问题split中terminator默认是\n,但是我如下写法为什么切割不成功呢，不写terminator是可以的
filter {
        split {
                        field => "message"
                        terminator => "\\n"
               }
