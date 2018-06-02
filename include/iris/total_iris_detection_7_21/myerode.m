% 2011_7_20  ¢œ» 
% ∏Ø ¥
function I = myerode(Pic, canshu)

if canshu == 'se5'
    se = [0     0     1     1     1     1     1     0     0
         0     1     1     1     1     1     1     1     0
         1     1     1     1     1     1     1     1     1
         1     1     1     1     1     1     1     1     1
         1     1     1     1     1     1     1     1     1
         1     1     1     1     1     1     1     1     1
         1     1     1     1     1     1     1     1     1
         0     1     1     1     1     1     1     1     0
         0     0     1     1     1     1     1     0     0];
elseif canshu == 'se3'
    se = [1     1     1     1     1
         1     1     1     1     1
         1     1     1     1     1
         1     1     1     1     1
         1     1     1     1     1];
end
 
[row_se, aa] = size(se);
rc = floor(row_se/2);  
[row_pic, col_pic] = size(Pic);
I = false(row_pic, col_pic);
for ii = rc + 1 : row_pic-rc-1
    for jj = rc + 1 : col_pic-rc-1
        sum_juanji = sum(sum ( Pic(ii-rc:ii+rc, jj-rc:jj+rc).* se));
        if sum_juanji == sum(sum(se))
            I(ii,jj) = 1;
        end
    end
end

            