#Code by Mudit
#Date: 28/09/2026
import numpy as np
import matplotlib.pyplot as plt
import subprocess

# Create a grid for x1 and x3
x1 = np.linspace(-5, 5, 100)
x3 = np.linspace(-5, 5, 100)

X1, X3 = np.meshgrid(x1, x3)

# Plane 1:
# x1 + x2 + x3
X2_plane1 = -X1 - X3

# Plane 2:
# x1 + 2x3 = 0
x2 = np.linspace(-5, 5, 100)
X2, X3_plane2 = np.meshgrid(x2, x3)

# From x1 + 2x3 = 0:
# x1 = -2x3
X1_plane2 = -2 * X3_plane2

# Create 3D figure
fig = plt.figure(figsize=(10, 8))
ax = fig.add_subplot(111, projection='3d')

# Plot Plane 1
ax.plot_surface(
    X1, X2_plane1, X3,
    alpha=0.5
)

# Plot Plane 2
ax.plot_surface(
    X1_plane2, X2, X3_plane2,
    alpha=0.5
)

# Intersection line:
# (x1, x2, x3) = x3(-2, 1, 1)
t = np.linspace(-5, 5, 200)

x1_line = -2 * t
x2_line = t
x3_line = t

ax.plot(
    x1_line,
    x2_line,
    x3_line,
    linewidth=4,
    label='Line of intersection'
)

# Mark origin
ax.scatter(0, 0, 0, s=50)

# Labels
ax.set_xlabel('$x_1$')
ax.set_ylabel('$x_2$')
ax.set_zlabel('$x_3$')

ax.set_title(
    'Intersection of Two Planes\n'
    '$x_1+x_2+x_3=0$ and $x_1+2x_3=0$'
)

ax.legend()

# Save the figure
filename = 'two_planes_intersection.pdf'
plt.savefig(filename, dpi=200, bbox_inches='tight')

# Close the figure
plt.close()

# Open the pdf
subprocess.run(['termux-open', filename])
