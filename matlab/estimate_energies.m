function [E0, dE0, DeltaE, dDeltaE] = estimate_energies(qho_sampler, a, N, m, omg)

[x2_samples, xx_samples] = qho_sampler.sample(a, N, m, omg);

e0_samples = m * omg ^ 2 .* x2_samples;

E0 = mean(e0_samples);

e0_autocorr = MYautocorr_fft(e0_samples, floor(length(e0_samples) / 2));

idx = find(e0_autocorr < 0, 1);
tau_int = sum(e0_autocorr(1:(idx - 1))) - 0.5;

dE0 = std(e0_samples) / sqrt(length(e0_samples) / (2 * tau_int));

% xx_samples size: (Niter, max_shift)

[Niter, max_shift] = size(xx_samples);

x = (1:max_shift) .* a;
y = mean(xx_samples);

dy = zeros(1, max_shift);

for i=1:max_shift
    xx_autocorr = MYautocorr_fft(xx_samples(:, i), floor(Niter / 2));
    idx = find(xx_autocorr < 0, 1);
    tau_int = sum(xx_autocorr(1:(idx - 1))) - 0.5;
    dy(i) = std(xx_samples(:, i)) / sqrt(Niter / (2 * tau_int));
end


    function dE = do_fit(x, y)
        fitobj = fit(x', y', @(C, dE, x) C * exp(-dE * x), StartPoint=[0.5, 1]);
        dE = fitobj.dE;
    end

% errorbar(x, y, dy)

[DeltaE, dDeltaE] = bootstrap(1000, @do_fit, x, y, dy);

% res = bootstrp(1000, @do_fit, x', y', (1:max_shift)');
% res = bootstrp(1000, @do_fit, x', y');

% bootci(1000, @do_fit, x', y', (1:max_shift)')

end
