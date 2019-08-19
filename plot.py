import matplotlib.pyplot as plt


x = []
y = []

with open('boundary.txt', 'r') as f:
    for line in f:
        nums = line.split(' ')
        x.append(float(nums[0]))
        y.append(float(nums[1]))

# append first point to end to make closed loop
x.append(x[0])
y.append(y[0])

plt.plot(x,y)


x = []
y = []

with open('points.txt', 'r') as f:
    for line in f:
        nums = line.split(' ')
        x.append(float(nums[0]))
        y.append(float(nums[1]))
plt.plot(x,y)

plt.gca().set_aspect('equal')
plt.axis('off')

plt.show()