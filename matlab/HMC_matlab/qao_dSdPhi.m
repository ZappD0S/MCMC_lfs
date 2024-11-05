function [dSdPhi] = qao_dSdPhi(Phi, m0, omg0, lamb)

Phi_p = circshift(Phi, -1);
Phi_m = circshift(Phi, 1);
delta = Phi_p + Phi_m;

dSdPhi = m0 * (delta - (2 + omg0 ^ 2) * Phi) + 4 * lamb * Phi .^ 3;

end
