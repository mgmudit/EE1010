#Code by Mudit
#Date: 01/10/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Values of n
n = np.arange(11)

# Solution obtained from the inverse Z-transform
T = ((n**2 + n + 2) / 2) * 2**n

# Verify the difference equation
lhs = T[1:] - 2*T[:-1]
rhs = n[1:] * 2**n[1:]

# Print the results
print("\nZ-Transform Verification")

print("\nT(n) from inverse Z-transform:")
print(T.astype(int))

print("\nChecking T(n) - 2T(n-1) = n*2^n")

print("\nLeft side:")
print(lhs.astype(int))

print("\nRight side:")
print(rhs.astype(int))

print("\nDifference equation verified:", np.allclose(lhs, rhs))


# Plot T(n)
plt.plot(n, T, "o-", label="T(n)")

plt.xlabel("n")
plt.ylabel("T(n)")
plt.title("T(n) from Inverse Z-Transform")
plt.grid()
plt.legend()

# Save the plot as a PDF
plt.savefig("z_transform.pdf")

# Open the PDF
subprocess.run(["xdg-open", "z_transform.pdf"])
