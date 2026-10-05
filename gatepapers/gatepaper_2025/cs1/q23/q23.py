# Code by Mudit
# Date: 05/10/2026

import numpy as np
import matplotlib.pyplot as plt
import subprocess


# Values of k for different types of solutions
print("Values of k:")
print("No solution              : k = 1")
print("Infinitely many solutions: k = -1")
print("Unique solution          : k != 1 and k != -1")


# Take k from the user
k = float(input("\nEnter k: "))


# Coefficient matrix
A = np.array([[1, k],
              [k, 1]], dtype=float)

# Constant matrix
B = np.array([[1],
              [-1]], dtype=float)

# Augmented matrix
M = np.hstack((A, B))


# Row operation: R2 -> R2 - kR1
M[1] = M[1] - k * M[0]

print("\nAugmented matrix after row operation:")
print(M)


# Find ranks
rank_A = np.linalg.matrix_rank(A)
rank_M = np.linalg.matrix_rank(M)

print("\nRank of A =", rank_A)
print("Rank of [A|B] =", rank_M)


# Determine the type of solution
if rank_A < rank_M:
    print("\nNo solution")

elif rank_A == rank_M and rank_A < 2:
    print("\nInfinitely many solutions")

else:
    print("\nUnique solution")


# Generate points for the two lines

if k != 0:

    # x + ky = 1
    P1 = np.array([-10, 11/k])
    P2 = np.array([10, -9/k])

    # kx + y = -1
    Q1 = np.array([-10, 10*k - 1])
    Q2 = np.array([10, -10*k - 1])

else:

    # x = 1
    P1 = np.array([1, -10])
    P2 = np.array([1, 10])

    # y = -1
    Q1 = np.array([-10, -1])
    Q2 = np.array([10, -1])


# Plot the two lines
plt.figure(figsize=(8, 6))

plt.plot([P1[0], P2[0]], [P1[1], P2[1]],
         label="x + ky = 1")

plt.plot([Q1[0], Q2[0]], [Q1[1], Q2[1]],
         label="kx + y = -1")

plt.xlabel("x")
plt.ylabel("y")
plt.title(f"Two lines for k = {k}")

plt.grid()
plt.legend()
plt.axhline(0, linewidth=0.8)
plt.axvline(0, linewidth=0.8)

# Save as PDF
path = "/sdcard/Download/two_lines.pdf"
plt.savefig(path, format="pdf", bbox_inches="tight")

plt.close()

# Open PDF
subprocess.run(["termux-open", path])
