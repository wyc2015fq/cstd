
pow = Math.pow;
floor = Math.floor;
tanh = Math.tanh;

var shift = 13;
var shift1 = pow(2, shift);
var half_shift1 = shift1*0.5;

var fso = new ActiveXObject("Scripting.FileSystemObject");
var a = fso.CreateTextFile("pow01_13.txt", true);

var pow_shift1_01 = pow(shift1, 0.1);
//a.WriteLine(pow_shift1_01 + ',');
for (var i=0; i<4096; ++i) {
  a.WriteLine(floor((pow(i, 0.1)*shift1+160)/pow_shift1_01) + ',');
}

var b = fso.CreateTextFile("pow02_13.txt", true);

for (var i=0; i<256; ++i) {
  b.WriteLine(floor(pow(i, 0.2)*shift1+half_shift1) + ',');
}

var c = fso.CreateTextFile("tanh_130.txt", true);

for (var i=0; i<256; ++i) {
  //c.WriteLine(floor(tanh(i*0.1)*10.*shift1+half_shift1) + ',');
}
