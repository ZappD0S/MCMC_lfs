function [E0s, dE0s, DeltaEs, dDeltaEs] = extrapolate_true_energies(qho_sampler, T, Ns, m, omg)

% Ns = round(linspace(N0, Nmax, steps));

E0s = zeros(size(Ns));
dE0s = zeros(size(Ns));

DeltaEs = zeros(size(Ns));
dDeltaEs = zeros(size(Ns));

aa = T ./ Ns;

for i=1:length(Ns)
    N = Ns(i);
    a = aa(i);
    [E0, dE0, DeltaE, dDeltaE] = estimate_energies(qho_sampler, a, N, m, omg);

    E0s(i) = E0;
    dE0s(i) = dE0;
    DeltaEs(i) = DeltaE;
    dDeltaEs(i) = dDeltaE;
end

[funcs, n_vars_arr] = generate_function_strings(6);

weights = 1 ./ (dE0s .^ 2);
weights = weights ./ sum(weights);

rmse_arr = zeros(1, length(funcs));
errorbar(aa, E0s, dE0s)

for i=1:length(funcs)
    f = funcs(i);
    n_vars = n_vars_arr(i);

    [~, gof] = fit(aa', E0s', f, Weights=weights, StartPoint=[zeros(1, n_vars - 1) 0.5]);
    rmse_arr(i) = gof.rmse;
end

[min_rmse, idx_min] = min(rmse_arr)
best_f = funcs(idx_min)
best_f_n_vars = n_vars_arr(idx_min);

function E0 = do_fit(x, y)
    fitobj = fit(x', y', best_f, StartPoint=[zeros(1, best_f_n_vars - 1) 0.5]);
    E0 = fitobj.a0;
end
% [rmse_sorted, rmse_order] = sort(rmse_arr);
% fitobjs = fitobjs(rmse_order);
[E0, dE0] = bootstrap(1000, @do_fit, aa, E0s, dE0s)

end

