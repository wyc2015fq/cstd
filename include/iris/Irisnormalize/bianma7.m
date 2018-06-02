%%两核滤波器+三核滤波器fusion
%%
function A_f=bianma7(A)

[a b]=size(A);

fn=5;
alf=1.7;
dist=5;%%需要调整的参数

H1=fspecial('gaussian',fn,alf);
A_j1 = imfilter(A,H1);

A_j2=A_j1;
for i=1:(b-dist)
    A_j2(:,i)=A_j1(:,i)-A_j1(:,i+dist);
end

H2=fspecial('gaussian',fn,alf);
B1=imfilter(A,H2);



B3=B1;
for i=1:(b-dist)
    B3(:,i)=B1(:,i)-2*B1(:,i+dist);
end
B4=B3;
for i=1:(b-2*dist)
    B4(:,i)=B3(:,i)+B1(:,i+2*dist);
end

%%fusion of score-level
B5=B4+A_j2;

%%二值化
for i=1:a
    for j=1:b
        if B5(i,j)>0
            A_f(i,j)=1;
        else
            A_f(i,j)=0;
        end
    end
end
