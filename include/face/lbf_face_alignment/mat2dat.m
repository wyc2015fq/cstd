clc;
load train_model_new.mat
f = fopen('train_model_new.dat', 'wb');
npt = size(meanshape, 2)/2; %68
max_stage = 5;%迭代最大阶段数
fwrite(f, npt, 'integer*4');
fwrite(f, max_stage, 'integer*4');
fwrite(f, NumOfTree, 'integer*4');
fwrite(f, max_depth, 'integer*4');
% 给浮点版本 lbf_model_load 和 lbf_face_alignment 用的 dat文件
if 1
    fwrite(f, meanshape, 'double');
    for i=[1: max_stage],
        for j=[1:npt],
            for k=[1:NumOfTree],
                tt = forestmodel{i}(j, k);
                fwrite(f, tt{1}.feat', 'double');
                fwrite(f, tt{1}.thresh, 'double');
            end
        end
    end
    for i=[1: max_stage],
        fwrite(f, Wt{i}, 'double');
    end
end
% 给定点版本 lbf_model_load 用的 dat文件
if 1
end
fclose(f);

