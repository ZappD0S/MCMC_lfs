function [S] = qao_S(Phi, m0, omg0, lamb)

    % remember: lamb_hat = lamb * a ^ 5

    Phi_p = circshift(Phi, 1);
    delta = Phi_p - Phi;

    S = 0.5 * m0 * sum(delta .^ 2 + omg0 ^ 2 * Phi .^ 2) + lamb * sum(Phi .^ 4);
    end
