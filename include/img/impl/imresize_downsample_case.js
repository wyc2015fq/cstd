
var fso = new ActiveXObject('Scripting.FileSystemObject');
var fname1 = 'imresize_downsample_case1.txt';
var fname2 = 'imresize_downsample_case2.txt';
var fname3 = 'imresize_downsample_case_sc.inl';

var a = fso.CreateTextFile(fname1, true);
var s='switch (ccw) {\r\n';
var nn=6;
for (var i = 1; i < nn; ++i)
{
  s += 'case '+i+': {\r\n';
	s+= 'enum {__cch='+ i + '};\r\n';
	s+='#define ADDX(_i)  ('
	var n = Math.pow(2, i);
	for (var j=0; j<n; ++j) {
		if (j>0) {s+='+';}
		s+= 'a0[_i+('+j+')*__cn]'
	}
	s+=')\r\n';
    s+= '#include "'+fname2+'"\r\n';
	s+='#undef ADDX\r\n';
  s+='} break;';
  s += '\r\n';
}
  s += '}\r\n';

a.WriteLine(s);

var a = fso.CreateTextFile(fname2, true);

var s='switch (cch) {\r\n';
for (var i = 1; i < nn; ++i)
{
  s += 'case '+i+': {\r\n';
	s+= 'enum {__ccw='+ i + '};\r\n';
	s+='#define ADDY(_i)  ('
	var n = Math.pow(2, i);
	for (var j=0; j<n; ++j) {
		if (j>0) {s+='+';}
		s+= 'ADDX(_i+'+j+'*al)'
	}
	s+=')\r\n';
    s+= '#include "'+fname3+'"\r\n';
	s+='#undef ADDY\r\n';
  s+='} break;';
  s += '\r\n';
}
  s += '}\r\n';

a.WriteLine(s);
