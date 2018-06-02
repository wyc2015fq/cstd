
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('typedef.txt', true);

a.WriteLine('//此文件由 typedef.js自动生成，请勿修改');

var US = new Array('U', 'S', 'U', 'S', 'U', 'S', 'U', 'S', 'F', 'F' );
var us = new Array('u', 's', 'u', 's', 'u', 's', 'u', 's', 'f', 'f' );
var cc = new Array('1', '1', '2', '2', '4', '4', '8', '8', '4', '8' );
var tt = new Array('uchar', 'char', 'ushort', 'short', 'uint', 'int', 'uint64', 'int64', 'float', 'double' );


for (var i=0; i<us.length; ++i) {
  var s = 'TYPEDEF(CC_'+cc[i]+US[i]+','+us[i]+cc[i]+','+cc[i]+','+tt[i]+')';
  a.WriteLine(s);
}

var a = fso.CreateTextFile('typecvt.txt', true);
a.WriteLine('//此文件由 typedef.js自动生成，请勿修改');

for (var i=0; i<us.length; ++i) {
for (var j=0; j<us.length; ++j) {
  var s = 'TYPECVTDEF(CC_'+cc[i]+US[i]+','+us[i]+cc[i]+','+cc[i]+','+tt[i]+',CC_'+cc[j]+US[j]+','+us[j]+cc[j]+','+cc[j]+','+tt[j]+')';
  a.WriteLine(s);
}
}


