function res = MYautocorr_fft(dd,tt)

% [Nsamples, N]

fft_size = 2^ceil(log2(tt));
res = fft(dd - mean(dd, 1), fft_size, 1);
res = ifft(res .* conj(res), fft_size, 1);
res = res(1:tt, :) ./ res(1, :);

% if nargin == 2
%     res = res(:, 1:(tt + 1));
% end

end