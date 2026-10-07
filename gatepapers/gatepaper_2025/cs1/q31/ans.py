#Code by Mudit
#Date: 07/10/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Values obtained from the differentiability condition
a = 5
b = -2

# x < 1
x1 = np.linspace(-3, 1, 300, endpoint=False)
y1 = a * x1 + b

# x >= 1
x2 = np.linspace(1, 3, 300)
y2 = x2**3 + x2**2 + 1

# Plot both pieces
plt.plot(x1, y1, label="5x - 2")
plt.plot(x2, y2, label="x^3 + x^2 + 1")

# Point where the two pieces meet
plt.scatter(1, 3, s=50, zorder=3, label="(1, 3)")

plt.xlabel("x")
plt.ylabel("f(x)")
plt.title("f(x) for a = 5, b = -2")
plt.legend()
plt.grid(True)
plt.tight_layout()

# Save as PDF
plt.savefig("piecewise_function.pdf")
plt.close()

# Open the PDF
subprocess.run(["xdg-open", "piecewise_function.pdf"])
