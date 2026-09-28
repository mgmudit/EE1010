#Code by Mudit
#Date: 28/09/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Generate x-values
x = np.linspace(0, 7, 500)

# Equations of the two lines
y1 = 7 - x
y2 = 13 - 3*x

# Point of intersection
x0, y0 = 3, 4

plt.figure(figsize=(8, 6))

# Plot the lines
plt.plot(x, y1, label="x + y = 7")
plt.plot(x, y2, label="3x + y = 13")

# Mark the intersection point
plt.scatter(x0, y0, s=80, label="Point (3, 4)")

plt.axhline(0)
plt.axvline(0)

plt.xlabel("x")
plt.ylabel("y")
plt.title("Lines and Their Point of Intersection")
plt.grid(True)
plt.legend()
plt.axis("equal")

# Save and open the plot
filename = "/sdcard/Download/lines_intersection.png"
plt.savefig(filename, dpi=150, bbox_inches="tight")
plt.close()

subprocess.run(["termux-open", filename])
