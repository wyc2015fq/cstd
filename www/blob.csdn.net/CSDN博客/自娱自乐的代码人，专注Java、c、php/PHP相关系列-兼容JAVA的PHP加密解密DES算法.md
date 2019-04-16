# PHP相关系列 - 兼容JAVA的PHP加密解密DES算法 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年07月25日 18:22:22[initphp](https://me.csdn.net/initphp)阅读数：3863








今天一直折腾和JAVA对接数据加密解密的问题。

刚开始使用TEA的加密算法，但是JAVA那边和PHP端加密出来的结果不一样

只能修改算法，将算法修改成了DES的加密算法

但是PHP需要装一个mcrypt扩展，这点有点烦人

看代码吧：

```php
/**
	 * 加密
	 * @param string $str 字符串
	 * @param string $key 密钥
	 */
	public static function encrypt($str, $key)  {
		$block = mcrypt_get_block_size('des', 'ecb');
		$pad = $block - (strlen($str) % $block);
		$str .= str_repeat(chr($pad), $pad);
		$str = mcrypt_encrypt(MCRYPT_DES, $key, $str, MCRYPT_MODE_ECB);
		return base64_encode($str);
	}

	/**
	 * 解密
	 * @param string $str 字符串
	 * @param string $key 密钥
	 */
	public static function decrypt($str, $key) {
		$str = base64_decode($str);
		$str = mcrypt_decrypt(MCRYPT_DES, $key, $str, MCRYPT_MODE_ECB);
		$block = mcrypt_get_block_size('des', 'ecb');
		$pad = ord($str[($len = strlen($str)) - 1]);
		return substr($str, 0, strlen($str) - $pad);
	}
```






