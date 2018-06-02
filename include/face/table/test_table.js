
pow = Math.pow;
floor = Math.floor;
tanh = Math.tanh;

var shift = 13;
var shift1 = pow(2, shift);

WScript.Echo(floor(pow(213, 0.2)*shift1)/shift1);
