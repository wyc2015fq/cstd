
const char* curl_easy_strerror_cn(CURLcode code)
{  
    switch (code)  
    {  
    case CURLE_UNSUPPORTED_PROTOCOL:  
        return "您传送给 libcurl 的网址使用了此 libcurl 不支持的协议。 可能是您没有使用的编译时选项造成了这种情况（可能是协议字符串拼写有误，或没有指定协议 libcurl 代码）。";  
  
    case CURLE_FAILED_INIT:  
        return "非常早期的初始化代码失败。 可能是内部错误或问题。";  
  
    case CURLE_URL_MALFORMAT:  
        return "网址格式不正确。";  
  
    case CURLE_COULDNT_RESOLVE_PROXY:  
        return "无法解析代理服务器。 指定的代理服务器主机无法解析。";  
  
    case CURLE_COULDNT_RESOLVE_HOST:  
        return "无法解析主机。 指定的远程主机无法解析。";  
  
    case CURLE_COULDNT_CONNECT:  
        return "无法通过 connect() 连接至主机或代理服务器。";  
  
    case CURLE_FTP_WEIRD_SERVER_REPLY:  
        return "在连接到 FTP 服务器后，libcurl 需要收到特定的回复。 此错误代码表示收到了不正常或不正确的回复。 指定的远程服务器可能不是正确的 FTP 服务器。";  
  
    case CURLE_REMOTE_ACCESS_DENIED:  
        return "我们无法访问网址中指定的资源。 对于 FTP，如果尝试更改为远程目录，就会发生这种情况。";  
  
    case CURLE_FTP_WEIRD_PASS_REPLY:  
        return "在将 FTP 密码发送到服务器后，libcurl 需要收到正确的回复。 此错误代码表示返回的是意外的代码。";  
  
    case CURLE_FTP_WEIRD_PASV_REPLY:  
        return "libcurl 无法从服务器端收到有用的结果，作为对 PASV 或 EPSV 命令的响应。 服务器有问题。";  
  
    case CURLE_FTP_WEIRD_227_FORMAT:  
        return "FTP 服务器返回 227 行作为对 PASV 命令的响应。 如果 libcurl 无法解析此行，就会返回此代码。";  
  
    case CURLE_FTP_CANT_GET_HOST:  
        return "在查找用于新连接的主机时出现内部错误。";  
  
    case CURLE_FTP_COULDNT_SET_TYPE:  
        return "在尝试将传输模式设置为二进制或 ascii 时发生错误。";  
  
    case CURLE_PARTIAL_FILE:  
        return "文件传输尺寸小于或大于预期。 当服务器先报告了一个预期的传输尺寸，然后所传送的数据与先前指定尺寸不相符时，就会发生此错误。";  
  
    case CURLE_FTP_COULDNT_RETR_FILE:  
        return "‘RETR’ 命令收到了不正常的回复，或完成的传输尺寸为零字节。";  
  
    case CURLE_QUOTE_ERROR:  
        return "在向远程服务器发送自定义 “QUOTE” 命令时，其中一个命令返回的错误代码为 400 或更大的数字（对于 FTP），或以其他方式表明命令无法成功完成。";  
  
    case CURLE_HTTP_RETURNED_ERROR:  
        return "如果 CURLOPT_FAILONERROR 设置为 TRUE，且 HTTP 服务器返回 >= 400 的错误代码，就会返回此代码。 （此错误代码以前又称为 CURLE_HTTP_NOT_FOUND。）";  
  
    case CURLE_WRITE_ERROR:  
        return "在向本地文件写入所收到的数据时发生错误，或由写入回调 (write callback) 向 libcurl 返回了一个错误。";  
  
    case CURLE_UPLOAD_FAILED:  
        return "无法开始上传。 对于 FTP，服务器通常会拒绝执行 STOR 命令。 错误缓冲区通常会提供服务器对此问题的说明。 （此错误代码以前又称为 CURLE_FTP_COULDNT_STOR_FILE。）";  
  
    case CURLE_READ_ERROR:  
        return "读取本地文件时遇到问题，或由读取回调 (read callback) 返回了一个错误。";  
  
    case CURLE_OUT_OF_MEMORY:  
        return "内存分配请求失败。 此错误比较严重，若发生此错误，则表明出现了非常严重的问题。";  
  
    case CURLE_OPERATION_TIMEDOUT:  
        return "操作超时。 已达到根据相应情况指定的超时时间。 请注意： 自 Urchin 6.6.0.2 开始，超时时间可以自行更改。 要指定远程日志下载超时，请打开 urchin.conf 文件，取消以下行的注释标记：#DownloadTimeout: 30";  
  
    case CURLE_FTP_PORT_FAILED:  
        return "FTP PORT 命令返回错误。 在没有为 libcurl 指定适当的地址使用时，最有可能发生此问题。 请参阅 CURLOPT_FTPPORT。";  
  
    case CURLE_FTP_COULDNT_USE_REST:  
        return "FTP REST 命令返回错误。 如果服务器正常，则应当不会发生这种情况。";  
  
    case CURLE_RANGE_ERROR:  
        return "服务器不支持或不接受范围请求。";  
  
    case CURLE_HTTP_POST_ERROR:  
        return "此问题比较少见，主要由内部混乱引发。";  
  
    case CURLE_SSL_CONNECT_ERROR:  
        return "同时使用 SSL/TLS 时可能会发生此错误。 您可以访问错误缓冲区查看相应信息，其中会对此问题进行更详细的介绍。 可能是证书（文件格式、路径、许可）、密码及其他因素导致了此问题。";  
  
    case CURLE_FTP_BAD_DOWNLOAD_RESUME:  
        return "尝试恢复超过文件大小限制的 FTP 连接。";  
  
    case CURLE_FILE_COULDNT_READ_FILE:  
        return "无法打开 FILE:// 路径下的文件。 原因很可能是文件路径无法识别现有文件。 建议您检查文件的访问权限。";  
  
    case CURLE_LDAP_CANNOT_BIND:  
        return "LDAP 无法绑定。LDAP 绑定操作失败。";  
  
    case CURLE_LDAP_SEARCH_FAILED:  
        return "LDAP 搜索无法进行。";  
  
    case CURLE_FUNCTION_NOT_FOUND:  
        return "找不到函数。 找不到必要的 zlib 函数。";  
  
    case CURLE_ABORTED_BY_CALLBACK:  
        return "由回调中止。 回调向 libcurl 返回了 “abort”。";  
  
    case CURLE_BAD_FUNCTION_ARGUMENT:  
        return "内部错误。 使用了不正确的参数调用函数。";  
  
    case CURLE_INTERFACE_FAILED:  
        return "界面错误。 指定的外部界面无法使用。 请通过 CURLOPT_INTERFACE 设置要使用哪个界面来处理外部连接的来源 IP 地址。 （此错误代码以前又称为 CURLE_HTTP_PORT_FAILED。）";  
  
    case CURLE_TOO_MANY_REDIRECTS:  
        return "重定向过多。 进行重定向时，libcurl 达到了网页点击上限。 请使用 CURLOPT_MAXREDIRS 设置上限。";  
  
    case CURLE_UNKNOWN_TELNET_OPTION:  
        return "无法识别以 CURLOPT_TELNETOPTIONS 设置的选项。 请参阅相关文档。";  
  
    case CURLE_TELNET_OPTION_SYNTAX:  
        return "telnet 选项字符串的格式不正确。";  
  
    case CURLE_PEER_FAILED_VERIFICATION:  
        return "远程服务器的 SSL 证书或 SSH md5 指纹不正确。";  
  
    case CURLE_GOT_NOTHING:  
        return "服务器未返回任何数据，在相应情况下，未返回任何数据就属于出现错误。";  
  
    case CURLE_SSL_ENGINE_NOTFOUND:  
        return "找不到指定的加密引擎。";  
  
    case CURLE_SSL_ENGINE_SETFAILED:  
        return "无法将选定的 SSL 加密引擎设为默认选项。";  
  
    case CURLE_SEND_ERROR:  
        return "无法发送网络数据。";  
  
    case CURLE_RECV_ERROR:  
        return "接收网络数据失败。";  
  
    case CURLE_SSL_CERTPROBLEM:  
        return "本地客户端证书有问题";  
  
    case CURLE_SSL_CIPHER:  
        return "无法使用指定的密钥";  
  
    case CURLE_SSL_CACERT:  
        return "无法使用已知的 CA 证书验证对等证书";  
  
    case CURLE_BAD_CONTENT_ENCODING:  
        return "无法识别传输编码";  
  
    case CURLE_LDAP_INVALID_URL:  
        return "LDAP 网址无效";  
  
    case CURLE_FILESIZE_EXCEEDED:  
        return "超过了文件大小上限";  
  
    case CURLE_USE_SSL_FAILED:  
        return "请求的 FTP SSL 级别失败";  
  
    case CURLE_SEND_FAIL_REWIND:  
        return "进行发送操作时，curl 必须回转数据以便重新传输，但回转操作未能成功";  
  
    case CURLE_SSL_ENGINE_INITFAILED:  
        return "SSL 引擎初始化失败";  
  
    case CURLE_LOGIN_DENIED:  
        return "远程服务器拒绝 curl 登录（7.13.1 新增功能）";  
  
    case CURLE_TFTP_NOTFOUND:  
        return "在 TFTP 服务器上找不到文件";  
  
    case CURLE_TFTP_PERM:  
        return "在 TFTP 服务器上遇到权限问题";  
  
    case CURLE_REMOTE_DISK_FULL:  
        return "服务器磁盘空间不足";  
  
    case CURLE_TFTP_ILLEGAL:  
        return "TFTP 操作非法";  
  
    case CURLE_TFTP_UNKNOWNID:  
        return "TFTP 传输 ID 未知";  
  
    case CURLE_REMOTE_FILE_EXISTS:  
        return "文件已存在，无法覆盖";  
  
    case CURLE_TFTP_NOSUCHUSER:  
        return "运行正常的 TFTP 服务器不会返回此错误";  
  
    case CURLE_CONV_FAILED:  
        return "字符转换失败";  
  
    case CURLE_CONV_REQD:  
        return "调用方必须注册转换回调";  
  
    case CURLE_SSL_CACERT_BADFILE:  
        return "读取 SSL CA 证书时遇到问题（可能是路径错误或访问权限问题）";  
  
    case CURLE_REMOTE_FILE_NOT_FOUND:  
        return "网址中引用的资源不存在";  
  
    case CURLE_SSH:  
        return "SSH 会话中发生无法识别的错误";  
  
    case CURLE_SSL_SHUTDOWN_FAILED:  
        return "无法终止 SSL 连接";  
  
    default:  
        return "未知的 curl 下载错误";  
    }  
}