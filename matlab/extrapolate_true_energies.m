function [E0s, dE0s, DeltaEs, dDeltaEs] = extrapolate_true_energies(sampler, T, Ns, m, omg)

E0s = zeros(size(Ns));
dE0s = zeros(size(Ns));

DeltaEs = zeros(size(Ns));
dDeltaEs = zeros(size(Ns));

aa = T ./ Ns;

for i=1:length(Ns)
    N = Ns(i);
    a = aa(i);
    [E0, dE0, DeltaE, dDeltaE] = estimate_energies(sampler, a, N, m, omg);

    E0s(i) = E0;
    dE0s(i) = dE0;
    DeltaEs(i) = DeltaE;
    dDeltaEs(i) = dDeltaE;
end

[funcs, n_vars_arr] = generate_function_strings(6);

weights = 1 ./ (dE0s .^ 2);
% starts = 1:floor(length(Ns) / 4);
starts = 1:15;

chi2_arr = zeros(length(starts), length(funcs));
errorbar(aa, E0s, dE0s)

for i=1:length(starts)
    start = starts(i);

    for j=1:length(funcs)
        f = funcs(j);
        n_vars = n_vars_arr(j);

        fitobj = fit(aa(start:end)', E0s(start:end)', f, Weights=weights(start:end), StartPoint=[zeros(1, n_vars - 1) 0.5]);

        err_sum = weights(start:end) * (E0s(start:end)' - feval(fitobj, aa(start:end))) .^ 2;
        df = length(E0s(start:end)) - n_vars;
        chi2_arr(i, j) = err_sum / df;
    end
end

[~, idx_best] = min(abs(chi2_arr - 1), [], "all");
best_chi2 = chi2_arr(idx_best)
[r, c] = ind2sub(size(chi2_arr), idx_best);
start = starts(r)
f = funcs(c)
n_vars = n_vars_arr(c);

    function E0 = do_fit(x, y)
        fitobj = fit(x', y', f, StartPoint=[zeros(1, n_vars - 1) 0.5]);
        E0 = fitobj.a0;
    end

[E0, dE0] = bootstrap(1000, @do_fit, aa, E0s, dE0s)

end

