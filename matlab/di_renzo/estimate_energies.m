function acf = estimate_energies(a, N, m0, omg0)

m0_hat = m0 * a;
omg0_hat = omg0 * a;

Phi0 = zeros(1, N);
Nhmc = 20;
% epsilon = 50;

% Nhmc = find_best_Nhmc(epsilon, Phi0, m0_hat, omg0_hat)

epsilon = find_best_epsilon(Nhmc, Phi0, m0_hat, omg0_hat)

Phi_warmup = HMCqmho(m0_hat, omg0_hat, N, epsilon, Nhmc, 1e6, Phi0);
[Phi_hat, p_acc] = HMCqmho(m0_hat, omg0_hat, N, epsilon, Nhmc, 1e6, Phi_warmup(end, :));

p_acc

Phi = Phi_hat * a;

E0_samples = m0 * omg0 ^ 2 * mean(Phi .^ 2, 2); % teorema viriale..

E0 = mean(E0_samples)


acf = MYautocorr(E0_samples, length(E0_samples));
plot(cumsum(acf));
tau_int = 0.5 + max(cumsum(acf));

dE0_wrong = std(E0_samples) / sqrt(length(E0_samples))
dE0 = std(E0_samples) / sqrt(length(E0_samples) / (2 * tau_int))

end

function best_epsilon = find_best_epsilon(Nhmc, Phi0, m0_hat, omg0_hat)

    function target = f(epsilon)
        [~, p_acc] = HMCqmho(m0_hat, omg0_hat, length(Phi0), epsilon, Nhmc, 1e3, Phi0);
        target = p_acc - 0.5;
    end

upper = 1;

while true
    try
        best_epsilon = fzero(@f, [0, upper]);
        break;
    catch ME
        if ME.identifier == "MATLAB:fzero:ValuesAtEndPtsSameSign"
            upper = upper * 1.5;
        elseif ME.identifier ==  "MATLAB:fzero:InvalidFunctionSupplied"
            upper = upper / 1.5;
        else
            rethrow(ME)
        end
    end
end

end