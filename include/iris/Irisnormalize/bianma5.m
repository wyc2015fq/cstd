%%
%%ÈıºËÂË²¨Æ÷
function A_f=bianma4(A)

[a b]=size(A);

fn=5;
alf=1.7;
dist=5;

H1=fspecial('gaussian',fn,alf);
H2=fspecial('gaussian',fn,alf);

B1=imfilter(A,H1);
B2=imfilter(A,H2);


B3=B1;
for i=1:(b-dist)
    B3(:,i)=B1(:,i)-2*B2(:,i+dist);
end
B4=B3;
for i=1:(b-2*dist)
    B4(:,i)=B3(:,i)+B1(:,i+2*dist);
end

for i=1:a
    for j=1:b
        if B4(i,j)>0
            A_f(i,j)=1;
        else
            A_f(i,j)=0;
        end
    end
end
