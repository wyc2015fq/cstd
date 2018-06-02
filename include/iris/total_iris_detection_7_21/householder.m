function H=householder(x)
%householder变换
n=length(x);
I=eye(n);
e1=I(1:n,1);%e1是单位列向量
u=(x-norm(x)*e1)/norm(x-norm(x)*e1);%u是单位列向量
H=I-2*(u)*u';