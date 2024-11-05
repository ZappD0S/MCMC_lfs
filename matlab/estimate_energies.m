function [E0s, dE0s, DeltaEs, dDeltaEs, DeltaEs_creutz, dDeltaEs_creutz] = estimate_energies(sampler, aa, Ns, base_path)

E0s = zeros(size(Ns));
dE0s = zeros(size(Ns));

DeltaEs = zeros(size(Ns));
dDeltaEs = zeros(size(Ns));

DeltaEs_creutz = zeros(size(Ns));
dDeltaEs_creutz = zeros(size(Ns));

for i=1:length(Ns)
    N = Ns(i);
    a = aa(i);
    [E0, dE0, DeltaE, dDeltaE, DeltaE_creutz, dDeltaE_creutz] = estimate_e0_deltae(sampler, a, N, base_path);

    E0s(i) = E0;
    dE0s(i) = dE0;

    DeltaEs(i) = DeltaE;
    dDeltaEs(i) = dDeltaE;

    DeltaEs_creutz(i) = DeltaE_creutz;
    dDeltaEs_creutz(i) = dDeltaE_creutz;
end

end
