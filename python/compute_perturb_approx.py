import numpy as np

# from math import sqrt
from numpy import sqrt
from scipy.sparse import lil_matrix


# def mat_elem(n1, n2):
#     return (
#         4 * n2 ** (3 / 2) * sqrt(n2 - 1) * (1 if n1 == n2 - 2 else 0)
#         + sqrt(n2)
#         * sqrt(n2 - 3)
#         * sqrt(n2 - 2)
#         * sqrt(n2 - 1)
#         * (1 if n1 == n2 - 4 else 0)
#         - 2 * sqrt(n2) * sqrt(n2 - 1) * (1 if n1 == n2 - 2 else 0)
#         + 6 * n2**2 * (1 if n1 == n2 else 0)
#         + 4 * n2 * sqrt(n2 + 1) * sqrt(n2 + 2) * (1 if n1 == n2 + 2 else 0)
#         + 6 * n2 * (1 if n1 == n2 else 0)
#         + sqrt(n2 + 1)
#         * sqrt(n2 + 2)
#         * sqrt(n2 + 3)
#         * sqrt(n2 + 4)
#         * (1 if n1 == n2 + 4 else 0)
#         + 6 * sqrt(n2 + 1) * sqrt(n2 + 2) * (1 if n1 == n2 + 2 else 0)
#         + 3 * (1 if n1 == n2 else 0)
#     )


def mat_elem(n, m):
    return (
        n ** (3 / 2) * sqrt(n - 1) * (1 if m == n - 2 else 0)
        + (1 / 4)
        * sqrt(n)
        * sqrt(n - 3)
        * sqrt(n - 2)
        * sqrt(n - 1)
        * (1 if m == n - 4 else 0)
        - 1 / 2 * sqrt(n) * sqrt(n - 1) * (1 if m == n - 2 else 0)
        + (3 / 2) * n**2 * (1 if m == n else 0)
        + n * sqrt(n + 1) * sqrt(n + 2) * (1 if m == n + 2 else 0)
        + (3 / 2) * n * (1 if m == n else 0)
        + (1 / 4)
        * sqrt(n + 1)
        * sqrt(n + 2)
        * sqrt(n + 3)
        * sqrt(n + 4)
        * (1 if m == n + 4 else 0)
        + (3 / 2) * sqrt(n + 1) * sqrt(n + 2) * (1 if m == n + 2 else 0)
        + (3 / 4) * (1 if m == n else 0)
    )


# def mat_elem(n1, n2):
#     res = 0.0

#     if n1 == n2 - 2:
#         res += 4 * n2 ** (3 / 2) * sqrt(n2 - 1)

#     if n1 == n2 - 4:
#         res += sqrt(n2) * sqrt(n2 - 3) * sqrt(n2 - 2) * sqrt(n2 - 1)

#     if n1 == n2 - 2:
#         res += -2 * sqrt(n2) * sqrt(n2 - 1)

#     if n1 == n2:
#         res += 6 * n2**2

#     if n1 == n2 + 2:
#         res += 4 * n2 * sqrt(n2 + 1) * sqrt(n2 + 2)

#     if n1 == n2:
#         res += 6 * n2

#     if n1 == n2 + 4:
#         res += sqrt(n2 + 1) * sqrt(n2 + 2) * sqrt(n2 + 3) * sqrt(n2 + 4)

#     if n1 == n2 + 2:
#         res += 6 * sqrt(n2 + 1) * sqrt(n2 + 2)

#     if n1 == n2:
#         res += 3

#     return res


def build_x4_mat(N):
    matrix = lil_matrix((N, N))

    for n1 in range(N):
        for n2 in range(n1 - 4, n1 + 4 + 1):
            if n2 < 0 or n2 >= N:
                continue

            el = mat_elem(n1 + 0j, n2 + 0j)
            assert np.isreal(el).all()
            matrix[n1, n2] = el.real

    return matrix


def delete_row_lil(mat, i):
    if not isinstance(mat, lil_matrix):
        raise ValueError("works only for LIL format -- use .tolil() first")

    new_mat = mat.copy()

    new_mat.rows = np.delete(new_mat.rows, i)
    new_mat.data = np.delete(new_mat.data, i)
    new_mat._shape = (new_mat._shape[0] - 1, new_mat._shape[1])

    return new_mat


lam = 1e-2
N = 100

En = np.arange(N) + 0.5
n1_x4_n2 = build_x4_mat(N)


# E0 = En[0]
# row = n1_x4_n2[0, 1:]
# col = n1_x4_n2[1:, 0]

# corr1 = n1_x4_n2[0, 0]

# corr2 = (row / (En[0] - En[1:]) * col).todense().item()

# term1 = (row / (En[0] - En[1:])) * n1_x4_n2[1:, 1:] * (col / (En[0] - En[1:, None]))
# term2 = n1_x4_n2[0, 0] * row / (En[0] - En[1:]) ** 2 * col
# corr3 = (term1 - term2).todense().item()

# E_corr = E0 + lam * corr1 + lam**2 * corr2 + lam**3 * corr3


i = 0


def compute_Ei(i):
    Ei = En[i]
    DeltaE = Ei - np.delete(En, i)

    row = delete_row_lil(n1_x4_n2[i].T, i).T
    col = delete_row_lil(n1_x4_n2[:, i], i)

    corr1 = n1_x4_n2[i, i]

    corr2 = (row / DeltaE * col).todense().item()

    reduced_x4 = delete_row_lil(delete_row_lil(n1_x4_n2.T, i).T, i)
    term1 = (row / DeltaE) * reduced_x4 * (col / DeltaE[:, None])
    term2 = n1_x4_n2[i, i] * row / DeltaE**2 * col
    corr3 = (term1 - term2).todense().item()

    return Ei + lam * corr1 + lam**2 * corr2 + lam**3 * corr3


E0 = compute_Ei(0)
E1 = compute_Ei(1)

print("E0:", E0)
print("E1:", E1)
print("DeltaE:", E1 - E0)

# print(E_corr)
