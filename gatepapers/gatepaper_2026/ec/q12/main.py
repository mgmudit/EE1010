#Code by Mudit
#Date: 29/10/20


import numpy as np
import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.colors import LightSource
import subprocess
import os

# Surface: 2x² - y² - z² = 0
M = np.diag([2, -1, -1])

# Given point
P = np.array([1.0, 0.0, np.sqrt(2)])

# Calculate normal and unit normal
N = M @ P
n = N / np.linalg.norm(N)

print("Matrix M:\n", M)
print("Normal vector:", N)
print("Unit normal:", n)
print("Correct answer: Option D")

# Create cone coordinates
r = np.linspace(0.03, 2.5, 180)
theta = np.linspace(0, 2*np.pi, 240)
R, T = np.meshgrid(r, theta)

# Two halves of the cone
X1 = R
X2 = -R
Y = np.sqrt(2) * R * np.cos(T)
Z = np.sqrt(2) * R * np.sin(T)

# Smooth lighting to emphasize curvature
light = LightSource(azdeg=315, altdeg=40)

# Create figure
fig = plt.figure(figsize=(11, 9))
ax = fig.add_subplot(111, projection="3d")

# Shade each half using its surface gradients
for X, base_color in [(X1, "#54A8D8"), (X2, "#E9A15A")]:
    facecolors = light.shade(
        Z,
        cmap=plt.get_cmap("viridis"),
        vert_exag=0.8,
        blend_mode="soft"
    )

    ax.plot_surface(
        X, Y, Z,
        facecolors=facecolors,
        rstride=1,
        cstride=1,
        linewidth=0,
        antialiased=True,
        shade=True,
        alpha=0.96
    )

# Draw point P
ax.scatter(
    *P, color="black", s=65,
    depthshade=True, label="Point P"
)

# Tangent plane at P: z = sqrt(2)*x
xp = np.linspace(0.5, 1.5, 20)
yp = np.linspace(-0.7, 0.7, 20)
XP, YP = np.meshgrid(xp, yp)
ZP = np.sqrt(2) * XP

ax.plot_surface(
    XP, YP, ZP,
    color="limegreen",
    alpha=0.28,
    edgecolor="none"
)

# Normal arrow
ax.quiver(
    P[0], P[1], P[2],
    n[0], n[1], n[2],
    color="red",
    length=1.8,
    normalize=True,
    linewidth=3.5,
    arrow_length_ratio=0.2
)

ax.text(1, 0, np.sqrt(2) + 0.2, "P")
ax.text(1.7, 0, 0.5, "Unit normal", color="red")

# Labels and viewing angle
ax.set_xlabel("X axis (cone axis)")
ax.set_ylabel("Y axis")
ax.set_zlabel("Z axis")
ax.set_title("Shaded Double Cone with Tangent Plane and Normal")
ax.view_init(elev=23, azim=35)
ax.set_box_aspect((1, 1, 1))
ax.legend()

# Save image
filename = os.path.abspath("normal_plot.png")
plt.savefig(filename, dpi=220, bbox_inches="tight")
plt.close()

print("\nSaved:", filename)

# Automatically open image in Android
try:
    result = subprocess.run(["termux-open", filename], check=False)
    if result.returncode != 0:
        print("Image saved; automatic opening failed.")
except FileNotFoundError:
    print("termux-open unavailable; image is saved.")

