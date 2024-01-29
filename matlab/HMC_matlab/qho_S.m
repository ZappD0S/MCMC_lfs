function [S] = qho_S(Phi, m0, omg0)
% function [S] = qho_S(Phi, m0, omg0, a)

Phi_p = circshift(Phi, -1);
% delta = Phi_p - Phi;

% S = 0.5 * m0 / a * sum((Phi_p - Phi) .^ 2) + 0.5 * a * m0 * omg0 ^ 2 * sum(Phi .^ 2);
% S = 0.5 * m0 * sum((Phi_p - Phi) .^ 2) + 0.5 * m0 * omg0 ^ 2 * sum(Phi .^ 2);
S = 0.5 * m0 * sum((Phi_p - Phi) .^ 2 + omg0 ^ 2 * Phi .^ 2);

end
