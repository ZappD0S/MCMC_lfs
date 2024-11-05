function [E0, dE0, DeltaE, dDeltaE] = extrapolate_true_energies(E0s, dE0s, DeltaEs, dDeltaEs, T, aa, base_path)

[E0_fitobj, E0_start, E0_chi2] = find_best_fit_func(aa, E0s, dE0s, 0.5)

[E0, dE0] = bootstrap(1000, @(x, y) fit(x', y', E0_fitobj).a0, ...
    aa(E0_start:end), E0s(E0_start:end), dE0s(E0_start:end))

x = linspace(0, max(aa), 100);

clf("reset")

hold on;
errorbar(aa, E0s, dE0s, LineStyle="none")
plot(x, feval(E0_fitobj, x), "k", DisplayName=formula(E0_fitobj))
errorbar(0, E0, dE0, "r*", LineStyle="none", DisplayName=sprintf("E_0 = %f \\pm %f", E0, dE0))
hold off;

xlabel("$a$", Interpreter="latex")
ylabel("$E_0$", Interpreter="latex")
title(sprintf("Miglior $\\chi^2 = %.3f$ ($T = %.1f$, $a = %.1f \\dots %.1f$)", E0_chi2, T, aa(1), aa(end)), Interpreter="latex")
legend()

axis tight
axis padded

saveas(gcf, fullfile(base_path, "extrap_E0_fit.pdf"))

[DeltaE_fitobj, DeltaE_start, DeltaE_chi2] = find_best_fit_func(aa, DeltaEs, dDeltaEs, 1.0)

[DeltaE, dDeltaE] = bootstrap(1000, @(x, y) fit(x', y', DeltaE_fitobj).a0, ...
    aa(DeltaE_start:end), DeltaEs(DeltaE_start:end), dDeltaEs(DeltaE_start:end))

clf("reset")

hold on;
errorbar(aa, DeltaEs, dDeltaEs, LineStyle="none")
plot(x, feval(DeltaE_fitobj, x), "k", DisplayName=formula(DeltaE_fitobj))
errorbar(0, DeltaE, dDeltaE, "r*", LineStyle="none", DisplayName=sprintf("\\Delta E = %f \\pm %f", DeltaE, dDeltaE))
hold off;

xlabel("$a$", Interpreter="latex")
ylabel("$\Delta E$", Interpreter="latex")
title(sprintf("Miglior $\\chi^2 = %.3f$ ($T = %.1f$, $a = %.1f \\dots %.1f$)", DeltaE_chi2, T, aa(1), aa(end)), Interpreter="latex")
legend()

axis tight
axis padded

saveas(gcf, fullfile(base_path, "extrap_DeltaE_fit.pdf"))

end
