import ctypes
import matplotlib.pyplot as plt
from tqdm import tqdm

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
def test_envy_freeness(num_players=3, num_samples=1, max_items=11):
    ef_lib = ctypes.CDLL('main.so')
    ef_lib.percent_ef_test.argtypes = (ctypes.c_int, ctypes.c_int, ctypes.c_int)
    ef_lib.percent_ef_test.restype = ctypes.c_float

    num_items = [i+1 for i in range(max_items)]
    ef_values = [ef_lib.percent_ef_test(num_samples, num_players, n) for n in num_items]

    for i in range(max_items):
        print(num_items[i], ef_values[i])
    # plot the data
    fig = plt.figure()
    ax = fig.add_subplot(111)

    plt.plot(num_items, ef_values, 'bo')

    for xy in zip(num_items, ef_values):
        ax.annotate(' (%s, %s) ' % (xy[0], str(xy[1])[:5]), xy=xy, textcoords='data')


    # plt.title('Proportion Envy Free vs. Number of Items (' + str(num_samples) + ' samples)')
    plt.title('Fractional Envy vs. Number of Items')
    plt.xlabel('Number of Items')
    # plt.ylabel('Proportion Envy Free')
    plt.ylabel('Fractional Envy')

    plt.axis([-0.5, max_items+1, -0.1, 10])
    plt.show()

    # trial_name = "ef-" + str(num_players) + "-" + str(num_samples) + "-" + str(max_items) + ".png"
    trial_name = "fractional-envy" + str(num_players) + "-" + str(num_samples) + "-" + str(max_items) + ".png"
    fig.savefig(trial_name)

def test_ef1_all(num_players=3, num_samples=1, max_items=11):
    ef_lib = ctypes.CDLL('main.so')
    ef_lib.percent_ef1_all_test.argtypes = (ctypes.c_int, ctypes.c_int, ctypes.c_int)
    ef_lib.percent_ef1_all_test.restype = ctypes.c_float

    num_items = [i+1 for i in range(max_items)]
    ef_values = [ef_lib.percent_ef1_all_test(num_samples, num_players, n) for n in num_items]

    for i in range(max_items):
        print(num_items[i], ef_values[i])
    # plot the data
    fig = plt.figure()
    ax = fig.add_subplot(111)
    plt.plot(num_items, ef_values, 'bo')
    
    for xy in zip(num_items, ef_values):
        ax.annotate(' (%s, %s) ' % (xy[0], str(xy[1])[:5]), xy=xy, textcoords='data')
    
    plt.axis([-0.5, max_items+1, -0.1, 1.1])
    plt.title('Proportion EF-1-ALL vs. Number of Items (' + str(num_samples) + ' samples)')
    plt.xlabel('Number of Items')
    plt.ylabel('Proportion Envy Free')
    
    trial_name = "ef1-all-" + str(num_players) + "-" + str(num_samples) + "-" + str(max_items) + ".png"
    fig.savefig(trial_name)
    

def main():
    # example()
    test_envy_freeness(3, 10000, 10)
    # test_ef1_all(3, 10000, 10)

    return
main()
