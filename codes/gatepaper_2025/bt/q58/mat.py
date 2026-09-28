#Code by Mudit
#Date:21/09/2026
import numpy as np

# Coefficient matrix
A = np.array([
    [6,    0, 0, -1,   -1,  0],
    [12,   3, 0, -1.8,  0, -2],
    [6,    0, 2, -0.5, -2, -1],
    [0,    1, 0, -0.2,  0,  0],
    [-2.4, 0, 0,  1,    0,  0],
    [1,    0, 0,  0,    0,  0]
], dtype=float)

# RHS vector
B = np.array([
    0,
    0,
    0,
    0,
    0,
    1
], dtype=float)

# Solve A x = B
x = np.linalg.solve(A, B)

# Store the coefficients
x1, x2, x3, x4, x5, x6 = x

print("Coefficients:")
print(f"x1 (C6H12O6) = {x1:.2f}")
print(f"x2 (NH3)      = {x2:.2f}")
print(f"x3 (O2)       = {x3:.2f}")
print(f"x4 (Biomass)  = {x4:.2f}")
print(f"x5 (CO2)      = {x5:.2f}")
print(f"x6 (H2O)      = {x6:.2f}")

print("\nMoles of O2 consumed per mole of glucose =", f"{x3:.2f}")
