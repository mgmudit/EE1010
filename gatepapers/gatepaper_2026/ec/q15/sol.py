#Code by Mudit
#Date: 29/10/2026


import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
import subprocess
import os

# Sampling frequency and sample times
Fs = 40
Ts = 1 / Fs
n = np.arange(10)
ts = n * Ts

# Define the two continuous signals
t = np.linspace(0, 0.225, 2000)
x1 = np.cos(20 * np.pi * t)
x2 = np.cos(100 * np.pi * t)

# Calculate the first ten samples
s1 = np.cos(20 * np.pi * ts)
s2 = np.cos(100 * np.pi * ts)

# Print the sample table
print(" n       Time       x1[n]       x2[n]")
for i in range(10):
    print(f"{n[i]:2d}    {ts[i]:.3f}       "
          f"{s1[i]: .2f}        {s2[i]: .2f}")

print("\nSamples equal:", np.allclose(s1, s2))

# Create two plots
fig, ax = plt.subplots(2, 1, figsize=(10, 8))

# Plot 1: Continuous signals and their sample points
ax[0].plot(t, x1, label="x1(t) = cos(20πt)")
ax[0].plot(t, x2, label="x2(t) = cos(100πt)")
ax[0].scatter(ts, s1, color="red", label="Samples of x1", zorder=3)
ax[0].scatter(ts, s2, marker="x", color="black",
               label="Samples of x2", zorder=4)

ax[0].set_title("Continuous Signals and Sample Points")
ax[0].set_xlabel("Time (seconds)")
ax[0].set_ylabel("Amplitude")
ax[0].grid(True)
ax[0].legend()

# Plot 2: Compare the ten discrete samples
ax[1].stem(n, s1, linefmt="C0-", markerfmt="C0o",
           basefmt="k-", label="x1 samples")
ax[1].plot(n, s2, "rx", markersize=10,
           label="x2 samples")

ax[1].set_title("First Ten Samples")
ax[1].set_xlabel("Sample index n")
ax[1].set_ylabel("Amplitude")
ax[1].set_xticks(n)
ax[1].set_ylim(-1.3, 1.3)
ax[1].grid(True)
ax[1].legend()

plt.tight_layout()

# Save the graph
filename = os.path.abspath("sampling_plot.png")
plt.savefig(filename, dpi=180)
plt.close()

print("\nGraph saved:", filename)

# Automatically open the graph in Termux
try:
    subprocess.run(["termux-open", filename], check=False)
except FileNotFoundError:
    print("Automatic opening unavailable; open the PNG manually.")

