function [E0, dE0, DeltaE, dDeltaE, DeltaE_creutz, dDeltaE_creutz] = estimate_e0_deltae(sampler, a, N, base_path)

[e0_samples, xx_samples] = sampler.sample(a, N);

E0 = mean(e0_samples);

% for now...
e0_autocorr = MYautocorr_fft(e0_samples', floor(length(e0_samples) / 2))';

idx = find(e0_autocorr < 0, 1);

tau_int = sum(e0_autocorr(1:(idx - 1))) - 0.5;

dE0 = std(e0_samples) / sqrt(length(e0_samples) / (2 * tau_int));

% xx_samples size: (Niter, max_shift)

[~, max_shift] = size(xx_samples);

x = (1:max_shift) .* a;
y = mean(xx_samples);

dy = std_autocorr(xx_samples);

assert(all(size(y) == size(dy)))

% dy = zeros(1, max_shift);
% for i=1:max_shift
%     xx_autocorr = MYautocorr_fft(xx_samples(:, i), floor(Niter / 2));
%     idx = find(xx_autocorr < 0, 1);
%     tau_int = sum(xx_autocorr(1:(idx - 1))) - 0.5;
%     dy(i) = std(xx_samples(:, i)) / sqrt(Niter / (2 * tau_int));
% end

[DeltaE_creutz, dDeltaE_creutz] = estimate_deltae_creutz(y, dy, a);

% clf("reset");

% dts = (1:length(DeltaE_creutz)) .* a;

% errorbar(dts, DeltaE_creutz, dDeltaE_creutz)
% xlabel("$\Delta \tau = k a$", Interpreter="latex", fontsize=16)
% ylabel("$\Delta E$", Interpreter="latex", fontsize=16)
% legend()

% subpath = fullfile(base_path, "creutz");
% [~, ~] = mkdir(subpath);
% saveas(gcf, fullfile(subpath, sprintf("creutz_deltae_a_%f_N_%d.png", a, N)))


fitobj = fit(x', y', @(C, dE, x) C * exp(-dE * x), StartPoint=[0.5, 1]);

    function dE = do_fit(x, y)
        dE = fit(x', y', fitobj).dE;
    end

[DeltaE, dDeltaE] = bootstrap(1000, @do_fit, x, y, dy);
base_path = fullfile(base_path, "autocorr");
[~, ~] = mkdir(base_path);

clf("reset");

hold on;
errorbar(x, y, dy, "o", "DisplayName", "dati")
x1 = linspace(x(1), x(end), 100);
plot(x1, feval(fitobj, x1), "r", "DisplayName", "fit")
hold off;

title(sprintf("$\\Delta E = %f \\pm %f$", DeltaE, dDeltaE), Interpreter="latex")
xlabel("$\Delta \tau = k a$", Interpreter="latex", fontsize=16)
ylabel("$\left\langle x_i x_{i+k} \right\rangle$", Interpreter="latex", fontsize=16)
legend()

axis tight
axis padded

saveas(gcf, fullfile(base_path, sprintf("autocorr_a_%.2f_N_%d.png", a, N)))

end
