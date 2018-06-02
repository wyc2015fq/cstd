var fso = new ActiveXObject('Scripting.FileSystemObject');
var a = fso.CreateTextFile('blurdetect.inl', true);
var aty = [
-1,-1,-1,-1,-1,
-1,2,2,2,-1,
-1,2,0,2,-1,
-1,2,2,2,-1,
-1,-1,-1,-1,-1,
];

var m=5;
var n=5;
var s = "#define BLURDETECT(src, stp)  (";
for (var i=0; i<m; ++i) {
 for (var j=0; j<n; ++j) {
  var k=i*n+j;
  var t = aty[k];
  if (t!=0) {
    s+=t>0 ? '+':'-';
	if (Math.abs(t)!=1) {
	  s+=''+Math.abs(t)+'*';
	}
	s+='src[stp['+k+']]';
  }
 }
}
s += ')\r\n';
a.WriteLine(s);
