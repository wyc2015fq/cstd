
# Android文件的分割和组装 - 萌萌的It人 www.itmmd.com - CSDN博客


2013年12月02日 15:11:52[Jlins](https://me.csdn.net/dyllove98)阅读数：2021


使用UDP数据包发送文件时，由于[UDP数据包大小](http://blog.csdn.net/buptzwp/article/details/5055487)的限制，一个文件要放在几个UDP数据包里面发送，这就需要把一个文件分割成若干部分，分别放进若干个UDP数据包里面，在接收端，收到这些UDP数据包以后，再对文件进行组装，从而得到一个完整的文件。
定义的相关变量：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|//
 要分割的文件
|public|static|RandomAccessFile
 raf_split;
|//
 要合并的文件
|public|static|RandomAccessFile
 raf_merge;
|//
 文件长度
|public|static|long|len;
|//
 Byte数组
|public|static|int|offset;
|public|static|int|os
 =|5|;
|public|static|int|size
 =|1024|-os;
|public|static|byte|file_data[]
 =|new|byte|[|1024|];
|

FileOperClass类实现文件的分割和组装操作：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|//
 构造函数(0-分割文件,1-合并文件)
|public|FileOperClass(String
 file,|int|x){
|//
 分割文件
|if|(x
 ==|0|){
|try|{
|//
 RandomAccessFile以只读方式打开文件
|raf_split
 =|new|RandomAccessFile(file,|"r"|);
|//
 获得文件大小
|len
 = raf_split.length();
|//
 需要几个数据包
|pnum
 = (|int|)
 Math.ceil((raf_split.length()*|1.0|)/(size
 *|1.0|))
 +|1|;
|//
 最后一个数据包里面数据多少
|pmod
 = (|int|)
 (raf_split.length() - (pnum -|2|)*
 size);
|//
 分割文件
|split();
|}
|catch|(Exception
 e){
|}
|}
|//
 合并文件
|else|if|(x
 ==|1|){
|try|{
|//
 RandomAccessFile以读写方式打开文件
|raf_merge
 =|new|RandomAccessFile(file,|"rw"|);
|//
 合并文件
|merge();
|}
|catch|(Exception
 e){
|}
|}
|}
|

分割文件：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|40
|41
|42
|43
|44
|45
|46
|47
|48
|49
|50
|51
|52
|53
|54
|55
|56
|57
|58
|59
|60
|61
|//
 分割文件并发送
|public|static|void|split(){
|int|m1,m2;
|p_id
 =|0|;
|offset
 =|0|;
|try|{
|while|(len>|0|){
|//
 数据包类型
|file_data[|0|]
 = (|byte|)|2|;
|//
 客户端ID
|file_data[|1|]
 = (|byte|)
 MainActivity.cli_id;
|//
 会话ID
|file_data[|2|]
 = (|byte|)
 MainActivity.ses_id;
|//
 该会话数据包数量
|file_data[|3|]
 = (|byte|)
 pnum;
|//
 数据包ID
|file_data[|4|]
 = (|byte|)
 p_id;
|//
 seek
|raf_split.seek(offset);
|//
 读出数据到file_data
|raf_split.read(file_data,
 os, size);
|//
 发送数据包
|MainActivity.trd_send.set_action(|2|,
 file_data);
|len
 = len - size;
|p_id
 = p_id +|1|;
|offset
 = offset + size;
|}
|//
 记录最后一个数据包剩余字节的数据包
|//
 数据包类型
|file_data[|0|]
 = (|byte|)|2|;
|//
 客户端ID
|file_data[|1|]
 = (|byte|)
 MainActivity.cli_id;
|//
 会话ID
|file_data[|2|]
 = (|byte|)
 MainActivity.ses_id;
|//
 该会话数据包数量
|file_data[|3|]
 = (|byte|)
 pnum;
|//
 数据包ID
|file_data[|4|]
 = (|byte|)
 p_id;
|m1
 = pmod /|128|;
|m2
 = pmod %|128|;
|file_data[|5|]
 = (|byte|)
 m1;
|file_data[|6|]
 = (|byte|)
 m2;
|//
 发送数据包
|MainActivity.trd_send.set_action(|2|,
 file_data);
|}
|catch|(Exception
 e){
|}
|finally|{
|//
 关闭文件
|try|{
|raf_split.close();
|}
|catch|(Exception
 err){
|}
|}
|}
|

合并文件：
|1
|2
|3
|4
|5
|6
|7
|8
|9
|10
|11
|12
|13
|14
|15
|16
|17
|18
|19
|20
|21
|22
|23
|24
|25
|26
|27
|28
|29
|30
|31
|32
|33
|34
|35
|36
|37
|38
|39
|//
 合并文件
|public|static|void|merge(){
|byte|[][]
 tmp_byte =|new|byte|[MainActivity.mer_pkt_num][|1024|];
|int|i,j;
|try|{
|for|(i=|0|;
 i<MainActivity.r_datapacket.size(); i++){
|//
 判断数据包是否已经齐全
|if|((MainActivity.r_datapacket.get(i).c_id
 == MainActivity.mer_cli_id)
|&&
 (MainActivity.r_datapacket.get(i).ses_id == MainActivity.mer_ses_id))
|{
|//
 读出数据包的数据进byte数组
|tmp_byte[MainActivity.r_datapacket.get(i).p_id]
 =
|MainActivity.r_datapacket.get(i).b;
|}
|}
|for|(i=|0|;
 i<MainActivity.mer_pkt_num-|2|;
 i++){
|//
 把byte数组写进文件
|raf_merge.write(tmp_byte[i],
 os, size);
|}
|//
 最后一个byte数组写进文件
|raf_merge.write(tmp_byte[MainActivity.mer_pkt_num-|1|],
|os,
 MainActivity.mer_pkt_mod );
|}
|catch|(Exception
 e){
|}
|finally|{
|//
 关闭文件
|try|{
|raf_merge.close();
|}
|catch|(Exception
 err){
|}
|}
|}
|



