#Code by Mudit
#Date: 05/10/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Take k from the user
k = float(input("Enter the value of k: "))

# x values for plotting
x = np.linspace(-10, 10, 500)

plt.figure(figsize=(8, 6))

if k != 0:
    # First equation: x + ky = 1
    y1 = (1 - x) / k

    # Second equation: kx + y = -1
    y2 = -1 - k * x

    plt.plot(x, y1, label="x + ky = 1")
    plt.plot(x, y2, label="kx + y = -1")

else:
    # When k = 0:
    # x + 0y = 1  ->  x = 1
    # 0x + y = -1 ->  y = -1

    plt.axvline(1, label="x = 1")
    plt.axhline(-1, label="y = -1")

# Coordinate axes
plt.axhline(0, linewidth=0.8)
plt.axvline(0, linewidth=0.8)

plt.xlabel("x")
plt.ylabel("y")
plt.title(f"System of equations for k = {k}")

plt.xlim(-10, 10)
plt.ylim(-10, 10)

plt.grid(True)
plt.legend()
plt.tight_layout()

# Save as PDF
pdf_file = "two_lines.pdf"
plt.savefig(pdf_file, format="pdf")

# Open the PDF
subprocess.run(["xdg-open", pdf_file])

plt.show()
