function [E0, dE0,  DeltaE, dDeltaE] = estimate_energies(qho_sampler, a, N, m, omg)

[x2_samples, xx_samples] = qho_sampler.sample(a, N, m, omg);

e0_samples = m * omg ^ 2 .* x2_samples;

E0 = mean(e0_samples);

e0_autocorr = MYautocorr_fft(e0_samples, floor(length(e0_samples) / 2));
tau_int = max(cumsum(e0_autocorr)) - 0.5;

dE0 = std(e0_samples) / sqrt(length(e0_samples) / (2 * tau_int));

% xx_samples size: (Niter, max_shift)

N = size(xx_samples, 1);

xx_autocorrs = MYautocorr_fft(xx_samples, floor(N / 2));
tau_ints = max(cumsum(xx_autocorrs)) - 0.5;

xx_var = var(xx_samples) ./ (N ./ (2 * tau_ints));
weights = 1 ./ xx_var;

% A*x = B -> lscov(A,B,w)

B = log(mean(xx_samples));

max_shift = size(xx_samples, 2);
A = [-(1:max_shift) .* a; ones(1, max_shift)];

[x, stdx] = lscov(A', B', weights);

DeltaE = x(1);
dDeltaE = stdx(1);

end
