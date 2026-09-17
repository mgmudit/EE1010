import numpy as np
import matplotlib.pyplot as plt
import subprocess

k = float(input("Enter k: "))

A = np.array([[2., 3.],
              [4., 6.]])

B = np.array([[6.],
              [3*k]])

M = np.hstack((A, B))
M[1] = M[1] - 2*M[0]

print("Echelon form:")
print(M)
print("Ranks:", np.linalg.matrix_rank(A),
      np.linalg.matrix_rank(M))

x = np.linspace(-5, 5, 100)
y1 = (6 - 2*x) / 3
y2 = (3*k - 4*x) / 6

plt.plot(x, y1, label="2x + 3y = 6")
plt.plot(x, y2, "--", label=f"4x + 6y = {3*k:g}")

plt.xlabel("x")
plt.ylabel("y")
plt.grid()
plt.legend()
plt.title(f"System of equations for k = {k:g}")

path = "/sdcard/Download/q34.pdf"
plt.savefig(path)
plt.close()

subprocess.run(["termux-open", path])
