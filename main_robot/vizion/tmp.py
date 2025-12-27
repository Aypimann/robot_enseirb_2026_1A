l = [
    [-0.37712497,-0.29012523, 0.63398301],
    [-0.4765805 ,-0.29567763, 0.62516433],
    [-0.28996481,-0.18360556, 0.65059718],
    [-0.19050928,-0.17805316, 0.65941587]
]

d = lambda a, b: (sum([(a[i] - b[i])**2 for i in range(3)]))**0.5

scale = 1

rat = {}

for i in range(140):
    scale *= 1.01
    l2 = [[e*scale for e in ll] for ll in l]
    rat[scale] = d(l2[1],l2[2]) / d(l2[1],l2[0])

print(rat)

print(min(rat, key = lambda s: abs(1 - rat[s])))

print("dist :")
scale = 1.0615
l2 = [[e*scale for e in ll] for ll in l]
for i in range(4):
    print(d(l2[i], l2[(i+1)%4]))



print("\n\n\n")
l = [[ 137.,1249.],
 [ 410.,1274.],
 [ 387.,1543.],
 [ 117.,1522.]]

d = lambda a, b: (sum([(a[i] - b[i])**2 for i in range(2)]))**0.5

for i in range(4):
    print(d(l[i], l[(i+1)%4]))

