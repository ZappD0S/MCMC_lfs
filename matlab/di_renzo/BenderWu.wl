(* ::Package:: *)

VZero = 3/4 EigV[0] + 3 Sqrt[2]/2 EigV[2] + Sqrt[6]/2 EigV[4];

EigV[n_Integer] := 0 /; n<0;

Bracket[a__ EigV[n_Integer],c__] := a Bracket[EigV[n],c]; 
Bracket[c__,a__ EigV[n_Integer]] := a Bracket[c,EigV[n]]; 

Bracket[Plus[a_. EigV[n_Integer],b__],c__] := 
a Bracket[EigV[n],c] + Bracket[Plus[b],c]; 

Bracket[c__,Plus[a_. EigV[n_Integer],b__]] := 
a Bracket[c,EigV[n]] + Bracket[c,Plus[b]]; 

Bracket[EigV[n_Integer],EigV[m_Integer]] := If[n==m,1,0];

OpeR[x__] := OpeR[ExpandAll[x]];

OpeR[a__ EigV[n_Integer]] := a OpeR[EigV[n]]; 
OpeR[Plus[a_. EigV[n_Integer],b__]] := a OpeR[EigV[n]] + OpeR[Plus[b]];
OpeR[EigV[n_Integer]] := 1/n EigV[n] /; n>0; 
OpeR[EigV[0]] := 0;

OpeV[x__] := OpeV[ExpandAll[x]];

OpeV[a__ EigV[n_Integer]] := a OpeV[EigV[n]]; 
OpeV[Plus[a_. EigV[n_Integer],b__]] := a OpeV[EigV[n]] + OpeV[Plus[b]];
OpeV[EigV[n_Integer]] := 1/4(Sqrt[n(n-1)(n-2)(n-3)] EigV[n-4] + (4n-2)*Sqrt[n(n-1)] EigV[n-2] + 3(2 n^2+2n+1) EigV[n] + (4n+6)*Sqrt[(n+1)(n+2)] EigV[n+2] + Sqrt[(n+1)(n+2)(n+3)(n+4)] EigV[n+4]);

deltaE[n_Integer] := deltaE[n] = Bracket[VZero,deltaV[n-1]];

deltaV[0] = EigV[0];
deltaV[1] = - OpeR[VZero];

deltaV[n_Integer] := deltaV[n] = OpeR[Sum[deltaE[j] deltaV[n-j],{j,1,n-1}] - OpeV[deltaV[n-1]]];

