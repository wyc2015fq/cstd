
function savestr(name, s)
{
  var fso = new ActiveXObject('Scripting.FileSystemObject');
  var a = fso.CreateTextFile('color_impl.inl', true);
  a.WriteLine(s);
  return 0;
}

function echostr(s)
{
  WScript.echo(s);
}

function to_u8(x)
{
  return Math.round(x * Math.pow(2, 8));
}

var s = "{\r\n";
var a = [ //R G B
    0.299, 0.587, 0.114, 0,
    -0.169, -0.331, 0.5, 128,
    0.5, -0.419, -0.081, 128];
isu8 = 1;
sn = ['R', 'G', 'B'];
dn = ['Y', 'U', 'V'];

s += '  ' + (isu8 ? 'uchar' : 'double') + ' ';

for (i = 0; i < sn.length; ++i)
{
  s += (i > 0) ? ',' : '';
  s += sn[i] + '=src[' + i + ']';
}

for (i = 0; i < dn.length; ++i)
{
  s += (i > -1) ? ',' : '';
  s += dn[i];
}

s += ';\r\n';

for (i = 0; i < 3; ++i)
{
  s += "  ";
  s += dn[i] + '=';

  if (isu8) {
    s += '(';

    for (j = 0; j < 3; ++j) {
      s += j > 0 ? '+' : '';
      s += sn[j] + '*(' + to_u8(a[i * 4 + j]) + ')';
    }

    if (a[i * 4 + 3] != 0) {
      s += '+' + to_u8(a[i * 4 + 3]);
    }

    s += ')>>8';
  }
  else {
    for (j = 0; j < 3; ++j) {
      s += j > 0 ? '+' : '';
      s += sn[j] + '*' + a[i * 4 + j];
    }

    if (a[i * 4 + 3] != 0) {
      s += '+' + a[i * 4 + 3];
    }
  }

  s += ';\r\n';
}

for (i = 0; i < 3; ++i) {
  s += '  dst['+i+']='+dn[i]+';\r\n';
}

s += '}\r\n';

echostr(s);
//savestr('color_impl.inl', s);
