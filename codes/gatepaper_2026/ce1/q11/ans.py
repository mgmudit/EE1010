#Code by Mudit
#Date: 24/09/2026
import numpy as np

# Define the matrix P
P = np.array([
    [1, 0, 1],
    [0, 1, 0],
    [1, 0, 1]
])

print("Matrix P:")
print(P)

# Calculate the trace of P
trace_P = np.trace(P)

# Calculate the eigenvalues of P
eigenvalues = np.linalg.eigvals(P)

# Calculate the sum of the eigenvalues
sum_eigenvalues = np.sum(eigenvalues)

print("\nOption A:")
print("Trace of P =", trace_P)
print("Eigenvalues =", eigenvalues)
print("Sum of eigenvalues =", sum_eigenvalues)
print("A is ", np.isclose(trace_P, sum_eigenvalues))

# Calculate P transpose multiplied by P
PTP = P.T @ P

# Create the 3x3 identity matrix
I = np.eye(3)

print("\nOption B:")
print("P^T P =")
print(PTP)
print("Identity matrix =")
print(I)
print("B is ", np.array_equal(PTP, I))

# Check whether P is skew-symmetric
print("\nOption C:")
print("P^T =")
print(P.T)
print("-P =")
print(-P)
print("C is ", np.array_equal(P.T, -P))

# Calculate the absolute values of all eigenvalues
absolute_eigenvalues = np.abs(eigenvalues)

print("\nOption D:")
print("Eigenvalues =", eigenvalues)
print("Absolute values of eigenvalues =", absolute_eigenvalues)
print("D is ", np.allclose(absolute_eigenvalues, 1))

# Print the final answer
print("\nThe correct option is A.")
