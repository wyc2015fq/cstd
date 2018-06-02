function[pp]=oversample(A)
%A=imread('1.bmp');
step_x=8;
step_y=8;
num_total=0;
block=0;
for block_x=10:10:30;
for block_y=10:10:30;
for i=1:step_x:100
    for j=1:step_y:50
     if (i+block_x<=100)&(j+block_y<=50)
        num_total=num_total+1;
    block(num_total,1)=i;
    block(num_total,2)=j;
    block(num_total,3)=block_x;
    block(num_total,4)=block_y;
    
    else
    end
    end
end
end
end
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
indd=block;
pp=cell(372,1);
for i=1:372
   % for j=1:4
        pp{i}=A((indd(i,2)+1):(indd(i,2)+indd(i,4)),(indd(i,1)+1):(indd(i,1)+indd(i,3)));
    %end
end    