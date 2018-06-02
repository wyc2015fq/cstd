function Localmax = Findlocalmax(direction,ix,iy,mag);

[m,n,o] = size(mag);

switch direction
    case 1
        idx = find((iy <= 0 & ix > -iy) | ( iy >= 0 & ix < -iy));
    case 2
        idx = find((ix > 0 & -iy >= ix) | ( ix < 0 & -iy <= ix));
    case 3
        idx = find((ix <= 0 & ix > iy) | ( ix >= 0 & ix < iy));
    case 4
        idx = find((iy < 0 & ix <= iy) | ( iy > 0 & ix >= iy));
end

if ~isempty(idx)
    v = mod(idx,m);
    extIdx = find(v==1|v==0|idx<=m|(idx>(n-1)*m));
    idx(extIdx)=[];
end

ixv = ix(idx);
iyv = iy(idx);
gradmag = mag(idx);
switch direction
    case 1
        d = abs(iyv./ixv);
        gradmag1 = mag(idx + m).*(1-d) + mag(idx + m - 1).*d;
        gradmag2 = mag(idx - m).*(1-d) + mag(idx - m + 1).*d;
    case 2
        d = abs(ixv./iyv);
        gradmag1 = mag(idx - 1).*(1-d) + mag(idx + m - 1).*d;
        gradmag2 = mag(idx + 1).*(1-d) + mag(idx - m + 1).*d;
    case 3
        d = abs(ixv./iyv);
        gradmag1 = mag(idx - 1).*(1-d) + mag(idx - m - 1).*d;
        gradmag2 = mag(idx + 1).*(1-d) + mag(idx + m + 1).*d;
    case 4
        d = abs(iyv./ixv);
        gradmag1 = mag(idx - m).*(1-d) + mag(idx - m - 1).*d;
        gradmag2 = mag(idx + m).*(1-d) + mag(idx + m + 1).*d;
end

Localmax = idx(gradmag >= gradmag1 & gradmag >= gradmag2 );
