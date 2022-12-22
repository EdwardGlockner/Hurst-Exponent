from matplotlib import pyplot as plt

file = open("hurst.txt", "r")
lines = file.readlines()
file.close()
lag_vals = []
hurst_vals = []

for line in lines:
    temp_line = line.split(", ")
    lag_vals.append(float(temp_line[0]))
    hurst_vals.append(float(temp_line[1]))


plt.figure()
plt.plot(lag_vals[75:], hurst_vals[75:])
plt.show()

