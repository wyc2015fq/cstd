
var fso = new ActiveXObject('Scripting.FileSystemObject');

var a = fso.CreateTextFile('math_def.txt', true);

var ii = 1;
var n = 7;
var nn = 18;

a.WriteLine('//此文件由math.js自动生成，请勿修改');
a.WriteLine('#ifndef _MATHDEF_H_');
a.WriteLine('#define _MATHDEF_H_');

a.WriteLine('//向量访问');
for (var i = 0; i < nn; ++i)
{
  var s = '#define V' + i + '(C)  (C)[' + i + ']';
  a.WriteLine(s);
}

a.WriteLine('//向量赋值');

for (var i = ii; i < nn; ++i)
{
  var s = '#define V' + i + 'SET(C'
      var v = '('

  for (var j = 0; j < i; ++j) {
    s += ',A' + j;
    v += j ? ',' : '';
    v += '(C)[' + j + ']=A' + j;
  }

  s += ')  ' + v + ')';
  a.WriteLine(s);
}

for (var i = ii; i < nn; ++i)
{
  var s = '#define V' + i + 'SETALL(C, X)    ('

  for (var j = 0; j < i; ++j) {
    s += '(C)[' + j + ']=';
  }

  s += '=X)';
  a.WriteLine(s);
}
for (var i = ii; i < nn; ++i)
{
  var s = '#define V' + i + 'SETV(C, A)  ('

  for (var j = 0; j < i; ++j) {
    s += j ? ',' : '';
    s += '(C)[' + j + ']=(A)[' + j + ']';
  }

  s += ')';
  a.WriteLine(s);
}

var arrA = ['ADD', 'SUB', 'MUL', 'DIV'];
var arrB = ['+', '-', '*', '/'];

for (var k = 0; k < 4; ++k)
{
  a.WriteLine('//向量' + arrA[k]);

  for (var i = ii; i < nn; ++i) {
    var s = '#define V' + i + arrA[k] + 'V(C, A, B)  ('

    for (var j = 0; j < i; ++j) {
      s += j ? ',' : '';
      s += '(C)[' + j + ']=(A)[' + j + ']' + arrB[k] + '(B)[' + j + ']';
    }

    s += ')';
    a.WriteLine(s);
    s = '#define V' + i + arrA[k] + 'X(C, A, B)  ('

    for (var j = 0; j < i; ++j) {
      s += j ? ',' : '';
      s += '(C)[' + j + ']=(A)[' + j + ']' + arrB[k] + '(B)';
    }

    s += ')';
    a.WriteLine(s);
  }
}

a.WriteLine('//向量范数');

for (var i = 2; i < nn; ++i)
{
  var s = '#define V' + i + 'NORM(A)  (';

  for (var j = 0; j < i; ++j) {
    s += j ? '+' : '';
    s += '(A)[' + j + ']*(A)[' + j + ']';
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//向量点积');

for (var i = 2; i < nn; ++i)
{
  var s = '#define V' + i + 'DOT(A, B)  (';

  for (var j = 0; j < i; ++j) {
    s += j ? '+' : '';
    s += '(A)[' + j + ']*(B)[' + j + ']';
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//向量混合');

for (var k = 2; k < 5; ++k)
{
  for (var i = 2; i < nn; ++i) {
    var s = '#define V' + i + 'MIX' + k + '(C';

    for (var m = 0; m < k; ++m) {
      s += ',A' + m;
      s += ',B' + m;
    }

    s += ')  (';

    for (var j = 0; j < i; ++j) {
      s += j ? ',' : '';
      s += '(C)[' + j + ']=';

      for (var m = 0; m < k; ++m) {
        s += m > 0 ? '+' : '';
        s += '(A' + m + ')[' + j + ']*(B' + m + ')';
      }

      s += '\\\n';
    }

    s += ')';
    a.WriteLine(s);
  }
}

a.WriteLine('//向量距离平方');

for (var i = 2; i < nn; ++i)
{
  var s = '#define V' + i + 'DIST2(A, B)  (';

  for (var j = 0; j < i; ++j) {
    s += j ? '+' : '';
    s += '((A)[' + j + ']-(B)[' + j + '])';
    s += '*';
    s += '((A)[' + j + ']-(B)[' + j + '])';
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//矩阵访问');

for (var i = 2; i < n; ++i)
{
  a.WriteLine('//' + i + '维矩阵');

  for (var j = 0; j < i; ++j) {
    for (var k = 0; k < i; ++k) {
      var s = '#define M' + i + j + k + '(C)  (C)[' + i + '*(' + j + ')' + '+(' + k + ')]';
      a.WriteLine(s);
    }
  }
}

a.WriteLine('//单位矩阵');

for (var i = 2; i < n; ++i)
{
  var s = '#define M' + i + 'ID(C)   (';

  for (var j = 0; j < i; ++j) {
    for (var k = 0; k < i; ++k) {
      var x = j == k ? 1 : 0;
      s += (j + k) ? ',' : '';
      s += 'M' + i + j + k + '(C)=' + x;
    }
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//矩阵转置');
for (var i = 2; i < n; ++i)
{
  var s = '#define M' + i + 'TRANS(C,t)  (';
  var t=0;
  for (var j = 0; j < i; ++j) {
    for (var k = j+1; k < i; ++k) {
      s += (t>0) ? ',' : '';
      s += 'CC_SWAP(M' + i + j + k + '(C), M' + i + k + j + '(C), t)';
	  ++t;
    }
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//矩阵乘同维向量');

for (var i = 2; i < n; ++i)
{
  var s = '#define M' + i + 'MULV' + i + '(C, A, B)   (';

  for (var j = 0; j < i; ++j) {
    s += (j) ? ',' : '';
    s += '(C)[' + j + ']=';

    for (var k = 0; k < i; ++k) {
      s += k ? '+' : '';
      s += 'M' + i + j + k + '(A)*(B)[' + k + ']';
    }
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//矩阵乘同低一维向量');

for (var i = 2; i < n; ++i)
{
  var s = '#define M' + i + 'MULV' + (i - 1) + '(C, A, B)   (';

  for (var j = 0; j < i - 1; ++j) {
    s += (j) ? ',' : '';
    s += '(C)[' + j + ']=';

    for (var k = 0; k < i - 1; ++k) {
      s += k ? '+' : '';
      s += 'M' + i + j + k + '(A)*(B)[' + k + ']';
    }

    s += '+M' + i + j + k + '(A)';
  }

  s += ')';
  a.WriteLine(s);
}


a.WriteLine('//矩阵行列式');
a.WriteLine('#define DET1(A00)  (A00)');

for (var i = 2; i < n; ++i)
{
  var s = '#define DET' + i + '(';

  for (var j = 0; j < i; ++j) {
    for (var k = 0; k < i; ++k) {
      s += (j + k) ? ',' : '';
      s += 'A' + j + k;
    }
  }

  s += ')   (';
  var ss = 1;

  for (var j = 0; j < i; ++j) {
    s += (j % 2) ? '-' : '+';
    s += '(A0' + j + ')*DET' + (i - 1) + '(';

    for (var m = 1; m < i; ++m) {
      for (var k = 0; k < i; ++k) {
        if (k != j) {
          if (j > 0) {
            s += (k + m - 1) ? ',' : '';
          }
          else {
            s += (k + m - 1) > 1 ? ',' : '';
          }

          s += 'A' + m + k;
        }
      }
    }

    s += ')';
  }

  s += ')';
  a.WriteLine(s);
}

for (var i = 2; i < n; ++i)
{
  var s = '#define M' + i + 'DET(C)  DET' + i + '(';

  for (var j = 0; j < i; ++j) {
    for (var k = 0; k < i; ++k) {
      s += (j + k) ? ',' : '';
      s += 'M' + i + j + k + '(C)';
    }
  }

  s += ')';
  a.WriteLine(s);
}


a.WriteLine('//矩阵乘法');

for (var i = 2; i < n; ++i)
{
  var s = '#define M' + i + 'MUL(C, A, B)   (';

  for (var j = 0; j < i; ++j) {
    for (var k = 0; k < i; ++k) {
      s += (j + k) ? ',' : '';
      s += 'M' + i + j + k + '(C)=';

      for (var m = 0; m < i; ++m) {
        s += m ? '+' : '';
        s += 'M' + i + j + m + '(A)*M' + i + m + k + '(B)';
      }

      s += '\\\n';
    }
  }

  s += ')';
  a.WriteLine(s);
}

a.WriteLine('//解方程Ax=B');

for (var i = 2; i < n; ++i)
{
  var s = '#define SOLVE' + i + '(A, B, X)  {double d=M' + i + 'DET(A); if (d<-EPS || d>EPS) {';
  s += '\\\n';

  for (var j = 0; j < i; ++j) {
    s += '(X)[' + j + ']=DET' + i + '(';

    for (var m = 0; m < i; ++m) {
      for (var k = 0; k < i; ++k) {
        s += (0 == m && 0 == k) ? '' : ',';
        s += (k == j) ? ('(B)[' + m + ']') : ('M' + i + m + k + '(A)');
      }
    }

    s += ')/d;';
    s += '\\\n';
  }

  s += '}}'
      a.WriteLine(s);
}

a.WriteLine('#endif // _MATHDEF_H_');


