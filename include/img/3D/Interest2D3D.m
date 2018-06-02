function [in2Dabc new3Dabc w_3D] = Interest2D3D(all_point,featurePoint,simplefaceTri,u0,v0,kx,ky,X22,Rot,Tran)


ll=1;
for InNum=1:size(featurePoint,1);
    x=featurePoint(InNum,1);
    y=featurePoint(InNum,2);
    for jj=1:500
        x1=all_point(simplefaceTri(jj,1),1);
        y1=all_point(simplefaceTri(jj,1),2);
        x2=all_point(simplefaceTri(jj,2),1);
        y2=all_point(simplefaceTri(jj,2),2);
        x3=all_point(simplefaceTri(jj,3),1);
        y3=all_point(simplefaceTri(jj,3),2);
        [alfa,beta,gama] = warpparameters(x,y,x1,y1,x2,y2,x3,y3);
        if((alfa>0)&(alfa<1)&(beta>0)&(beta<1)&(gama>0)&(gama<1))
% 
%             img(zuobiao(InNum,1),zuobiao(InNum,2),1)=0;
%             img(zuobiao(InNum,1),zuobiao(InNum,2),1)=255;
%             img(zuobiao(InNum,1),zuobiao(InNum,2),1)=0;

            in3Dabc(ll,1)=alfa;
            in3Dabc(ll,2)=beta;
            in3Dabc(ll,3)=gama;
            in3Dabc(ll,4)=jj;
            in2Dabc(ll,1)=InNum;
            xc=(x-v0)/ky;
            yc=(y-u0)/kx;
            zc=1;
            origin=[yc xc zc];
            direction=[yc xc zc];
            v00=X22(:,simplefaceTri(jj,1));
            v11=X22(:,simplefaceTri(jj,2));
            v22=X22(:,simplefaceTri(jj,3));
            w_3D(ll,1:3)=v00';
            w_3D(ll,4:6)=v11';
            w_3D(ll,7:9)=v22';
            v00=(Rot*v00+Tran)';
            v11=(Rot*v11+Tran)';
            v22=(Rot*v22+Tran)';
            triangle = [v00; v11; v22];
            [flag, u, v, t] = rayTriangleIntersection(origin, direction, v00, v11, v22);
            intersection = origin + t*direction;
            new3Dabc(ll,:)=inv(Rot)*(intersection'-Tran);
            clear origin direction t
%             new3Dabc(ll,1)=X22(1,simplefaceTri(jj,1))*alfa+X22(1,simplefaceTri(jj,2))*beta+X22(1,simplefaceTri(jj,3))*gama;
%             new3Dabc(ll,2)=X22(2,simplefaceTri(jj,1))*alfa+X22(2,simplefaceTri(jj,2))*beta+X22(2,simplefaceTri(jj,3))*gama;
%             new3Dabc(ll,3)=X22(3,simplefaceTri(jj,1))*alfa+X22(3,simplefaceTri(jj,2))*beta+X22(3,simplefaceTri(jj,3))*gama;
            ll=ll+1;
          break;
        end
       
    end
end