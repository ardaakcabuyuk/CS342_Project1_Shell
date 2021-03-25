import matplotlib.pyplot as plt
import numpy as np

plt.xlabel("M")
plt.ylabel("Time (Microseconds)")
plt.title("Elapsed time vs M value (where N is fixed at 1)")
M = np.array([1, 4, 16, 64, 256, 1024, 4096])
N1 = np.array([7441252, 7430443, 11401421, 16143623, 13901704, 15929865, 20728642])
N10 = np.array([6307669, 11401421, 14936194, 17551575, 28255916, 17364924, 16137583])

plt.plot(M, N1, label="Normal mode")
plt.plot(M, N10, label="Tapped mode")
plt.xticks(M)
#y = np.array([965, 1355, 1668, 1768, 1886, 2268, 3993, 4739, 6821])
#plt.yticks(y)
plt.yscale('log')
plt.xscale('log')

plt.legend()
plt.show()
