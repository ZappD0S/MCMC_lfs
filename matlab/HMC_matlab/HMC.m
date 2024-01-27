function [PhiOUT, p_acc] = HMC(epsilon, Niter, Nhmc, S_f, dSdPhi_f, Phi0, varargin)

PhiOUT = zeros(Niter, size(Phi0, 2));
Phi = Phi0;
acc_count = 0;

for i = 1:Niter
    Pi = randn(size(Phi));

    [Phi_proposal, Pi_proposal] = leapfrog(Phi, Pi, epsilon, dSdPhi_f, Nhmc, varargin{:});

    H = 0.5 * sum(Pi .^ 2) + S_f(Phi, varargin{:});
    H_proposal = 0.5 * sum(Pi_proposal .^ 2) + S_f(Phi_proposal, varargin{:});

    dH = H_proposal - H;

    % if rand() < min(1, exp(-dH))
    if (dH <= 0) || (rand() < exp(-dH))
        Phi = Phi_proposal;
        acc_count = acc_count + 1;
    end

    PhiOUT(i, :) = Phi;
end

p_acc = acc_count / Niter;

end
