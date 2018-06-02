clear all;
t=[0:0.2:10];
y = sin(t);
plot(t,y,'r-', t,y+0.1,'g:', t,y+0.2,'b*');
ezplot