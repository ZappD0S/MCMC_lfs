function res = is_leapfrog_reversible(Phi0, Pi0, epsilon, dHdPhi, Niter, varargin)
[Phi, Pi] = leapfrog(Phi0, Pi0, epsilon, dHdPhi, Niter, varargin{:});
[Phi0_rev, Pi0_rev] = leapfrog(Phi, Pi, -epsilon, dHdPhi, Niter, varargin{:});

res = all(isclose(Phi0, Phi0_rev)) && all(isclose(Pi0, Pi0_rev));
end