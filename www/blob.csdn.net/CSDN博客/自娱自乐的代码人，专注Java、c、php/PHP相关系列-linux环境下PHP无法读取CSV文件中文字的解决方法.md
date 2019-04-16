# PHP相关系列 - linux环境下PHP无法读取CSV文件中文字的解决方法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年09月07日 10:01:18[initphp](https://me.csdn.net/initphp)阅读数：1990








在win下可以轻松读取中文，但是放到linux的测试机器上，中文字就读取不出来。问题原因肯定和编码有关系：

```php
public function readCsv($file) {
		$row = 1; 
		if (!is_file($file)) {
			return array();
		}
		$handle = fopen($file,"r");
		$arr = array();
		while ($data = fgetcsv($handle, 1000, ",")) {  
			$num = count($data); 
			$row++;  
			for ($c=0; $c < $num; $c++) {
				$arr[$row]['book_id'] 			= $data[0];  
				$arr[$row]['book_title'] 		= iconv('gb2312', 'utf-8',$data[1]);  
				$arr[$row]['book_author'] 		= iconv('gb2312', 'utf-8',$data[2]);  
				$arr[$row]['book_img'] 			= $data[3];  
				$arr[$row]['book_taobao_price'] = $data[4];  
				$arr[$row]['book_des'] 			= iconv('gb2312', 'utf-8',$data[5]);  
			} 
		}
		$temp = array(); 
		foreach ($arr as $val) {
			if ($val['book_id'] != '' && is_numeric($val['book_id'])) {
				$temp[] = $val;
			}
		}
		fclose($handle);
		return $temp; 
	}
```


**解决方法：setlocale**

setlocale() 函数设置地区信息（地域信息）。

地区信息是针对一个地理区域的语言、货币、时间以及其他信息。

该函数返回当前的地区设置，若失败则返回 false。



```php
setlocale(LC_ALL, 'zh_CN');
```



