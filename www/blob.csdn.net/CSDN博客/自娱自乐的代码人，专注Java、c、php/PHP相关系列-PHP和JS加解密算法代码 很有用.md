# PHP相关系列 - PHP和JS加解密算法代码 很有用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年08月01日 18:53:57[initphp](https://me.csdn.net/initphp)阅读数：1470








```php
<?php
/*
$mem = new Memcache();
$mem->addServer('10.249.198.235', 11211);
$ckey = 'num:lock';
$key = 'num';
if (false === $mem->add($ckey,1,0,1)) exit;
$num = $mem->get($key);
if ($num === false) {
	$mem->set($key, 1, 0, 20);
	$num = 1;
} else {
	$mem->increment($key);
	$num++;
}
if ($num > 20) exit;
$mem->delete($ckey);
error_log($num . "\n", 3, '/tmp/num.log');
*/
//$num = ceil(11/10);
//echo md5('pwcloudsync54256152');
//$str = '<div><b>fdsfdsfa</b></div>';
//echo filter_var($str, FILTER_SANITIZE_STRING);
//echo strtotime('2012-05-04');
//echo date('Y-m-d H:i:s', 1335325820);
//var_dump($_SERVER['LARAVEL_ENV']);
//var_dump(is_numeric('334545345546464654232'));

//echo php_sapi_name();
//echo phpinfo();
//echo strtotime('2012-06-26');
/*
include_once 'PwHttp.php';
$url = 'http://hongbao.local/market/user/isgain/?sign=dZ82YNzlSF%3A422A63acTedp8Zvmai9fisoa1%3A122436&random=0.097605431685224&cloud_app_id=1&sim=71a710076fa99af56631b0c5b5f3f15a&uuid=9D1EEBEF02A9DC918EFE1319A0A8DD26&device=HW-W880';
$i = 1;
$str = '';
while ($i <= 100) {
	$str .= $i . "\n";
	$result = PwHttp::get($url);
	if ($result->state != 200) {
		$str .= "======\n";
	} else {
		$str .= var_export(json_decode($result->data, true), true) . "\n";
	}
	$i++;
}
error_log($str, 3, '/tmp/aaa.log');
*/
function strencode($string){
	$string = base64_encode($string);
	$key    = '1234567890';
	$len    = strlen($key);
	$code    = '';
	for($i=0; $i < strlen($string); $i++){
		$k        = $i % $len;
		$code  .= $string[$i] ^ $key[$k];
	}
	return base64_encode($code);
}
echo strencode('abcdefg');
?>
<script>
var Base64 = {

// private property
_keyStr : "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=",

// public method for encoding
encode : function (input) {
    var output = "";
    var chr1, chr2, chr3, enc1, enc2, enc3, enc4;
    var i = 0;

    input = Base64._utf8_encode(input);

    while (i < input.length) {

        chr1 = input.charCodeAt(i++);
        chr2 = input.charCodeAt(i++);
        chr3 = input.charCodeAt(i++);

        enc1 = chr1 >> 2;
        enc2 = ((chr1 & 3) << 4) | (chr2 >> 4);
        enc3 = ((chr2 & 15) << 2) | (chr3 >> 6);
        enc4 = chr3 & 63;

        if (isNaN(chr2)) {
            enc3 = enc4 = 64;
        } else if (isNaN(chr3)) {
            enc4 = 64;
        }

        output = output +
        this._keyStr.charAt(enc1) + this._keyStr.charAt(enc2) +
        this._keyStr.charAt(enc3) + this._keyStr.charAt(enc4);

    }

    return output;
},

// public method for decoding
decode : function (input) {
    var output = "";
    var chr1, chr2, chr3;
    var enc1, enc2, enc3, enc4;
    var i = 0;

    input = input.replace(/[^A-Za-z0-9\+\/\=]/g, "");

    while (i < input.length) {

        enc1 = this._keyStr.indexOf(input.charAt(i++));
        enc2 = this._keyStr.indexOf(input.charAt(i++));
        enc3 = this._keyStr.indexOf(input.charAt(i++));
        enc4 = this._keyStr.indexOf(input.charAt(i++));

        chr1 = (enc1 << 2) | (enc2 >> 4);
        chr2 = ((enc2 & 15) << 4) | (enc3 >> 2);
        chr3 = ((enc3 & 3) << 6) | enc4;

        output = output + String.fromCharCode(chr1);

        if (enc3 != 64) {
            output = output + String.fromCharCode(chr2);
        }
        if (enc4 != 64) {
            output = output + String.fromCharCode(chr3);
        }

    }

    output = Base64._utf8_decode(output);

    return output;

},

// private method for UTF-8 encoding
_utf8_encode : function (string) {
    string = string.replace(/\r\n/g,"\n");
    var utftext = "";

    for (var n = 0; n < string.length; n++) {

        var c = string.charCodeAt(n);

        if (c < 128) {
            utftext += String.fromCharCode(c);
        }
        else if((c > 127) && (c < 2048)) {
            utftext += String.fromCharCode((c >> 6) | 192);
            utftext += String.fromCharCode((c & 63) | 128);
        }
        else {
            utftext += String.fromCharCode((c >> 12) | 224);
            utftext += String.fromCharCode(((c >> 6) & 63) | 128);
            utftext += String.fromCharCode((c & 63) | 128);
        }

    }

    return utftext;
},

// private method for UTF-8 decoding
_utf8_decode : function (utftext) {
    var string = "";
    var i = 0;
    var c = c1 = c2 = 0;

    while ( i < utftext.length ) {

        c = utftext.charCodeAt(i);

        if (c < 128) {
            string += String.fromCharCode(c);
            i++;
        }
        else if((c > 191) && (c < 224)) {
            c2 = utftext.charCodeAt(i+1);
            string += String.fromCharCode(((c & 31) << 6) | (c2 & 63));
            i += 2;
        }
        else {
            c2 = utftext.charCodeAt(i+1);
            c3 = utftext.charCodeAt(i+2);
            string += String.fromCharCode(((c & 15) << 12) | ((c2 & 63) << 6) | (c3 & 63));
            i += 3;
        }

    }

    return string;
}

}

function strencode(string){
	key='1234567890';
	string=Base64.decode(string);
	len=key.length;
	code='';
	for(i=0;i<string.length;i++){
		k=i % len;
		code +=String.fromCharCode(string.charCodeAt(i) ^ key.charCodeAt(k));
	}
	return Base64.decode(code);
}
var aa = strencode('aGV5Xm9xYVVjRwwP');
console.log(aa);
</script>
```




