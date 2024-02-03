function res = MYautocorr_fft(dd,tt)

res = fft(dd-mean(dd));
res = ifft(res.*conj(res));
res = res/res(1);

if nargin==2
    res = res(1:(tt+1));
end

end