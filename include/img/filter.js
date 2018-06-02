
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('filter.def', true);

a.WriteLine('//此文件由filter.js自动生成，请勿修改');
a.WriteLine('#ifndef _FILTER_DEF_');
a.WriteLine('#define _FILTER_DEF_');

function filter(n)
{
  var s = '#define FILTER' + n + '(H,W,A,AL,AI,B,BL,BI';

  for (var i = 0; i < n; ++i) {
    for (var j = 0; j < n; ++j) {
      s += ',M' + i + j;
    }
  }

  s += ')  {\\\n';
  s += '  int _I = 0, _J = 0;\\\n';
  var n2 = Math.floor(n / 2);

  // 四个角
  for (t = 0; t < 4; ++t) {
    for (var k = 0; k < n2; ++k) {
      for (var m = 0; m < n2; ++m) {
        var l = 0;

        if (0 == t) {
          s += '  (B)[( ' + k + ' )*(BL)+( ' + m + ' )*(BI)]=';
        }
        else if (1 == t) {
          s += '  (B)[( ' + (k) + ' )*(BL)+(W-' + (m + 1) + ')*(BI)]=';
        }
        else if (2 == t) {
          s += '  (B)[(H-' + (k + 1) + ')*(BL)+( ' + (m) + ' )*(BI)]=';
        }
        else if (3 == t) {
          s += '  (B)[(H-' + (k + 1) + ')*(BL)+(W-' + (m + 1) + ')*(BI)]=';
        }

        for (var i = 0; i < n; ++i) {
          for (var j = 0; j < n; ++j) {
            var y = k + i - n2;
            var x = m + j - n2;

            y = Math.max(0, y);
            x = Math.max(0, x);
            s += (l == 0) ? '' : '+';

            if (0 == t) {
              s += 'M' + i + j + '*(A)[( ' + y + ' )*(AL)+( ' + x + ' )*(AI)]';
            }
            else if (1 == t) {
              s += 'M' + (i) + (n - 1 - j) + '*(A)[( ' + (y) + ' )*(AL)+(W-' + (x + 1) + ')*(AI)]';
            }
            else if (2 == t) {
              s += 'M' + (n - 1 - i) + (j) + '*(A)[(H-' + (y + 1) + ')*(AL)+( ' + (x) + ' )*(AI)]';
            }
            else if (3 == t) {
              s += 'M' + (n - 1 - i) + (n - 1 - j) + '*(A)[(H-' + (y + 1) + ')*(AL)+(W-' + (x + 1) + ')*(AI)]';
            }

            ++l;

          }
        }

        s += ';\\\n';
      }
    }
  }

  for (var k = 0; k < n2; ++k) {
    s += '  for (_J = ' + n2 + '; _J < W-' + n2 + '; ++_J) {\\\n';

    for (t = 0; t < 2; ++t) {
      if (0 == t) {
        s += '    (B)[( ' + k + ' )*(BL)+(_J)*(BI)]=';
      }
      else if (1 == t) {
        s += '    (B)[(H-' + (k + 1) + ')*(BL)+(_J)*(BI)]=';
      }

      var l = 0;

      for (var i = 0; i < n; ++i) {
        for (var j = 0; j < n; ++j) {
          var y = i - n2 + k;
          var x = j - n2;
          y = Math.max(0, y);
          s += (l == 0) ? '' : '+';

          if (0 == t) {
            s += 'M' + i + j + '*(A)[( ' + y + ' )*(AL)+(_J+(' + x + '))*(AI)]';
          }
          else if (1 == t) {
            s += 'M' + (n - 1 - i) + j + '*(A)[(H-' + (y + 1) + ')*(AL)+(_J+(' + x + '))*(AI)]';
          }

          ++l;
        }
      }

      s += ';\\\n';
    }

    s += '  }\\\n';
  }

  s += '  for (_I = ' + n2 + '; _I < H-' + n2 + '; ++_I) {\\\n';

  for (var m = 0; m < n2; ++m) {
    for (t = 0; t < 2; ++t) {
      if (0 == t) {
        s += '    (B)[(_I)*(BL)+( ' + m + ' )*(BI)]=';
      }
      else if (1 == t) {
        s += '    (B)[(_I)*(BL)+(W-' + (m + 1) + ')*(BI)]=';
      }

      var l = 0;

      for (var i = 0; i < n; ++i) {
        for (var j = 0; j < n; ++j) {
          var y = i - n2;
          var x = j - n2 + m;
          x = Math.max(0, x);
          s += (l == 0) ? '' : '+';

          if (0 == t) {
            s += 'M' + i + j + '*(A)[(_I+(' + y + '))*(AL)+( ' + x + ' )*(AI)]';
          }
          else if (1 == t) {
            s += 'M' + i + (n - 1 - j) + '*(A)[(_I+(' + y + '))*(AL)+(W-' + (x + 1) + ')*(AI)]';
          }

          ++l;
        }

        ++l;
      }

      s += ';\\\n';
    }

  }

  s += '    for (_J = ' + n2 + '; _J < W-' + n2 + '; ++_J) {\\\n';
  s += '      (B)[(_I)*(BL)+(_J)*(BI)]=';
  var l = 0;

  for (var i = 0; i < n; ++i) {
    for (var j = 0; j < n; ++j) {
      var y = i - n2;
      var x = j - n2;
      s += (l == 0) ? '' : '+';

      s += 'M' + i + j + '*(A)[(_I+(' + y + '))*(AL)+(_J+(' + x + '))*(AI)]';

      ++l;
    }
  }

  s += ';\\\n';


  s += '    }\\\n';
  s += '  }\\\n';
  s += '}';

  return s;
}

for (var i = 3; i < 9; i += 2)
{
  a.WriteLine('//' + i + '*' + i + '滤波');
  var s = filter(i);
  a.WriteLine(s);
}

a.WriteLine('#endif // _FILTER_DEF_');

