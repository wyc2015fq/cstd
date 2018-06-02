
var fso = new ActiveXObject('Scripting.FileSystemObject');


var PI = 3.1415926535897932384626433832795028841971;
var s = '';
var ss = 16;
var shi = Math.pow(2, ss);

var a = fso.CreateTextFile('table_sin360.txt', true);

for (var i = 0; i < 360; ++i)
{
  var v = Math.sin(i * PI / 180);
  v2 = Math.round(v * shi);
  s += (v2 + ',\r\n');
}

a.WriteLine(s);

s = '';
var a = fso.CreateTextFile('table_cos360.txt', true);

for (var i = 0; i < 360; ++i)
{
  var v = Math.cos(i * PI / 180);
  v2 = Math.round(v * shi);
  s += (v2 + ',\r\n');
}

a.WriteLine(s);

s = '';
var a = fso.CreateTextFile('table_atan256.txt', true);

for (var i = 0; i < 256; ++i)
{
  var v = Math.atan2(i, 256);
  v2 = Math.round(v * 180 / PI);
  s += (v2 + ',\r\n');
}

a.WriteLine(s);

if (0)
{
  s = '';
  var a = fso.CreateTextFile('table_sqrttable.txt', true);

  for (var i = 0; i < 256; ++i) {
    for (var j = 0; j < 256; ++j) {
      var v = Math.sqrt(i * i + j * j);
      v2 = Math.round(v * shi);
      s += (v2 + ',\r\n');
    }
  }

  a.WriteLine(s);
}

s = '';
var a = fso.CreateTextFile('mag256.txt', true);

for (var i = 0; i < 256; ++i)
{
  var v = Math.sqrt(i * i + 256 * 256) / (256 + i);
  v2 = Math.round(v * 256);
  s += (v2 + ',\r\n');
}

a.WriteLine(s);

