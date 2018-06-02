
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('filterrect.def', true);

var n = 27;

a.WriteLine('//此文件由filter.js自动生成，请勿修改');
a.WriteLine('#ifndef _FILTERRECT_DEF_');
a.WriteLine('#define _FILTERRECT_DEF_');

function filterrect(n)
{
  var s = '#define FILTERRECT' + n + '(H,W,A,AL,AI,B,BL,BI,C,S';

  for (var i = 0; i < n; ++i) {
    s += ',V' + i;
  }

  for (var i = 0; i < n; ++i) {
    s += ',H' + i;
  }

  s += ')  {\\\n';
  s += '  int _I = 0, _J = 0;\\\n';
  var n2 = Math.floor(n / 2);

  for (var p = 0; p < 2; ++p) {
    for (var k = 0; k < n2; ++k) {
      s += '  for (_J = 0; _J < W; ++_J) {\\\n';
      s += '    (C)[_J]=(';

      var l = 0;

      for (var i = 0; i < n; ++i) {
        var y = Math.max(0, k + i - n2);
        s += (l == 0) ? '' : '+';

        if (0 == p) {
          s += 'V' + i + '*(A)[(' + y + ')*(AL)+(_J)*(AI)]';
        }
        else {
          s += 'V' + (n - 1 - i) + '*(A)[(H-' + (y + 1) + ')*(AL)+(_J)*(AI)]';
        }

        ++l;
      }

      s += ')>>(S);\\\n';
      s += '  }\\\n';

      for (var t = 0; t < 2; ++t) {
        for (var m = 0; m < n2; ++m) {
          var l = 0;

          if (0 == p) {
            if (0 == t) {
              s += '  (B)[( ' + k + ' )*(BL)+( ' + m + ' )*(BI)]=(';
            }
            else if (1 == t) {
              s += '  (B)[( ' + k + ' )*(BL)+(W-' + (m + 1) + ')*(BI)]=(';
            }
          }
          else if (1 == p) {
            if (0 == t) {
              s += '  (B)[(H-' + (k + 1) + ')*(BL)+( ' + m + ' )*(BI)]=(';
            }
            else if (1 == t) {
              s += '  (B)[(H-' + (k + 1) + ')*(BL)+(W-' + (m + 1) + ')*(BI)]=(';
            }
          }

          for (var i = 0; i < n; ++i) {
            var x = Math.max(0, m + i - n2);
            s += (l == 0) ? '' : '+';

            if (0 == t) {
              s += 'H' + i + '*(C)[' + x + ']';
            }
            else if (1 == t) {
              s += 'H' + (n - 1 - i) + '*(C)[W-' + (x + 1) + ']';
            }

            ++l;
          }

          s += ')>>(S);\\\n';
        }

      }

      s += '  for (_J = ' + n2 + '; _J < W-' + n2 + '; ++_J) {\\\n';
      var l = 0;

      if (0 == p) {
        s += '    (B)[( ' + k + ' )*(BL)+(_J)*(BI)]=(';
      }
      else if (1 == p) {
        s += '    (B)[(H-' + (k + 1) + ')*(BL)+(_J)*(BI)]=(';
      }

      for (var i = 0; i < n; ++i) {
        var x = i - n2;
        s += (l == 0) ? '' : '+';
        s += 'H' + i + '*(C)[_J+(' + x + ')]';
        ++l;
      }

      s += ')>>(S);\\\n';
      s += '  }\\\n';

      //s += '  for (_J = 0; _J < W; ++_J) {\\\n';
    }
  }


  s += '  for (_I = ' + n2 + '; _I < H-' + n2 + '; ++_I) {\\\n';
  s += '    for (_J = 0; _J < W; ++_J) {\\\n';

  s += '      (C)[_J]=(';

  var l = 0;

  for (var i = 0; i < n; ++i) {
    var y = i - n2;
    s += (l == 0) ? '' : '+';
    s += 'V' + i + '*(A)[(_I+(' + y + '))*(AL)+(_J)*(AI)]';
    ++l;
  }

  s += ')>>(S);\\\n';

  s += '    }\\\n';

  for (var t = 0; t < 2; ++t) {
    for (var m = 0; m < n2; ++m) {
      var l = 0;

      if (0 == t) {
        s += '    (B)[(_I)*(BL)+( ' + m + ' )*(BI)]=(';
      }
      else if (1 == t) {
        s += '    (B)[(_I)*(BL)+(W-' + (m + 1) + ')*(BI)]=(';
      }

      for (var i = 0; i < n; ++i) {
        var x = Math.max(0, m + i - n2);
        s += (l == 0) ? '' : '+';

        if (0 == t) {
          s += 'H' + i + '*(C)[' + x + ']';
        }
        else if (1 == t) {
          s += 'H' + (n - 1 - i) + '*(C)[W-' + (x + 1) + ']';
        }

        ++l;
      }

      s += ')>>(S);\\\n';
    }

  }

  s += '    for (_J = ' + n2 + '; _J < W-' + n2 + '; ++_J) {\\\n';
  var l = 0;
  s += '      (B)[(_I)*(BL)+(_J)*(BI)]=(';

  for (var i = 0; i < n; ++i) {
    var x = i - n2;
    s += (l == 0) ? '' : '+';
    s += 'H' + i + '*(C)[_J+(' + x + ')]';
    ++l;
  }

  s += ')>>(S);\\\n';
  s += '    }\\\n';
  s += '  }\\\n';


  s += '}';

  return s;
}

function MakeGaussKernel(WindowSize)
{
  var PI = 3.1415926535897932384626433832795028841971;
  //var WindowSize = 1 + 2 * Math.ceil(3 * sigma);
  var sigma = (WindowSize - 1) / 6;
  var Center = Math.floor(WindowSize / 2);
  var Kernel = new Array();
  var dSum = 0;

  for (var i = 0; i < WindowSize; i++) {
    var dDis = i - Center;
    dValue = Math.exp(-0.5 * dDis * dDis / (sigma * sigma));
    Kernel[ i ] = dValue;
    dSum += dValue;
  }

  // 归一化
  for (var i = 0; i < WindowSize; i++) {
    Kernel[ i ] /= dSum;
  }

  return Kernel;
}

function MakeGauss(n)
{
  var Kernel = MakeGaussKernel(n);
  var s = '#define GAUSS' + n + '(H,W,A,AL,AI,B,BL,BI,C)';
  var ss = 8;
  var shift = Math.pow(2, ss);
  s += '   FILTERRECT' + n + '(H,W,A,AL,AI,B,BL,BI,C,' + ss;

  for (var i = 0; i < Kernel.length; i++) {
    s += ',' + Math.round(Kernel[i] * shift);
  }

  for (var i = 0; i < Kernel.length; i++) {
    s += ',' + Math.round(Kernel[i] * shift);
  }

  s += ')';
  return s;
}

for (var i = 3; i < n; i += 2)
{
  a.WriteLine('//' + i + '*' + i + '滤波');
  var s = filterrect(i);
  a.WriteLine(s);
}

for (var i = 3; i < n; i += 2)
{
  a.WriteLine('//' + i + '*' + i + '高斯滤波');
  var s = MakeGauss(i);
  a.WriteLine(s);
}

a.WriteLine('#endif // _FILTER_DEF_');

