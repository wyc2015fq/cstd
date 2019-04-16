# PHP相关系列 - 获取文件大小的方法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月02日 14:48:39[initphp](https://me.csdn.net/initphp)阅读数：721







- 直接获取的方法：将文件使用file_get_contents取回后，strlen或者存为文件后使用filesize
- 使用get_header

```php
$a_array = get_headers($url, true);
$size = $a_array['Content-Length'];
Echo $size;
```



      3.使用fsockopen,然后正则匹配出文件大小



```php
function get_file_size($url) {
    $url = parse_url($url);
 
    if (empty($url['host'])) {
        return false;
    }
 
    $url['port'] = empty($url['post']) ? 80 : $url['post'];
    $url['path'] = empty($url['path']) ? '/' : $url['path'];
 
    $fp = fsockopen($url['host'], $url['port'], $error);
 
    if($fp) {
        fputs($fp, "GET " . $url['path'] . " HTTP/1.1\r\n");
        fputs($fp, "Host:" . $url['host']. "\r\n\r\n");
 
        while (!feof($fp)) {
            $str = fgets($fp);
            if (trim($str) == '') {
                break;
            }elseif(preg_match('/Content-Length:(.*)/si', $str, $arr)) {
                return trim($arr[1]);
            }
        }
        fclose ( $fp);
        return false;
    }else {
        return false;
    }
}
```






