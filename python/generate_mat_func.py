from sympy.physics.secondquant import B, Bd, BKet, BBra, apply_operators
from sympy.abc import n, m
from sympy import symbols, lambdify, simplify, latex, sqrt
import inspect


res = apply_operators(BBra((m,)) * ((B(0) + Bd(0))/ sqrt(2)) ** 4 * BKet((n,)))
res = simplify(res)

print(latex(res))

func = lambdify((n, m), res, modules="numpy")

print()
print(inspect.getsource(func))
