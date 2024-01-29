
function [Phi, Pi] = leapfrog(Phi0, Pi0, epsilon, dHdPhi, Niter, varargin)
[Phi, Pi] = leapfrog_base(Phi0, Pi0, epsilon, dHdPhi, Niter, varargin{:});
[Phi0_rev, Pi0_rev] = leapfrog_base(Phi, Pi, -epsilon, dHdPhi, Niter, varargin{:});

if not(all(isclose([Phi0 Pi0], [Phi0_rev Pi0_rev])))
    error("leapfrog is not reversible");
end

end

function [Phi, Pi] = leapfrog_base(Phi0, Pi0, epsilon, dHdPhi, Niter, varargin)

Phi = Phi0;
Pi = Pi0 - 0.5 * epsilon * dHdPhi(Phi, varargin{:});

for i = 1:Niter
    Phi = Phi + epsilon * Pi;
    Pi = Pi - epsilon * dHdPhi(Phi, varargin{:});
end

Phi = Phi + epsilon * Pi;
Pi = Pi - 0.5 * epsilon * dHdPhi(Phi, varargin{:});

end

