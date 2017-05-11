import sys
import pickle

if __name__ == '__main__':
	var = sys.argv[1]

	f = open('D://dictionary', 'rb')
	dictionary = pickle.load(f)
	f.close()

	try:
		number = dictionary.index(var)
	except:
		number = -1

	print(number)