#Code by Mudit
#Date: 24/09/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# x values
x = np.linspace(-3, 2, 500)

# The two conics
y1 = x**2
y2 = -x**2 - 2*x - 1

# Common chord
y3 = -x - 0.5

# Plot
plt.figure(figsize=(8, 6))

plt.plot(x, y1, label="y = x²")
plt.plot(x, y2, label="y = -x² - 2x - 1")
plt.plot(x, y3, '--', label="Common chord: y = -x - 1/2")

# Axes
plt.axhline(0)
plt.axvline(0)

plt.xlabel("x")
plt.ylabel("y")
plt.title("Intersection of Two Conics and Their Common Chord")
plt.grid(True)
plt.legend()

# Equal scaling of x and y
plt.axis("equal")

# Save the graph
filename = "/sdcard/Download/conics_chord.pdf"
plt.savefig(filename, dpi=150, bbox_inches="tight")

# Open the image using Android
subprocess.run(["termux-open", filename])

plt.close()
