function res = tau_estrap(tau,d,dd,o,N)

if (nargin<5 || isempty(N))
    N = 1000;
end

if (nargin<4 || isempty(o)&&isempty(N))
    o = 1;
    N = 1000;
end

aa = [];

for i = 1:N
    ad = d + dd.*randn(size(dd));
    aux = polyfit(tau,ad,o);
    aa = [aa aux(end)];
end

res(1) = mean(aa);
res(2) = std(aa);
