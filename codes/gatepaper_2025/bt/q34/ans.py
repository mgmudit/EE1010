#Code by Mudit
#Date: 17/09/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess
import sys

sys.path.insert(0, "/sdcard/github/matgeo/codes/CoordGeo")
# Source: github.com/gadepall/matgeo/codes/CoordGeo/line/funcs.py
from line.funcs import *

k = float(input("Enter k: "))

A = np.array([[2., 3.],
              [4., 6.]])

B = np.array([[6.],
              [3*k]])

# Form the augmented matrix
M = np.block([A, B])

# Row operation R2 -> R2 - 2R1
M[1] = M[1] - 2*M[0]

print("Echelon form:")
print(M)
print("Ranks of [A] and [A|B]: ", np.linalg.matrix_rank(A),
      np.linalg.matrix_rank(M))

# Two points on 2x + 3y = 6
P1 = np.array([0., 2.])
P2 = np.array([3., 0.])

# Two points on 4x + 6y = 3k
Q1 = np.array([0., k/2])
Q2 = np.array([3*k/4, 0.])

# Generate the two lines using CoordGeo
L1 = line_gen(P1, P2)
L2 = line_gen(Q1, Q2)

plt.plot(L1[0], L1[1], label="2x + 3y = 6")
plt.plot(L2[0], L2[1], label=f"4x + 6y = {3*k:g}")

plt.xlabel("x")
plt.ylabel("y")
plt.grid()
plt.legend()
plt.axis("equal")

path = "/sdcard/Download/ans.pdf"
plt.savefig(path)
plt.close()

subprocess.run(["termux-open", path])
