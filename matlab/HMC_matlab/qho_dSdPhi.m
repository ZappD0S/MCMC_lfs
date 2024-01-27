function [dSdPhi] = qho_dSdPhi(Phi, m0, omg0)
% function [dSdPhi] = qho_dSdPhi(Phi, m0, omg0, a)

Phi_p = circshift(Phi, -1);
Phi_m = circshift(Phi, 1);

delta = Phi_p + Phi_m;
dSdPhi = m0 * (-delta + (2 + omg0 ^ 2) * Phi);

% dSdPhi = m0 / a * (-Phi_m + 2 * Phi - Phi_p)  +  a * m0 * omg0 ^ 2 * Phi;

end
