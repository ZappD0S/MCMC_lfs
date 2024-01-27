function CC = MYautocorr(d,tt)
%autocorrelazione (temporale): funziona per VETTORI (d)! (entrate dal tempo 1
%al temp T) La autocorr e' misurta fino a distanza (temporale) tt

T = length(d);

CC = zeros(1, tt);

for t = 0:tt
    CC(t+1) = mean(d(1:(T-t)).*d((1+t):T));
end

CC = CC - (mean(d))^2;

CC = CC/CC(1);

end