var fso = new ActiveXObject('Scripting.FileSystemObject');
var a = fso.CreateTextFile('imtrans_copy.inl', true);
var aty = ["", "uchar", "ushort", "uchar", "uint"];

var s = "{\r\n";
var istrans = 0;
var ismirrx = 0;
var ismirry = 0;
var ff = 4;
var cn = 1;
var srccn = 0;
s += "int flag = (!!istrans)*4+(!!ismirrx)*2+(!!ismirry)*1;\r\n";
s += "switch(cn) {\r\n";
for (cn = 1; cn < aty.length; ++cn) {
  s += "case " + cn + ": {\r\n";
  s += "switch(flag) {\r\n";
  for (istrans = 0; istrans < 2; ++istrans) {
    for (ismirrx = 0; ismirrx < 2; ++ismirrx) {
      for (ismirry = 0; ismirry < 2; ++ismirry) {
        s += "case " + (istrans * 4 + ismirrx * 2 + ismirry * 1) + ": {\r\n";
        s += "  int i, j;\r\n";
        s += "  for (j=0; j<h; ++j) {\r\n";
        s += "    const uchar* src0 = src + j * srcstep;\r\n";
        s += "    uchar* dst0 = dst + ";
        s += ismirry ? "(h-1-j) * ": "j * ";
        dstcn = cn;
        srccn = cn;
        s += istrans ? "dstcn": "dststep";
        s += ";\r\n"
        if (ff > 1) {
          s += "    for (i=0; i<w-" + (ff - 1) + "; i+=" + ff + ") {\r\n";
        } else {
          s += "    for (i=0; i<w; ++i) {\r\n";
        }
        for (j = 0; j < ff; ++j) {
          for (i = 0; i < cn; ++i) {
            s += "      dst0[(";
            s += ismirrx ? "w-1-": "";
            s += (ff > 1) ? "(i+" + j + ")": "i";
            s += ")";
            if (0 == istrans) {
              s += cn > 1 ? " * " + dstcn: "";
            } else {
              s += " * dststep";
            }
            s += cn > 1 ? " + " + i: "";
            s += "] = src0[";
            s += (ff > 1) ? "(i+" + j + ")": "i";
            if (1 != srccn) {
              s += " * " + srccn;
            }
            s += cn > 1 ? "+" + i: "";
            s += "];\r\n";
          }
        }
        s += '    }\r\n';
        s += '  }\r\n';
        s += '} break;\r\n';
      }
    }
  }

  s += "}\r\n";
  s += "}break;\r\n";
}

s += "}\r\n";
s += '}\r\n';
a.WriteLine(s);
