function [Phi, p_acc] = estimate_energies(a, N, S_f, dSdPhi_f, m0, omg0)

m0_hat = m0 * a;
omg0_hat = omg0 * a;

Phi0 = zeros(1, N);
Nhmc = 20;

epsilon = find_best_epsilon(S_f, dSdPhi_f, Nhmc, Phi0, m0_hat, omg0_hat)
% epsilon = 0.01

Phi_warmup = HMC(epsilon, 1e4, Nhmc, S_f, dSdPhi_f, Phi0, m0_hat, omg0_hat);
[Phi_hat, p_acc] = HMC(epsilon, 1e5, Nhmc, S_f, dSdPhi_f, Phi_warmup(end, :), m0_hat, omg0_hat);

p_acc

Phi = Phi_hat * a;

E0_samples = m0 * omg0 ^ 2 * mean(Phi .^ 2, 2); % teorema viriale..

E0 = mean(E0_samples)

acf = my_autocorr(E0_samples, floor(N / 2));

tau_int = 0.5 + max(cumsum(acf));

dE0_wrong = std(E0_samples) / sqrt(length(E0_samples))
dE0 = std(E0_samples) / sqrt(length(E0_samples) / (2 * tau_int))


% for k=1:10
%     Phi_pad = [Phi Phi(:, 1:k)];
%     delta = mean(Phi_pad(:, (1 + k):end) .* Phi_pad(:, 1:(end - k)), 2);
% end

end


function best_epsilon = find_best_epsilon(S_f, dSdPhi_f, Nhmc, Phi0, m0_hat, omg0_hat)

    function target = f(epsilon)
        [~, p_acc] = HMC(epsilon, 1e3, Nhmc, S_f, dSdPhi_f, Phi0, m0_hat, omg0_hat);
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