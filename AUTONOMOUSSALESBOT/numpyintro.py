import numpy as np

cgnisthegoat = np.array([[1, 2], [3, 4], [5, 6], [7, 8]])
print(cgnisthegoat)
cgnisthegoat += 1
print(cgnisthegoat)
whothehelliscgn = np.array([[1, 2, 3, 4], [5, 6, 7, 8]])

# we are using @ operator for matrix multiplication
print(cgnisthegoat @ whothehelliscgn)
print(whothehelliscgn @ cgnisthegoat)

# we are writing shape after the name of the array for outputting the size
print(cgnisthegoat.shape)
print(whothehelliscgn.shape)

print(np.sum(cgnisthegoat))
print(np.mean(whothehelliscgn))

print(np.mean(cgnisthegoat, axis=(0, 1)))
print("")
