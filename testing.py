import ctypes
import matplotlib.pyplot as plt

def example():
    plt.figure()
    plt.plot([1, 2, 3, 4, 5, 6, 7, 8, 9], [0, 0, 0.222180, 0.238680, 0.324010, 0.629210, 0.723180, 0.785990, 0.879370], 'bo')
    plt.axis([0, 10, 0, 10])
    plt.title('Proportion Envy Free vs. Number of Items (100,000 samples)')
    plt.xlabel('Number of Items')
    plt.ylabel('Proportion Envy Free')
    plt.show()


# Test envy freeness for n items
#
def test_envy_freeness(n):
	lst1 = [(i+1) for i in range(n)]
	lst2 = [(i+1)*(i+1) for i in range(n)]

	dummy = 0
	for i in range(n):
		# use i + 1
		# call c function percent_ef_test
		# add value to lst2
		dummy += 1

	plt.figure()
	plt.plot(lst1, lst2, 'bo')
	plt.axis([0, n+1, 0, n*n+1])
	plt.show()

def main():
    # test_envy_freeness(10)
    example()
    return

main()