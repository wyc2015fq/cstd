%%
%%三核滤波器+三核滤波器
function A_f=bianma9(A)

[a b]=size(A);

fn=5;
alf=1.7;
dist1=5;
dist2=6;%%%%需要调整的参数

H1=fspecial('gaussian',fn,alf);


B1=imfilter(A,H1);



B2=B1;
for i=1:(b-dist1)
    B2(:,i)=B1(:,i)-2*B1(:,i+dist1);
end
B3=B2;
for i=1:(b-2*dist1)
    B3(:,i)=B2(:,i)+B1(:,i+2*dist1);
end

B4=B1;
for i=1:(b-dist2)
    B4(:,i)=B1(:,i)-2*B1(:,i+dist2);
end
B5=B4;
for i=1:(b-2*dist2)
    B5(:,i)=B4(:,i)+B1(:,i+2*dist2);
end

B6=B3+B5;

for i=1:a
    for j=1:b
        if B6(i,j)>0
            A_f(i,j)=1;
        else
            A_f(i,j)=0;
        end
    end
end
