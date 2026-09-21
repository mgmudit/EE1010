#Code by Mudit
#Date: 21/09/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Enter values of a and b
a = float(input("Enter value of a: "))
b = float(input("Enter value of b: "))

# x values
x_left = np.linspace(-5, 0, 500)
x_right = np.linspace(0, 5, 500)

# Piecewise function
y_left = a + b * x_left
y_right = np.sin(2 * x_right)

# Plot
plt.plot(x_left, y_left, label=r"$a+bx$")
plt.plot(x_right, y_right, label=r"$\sin(2x)$")

# Mark x = 0
plt.axvline(0, linestyle="--")

plt.xlabel("x")
plt.ylabel("f(x)")
plt.title("Q46: Checking Differentiability at x = 0")
plt.grid(True)
plt.legend()

# Save as PDF
filename = "q46_differentiability.pdf"
plt.savefig(filename, format="pdf", bbox_inches="tight")

# Automatically open PDF in Android
subprocess.run(["termux-open", filename])

# Print entered values
print("\nEntered values:")
print(f"a = {a}")
print(f"b = {b}")
print(f"a + b = {a + b}")

# Mathematical verification
if a == 0 and b == 2:
    print("\nThe function is differentiable at x = 0.")
    print("Therefore, a + b = 2")
else:
    print("\nThe function is NOT differentiable at x = 0.")
    print("For differentiability, we require:")
    print("a = 0 and b = 2")
    print("Hence the required answer is a + b = 2")
