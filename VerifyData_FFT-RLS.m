%%%%��ǿ�����ݷ��� 

clear;

clear;
load c:\temp\MagData.m

format long

nLen = size(MagData,1);
nStart =1;
nTestCount = 1000;
nEnd = nLen;
pFunc = [-0.000000038339766   0.000013799628125  -0.000355771798074   0.340519915563616  -0.032323730910474];
Mzy = [0 1 0; 0 0 1; 1 0 0];
%%%ȥ���������ݵ���ƫ
% x = MagData(:,2) - sum(MagData(:,2))/nLen;
% y = MagData(:,3) - sum(MagData(:,3))/nLen;
% z = MagData(:,4) - sum(MagData(:,4))/nLen;
% tt = MagData(:,5) - sum(MagData(:,5))/nLen;
%%%%%ȥ����Ҫ���㲿�ֵ�������ƫ
x = MagData(:,2) - sum(MagData(nStart:(nStart+nTestCount-1),2))/nTestCount;
y = MagData(:,3) - sum(MagData(nStart:(nStart+nTestCount-1),3))/nTestCount;
z = MagData(:,4) - sum(MagData(nStart:(nStart+nTestCount-1),4))/nTestCount;


figure;
plot(1:nLen, x,'r',1:nLen, y,'k',1:nLen, z,'b')
title('ȥ����ֵ���ԭʼ����');
legend('x','z','y');
xlabel('������');
ylabel('��ǿ���ź�Ht/nT')

Mx = x(nStart:(nStart+nTestCount-1));
My = y(nStart:(nStart+nTestCount-1));
Mz = z(nStart:(nStart+nTestCount-1));

Mx = Mx - sum(Mx)/nTestCount;
My = My - sum(My)/nTestCount;
Mz = Mz - sum(Mz)/nTestCount;

% x = MagData(:,2) - sum(MagData(nStart:(nStart+nTestCount),2))/(nTestCount+1);
% y = MagData(:,3) - sum(MagData(nStart:(nStart+nTestCount),3))/(nTestCount+1);
% z = MagData(:,4) - sum(MagData(nStart:(nStart+nTestCount),4))/(nTestCount+1);
% 


for i = 1 : 50
    fSin(i) = 0;
    fCos(i) = 0;
    %ֻ����1000�����ݣ�������1�������
    for j = nStart : nTestCount + nStart-1
        fCos(i) = fCos(i) + 2*x(j)*cos(i*j*2.0*pi/nTestCount)/nTestCount;
        fSin(i) = fSin(i) + 2*x(j)*sin(i*j*2.0*pi/nTestCount)/nTestCount;
    end
    fAx(i) = sqrt(fSin(i)*fSin(i) + fCos(i)*fCos(i));
    fPhasex(i) = atan2(fCos(i), fSin(i));
end

for i = 1 : 50
    fSin(i) = 0;
    fCos(i) = 0;
    %ֻ����1000�����ݣ�������1�������
    for j = nStart : nTestCount + nStart - 1
        fSin(i) = fSin(i) + 2*y(j)*sin(i*j*2.0*pi/nTestCount)/nTestCount;
        fCos(i) = fCos(i) + 2*y(j)*cos(i*j*2.0*pi/nTestCount)/nTestCount;
    end
    fAy(i) = sqrt(fSin(i)*fSin(i) + fCos(i)*fCos(i));
    fPhasey(i) = atan2(fCos(i),fSin(i));
end

for i = 1 : 50
    fSin(i) = 0;
    fCos(i) = 0;
    %ֻ����1000�����ݣ�������1�������
    for j = nStart : nTestCount + nStart - 1
        fCos(i) = fCos(i) + 2*z(j)*cos(i*j*2.0*pi/nTestCount)/nTestCount;
        fSin(i) = fSin(i) + 2*z(j)*sin(i*j*2.0*pi/nTestCount)/nTestCount;
    end
    fAz(i) = sqrt(fSin(i)*fSin(i) + fCos(i)*fCos(i));
    fPhasez(i) = atan2(fCos(i),fSin(i));
end
%%%%�ҵ�����ֵ��Ӧ��Ƶ��

[MaxAmplx MaxFreqx] = max(fAx);
[MaxAmply MaxFreqy] = max(fAy);
[MaxAmplz MaxFreqz] = max(fAz);

%%%��С���˷�ȷ��Ƶ��
%%%Ƶ�ʹ��㣨���Ĳ������Ҳ�������ϵĿ����㷨_��־������
xixj = 0;
for i=nStart+1 : nStart+nTestCount-3
    xixj=xixj+x(i)*x(i+1);
end
gx = (x(nStart)*x(nStart+1)+x(nStart+nTestCount-2)*x(nStart+nTestCount-1)+2*xixj)/sum(x(nStart+1:(nStart+nTestCount-2)).^2);
wx = acos(gx/2);
ex = 10000000000;
 for w = wx-0.001 : 0.000001 : wx
     Ex_test=0;
    for i = 1 : nLen
        Ex_test = Ex_test + (x(i) - MaxAmplx * sin(w * i + fPhasex(MaxFreqx)))^2;
    end
	if Ex_test < ex
        ex = Ex_test;
        wx0 = w;
	end
 end

yiyj = 0;
for i=nStart+1 : nStart+nTestCount-3
    yiyj=yiyj+y(i)*y(i+1);
end
gy = (y(nStart)*y(nStart+1)+y(nStart+nTestCount-2)*y(nStart+nTestCount-1)+2*yiyj)/sum(y(nStart+1:(nStart+nTestCount-2)).^2);
wy = acos(gy/2);
ey = 10000000000;
 for w = wy-0.001 : 0.000001 : wy
     Ey_test=0;
    for i = 1 : nLen
        Ey_test = Ey_test + (y(i) - MaxAmply * sin(w * i + fPhasey(MaxFreqy)))^2;
    end
	if Ey_test < ey
        ey = Ey_test;
        wy0 = w;
	end
 end

zizj = 0;
for i=nStart+1 : nStart+nTestCount-3
    zizj=zizj+z(i)*z(i+1);
end
gz = (z(nStart)*z(nStart+1)+z(nStart+nTestCount-2)*z(nStart+nTestCount-1)+2*zizj)/sum(z(nStart+1:(nStart+nTestCount-2)).^2);
wz = acos(gz/2);
ez = 10000000000;
 for w = wz-0.001 : 0.000001 : wz-0.0005
     Ez_test=0;
    for i = 1 : nLen
        Ez_test = Ez_test + (z(i) - MaxAmplz * sin(w * i + fPhasez(MaxFreqz)))^2;
    end
	if Ez_test < ez
        ez = Ez_test;
        wz0 = w;
	end
 end
 
for i = 1 : nLen
     MagReconfDataX(i - nStart + 1) = MaxAmplx*sin(wx0*i + fPhasex(MaxFreqx));
     MagReconfDataY(i - nStart + 1) = MaxAmply*sin(wy0*i + fPhasey(MaxFreqy));
     MagReconfDataZ(i - nStart + 1) = MaxAmplz*sin(wz0*i + fPhasez(MaxFreqz));
end


figure;
plot(1:nLen, x,'r',1:nLen,MagReconfDataX,'b')
hold on;
plot(1:nLen, y,'r',1:nLen,MagReconfDataY,'b')
plot(1:nLen, z,'r',1:nLen,MagReconfDataZ,'b')
legend('ԭ�ź�','����ź�');
xlabel('������');
ylabel('��ǿ���ź�Ht/nT')
% title('���ٸ���Ҷ/��С���������Ϻ��ع����ź���ԭ�ź�');




%%%%�����ع����źż���Ƕ�

B1(1) = MagReconfDataX(nTestCount);
B1(2) = MagReconfDataY(nTestCount);
B1(3) = MagReconfDataZ(nTestCount);

B2(1) = MagReconfDataX(nTestCount-10);
B2(2) = MagReconfDataY(nTestCount-10);
B2(3) = MagReconfDataZ(nTestCount-10);

B3 = cross(B2, B1);
Myz = Mzy';

A = Myz*B3';

Hcsx = A(1);
Hcsy = A(2);
Hcsz = A(3);

% %%%ת��Ϊ������ϵ��
% HBxcs = sqrt(Hcsx*Hcsx + Hcsy*Hcsy + Hcsz*Hcsz);
% HBycs = atan2(Hcsy,Hcsx);
% HBzcs = atan(Hcsz/sqrt(Hcsx*Hcsx + Hcsy*Hcsy));

%%����phi��
Angle = atan(sqrt(Hcsx*Hcsx + Hcsy*Hcsy) / Hcsz)*180/pi + 180;
sqrt(Hcsx*Hcsx + Hcsy*Hcsy) / Hcsz;
Phi2 = pFunc*[Angle^4 Angle^3 Angle^2 Angle 1]';

%%����theta��
Theta2 = atan2(Hcsy, Hcsx)*180/pi;

fx=wx0*1000/2/pi;
fy=wy0*1000/2/pi;
fz=wz0*1000/2/pi;





