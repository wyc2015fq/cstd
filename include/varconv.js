
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('varconvdef.txt', true);

a.WriteLine('//此文件由 varconv.js自动生成，请勿修改');

var uc = new Array('c', 'w', 'i', 'q', 'f', 'd');
var ut = new Array('char', 'short', 'int', '__int64', 'float', 'double');
var uv = new Array('str2i', 'str2i', 'str2i', 'str2i', 'str2f', 'str2f');
var uu = new Array('myitoa', 'myitoa', 'myitoa', 'myi64toa', 'myftoa', 'myftoa');

for (var i=0; i<uc.length; ++i) {
  for (var j=0; j<uc.length; ++j) {
    var tt = (i+1)*10 + (j+1);
    var s = 'VARCONFDEF('+tt+',\''+uc[i]+'\','+ut[i]+',\''+uc[j]+'\','+ut[j]+')';
    a.WriteLine(s);
  }
}


var a = fso.CreateTextFile('vartypedef.txt', true);
for (var j=0; j<uc.length; ++j) {
	var s = 'VARTYPEDEF('+j+',\''+uc[j]+'\','+ut[j]+','+uv[j]+','+uu[j]+')';
	a.WriteLine(s);
}
