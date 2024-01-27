function CC = my_autocorr(d,tt)
%autocorrelazione (temporale): funziona per VETTORI (d)! (entrate dal tempo 1
%al temp T) La autocorr e' misurta fino a distanza (temporale) tt

T = length(d);

if isscalar(tt)
    a = 0;
    b = tt;
elseif isvector(tt) && length(tt) == 2
    a = tt(1);
    b = tt(2);
else
    error("tt has invalid size")
end

tt = a:(b - 1);
CC = zeros(size(tt));

for t = tt
    CC(t - a + 1) = mean(d(1:(T-t)).*d((1+t):T));
end

CC = CC - (mean(d))^2;
CC = CC / CC(1);

end