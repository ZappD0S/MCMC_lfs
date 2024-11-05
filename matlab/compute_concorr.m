function [concorr, err]  = compute_concorr(x_samples, max_shift)
N = size(x_samples, 2);

x_mean = mean(x_samples, 1);
dx = std_autocorr(x_samples);

concorr = zeros(N, max_shift);
err = zeros(N, max_shift);

x_samples_shift = x_samples;
x_mean_shift = x_mean;
dx_shift = dx;

for i=1:max_shift
    x_samples_shift = circshift(x_samples_shift, 1, 2);
    x_mean_shift = circshift(x_mean_shift, 1);
    dx_shift = circshift(dx_shift, 1);

    xx = x_samples .* x_samples_shift;
    % this is <x_i x_j>
    xx_mean = mean(xx, 1);
    dxx = std_autocorr(xx);

    % this is <x_i> <x_j>
    x_x_mean = x_mean .* x_mean_shift;
    dx_x = abs(x_x_mean) .* sqrt((dx ./ x_mean).^2 + (dx_shift ./ x_mean_shift).^2);

    concorr(:, i) = xx_mean - x_x_mean;
    err(:, i) = sqrt(dxx.^2 + dx_x.^2);
end

concorr = mean(concorr);
% err = sqrt(mean(err.^2));
err = sqrt(sum(err.^2, 1)) ./ size(err, 1);

end