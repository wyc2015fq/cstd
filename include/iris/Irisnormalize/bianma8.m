%%两核滤波器+两核滤波器fusion
%%
function A_f=bianma8(A)

[a b]=size(A);

fn=5;
alf=1.7;
dist1=5;
dist2=6;%%%%需要调整的参数

H1=fspecial('gaussian',fn,alf);
A1 = imfilter(A,H1);

A2=A1;
for i=1:(b-dist1)
    A2(:,i)=A1(:,i)-A1(:,i+dist1);
end

H2=fspecial('gaussian',fn,alf);
B1=imfilter(A,H2);



B3=B1;
for i=1:(b-dist2)
    B3(:,i)=B1(:,i)-B1(:,i+dist2);
end


%%fusion of score-level
B5=B3+A2;

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
