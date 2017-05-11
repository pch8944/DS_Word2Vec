import sys
import pickle

if __name__ == '__main__':
	var = sys.argv[1]

	f = open('D://dictionary', 'rb')
	dictionary = pickle.load(f)
	f.close()

	var = int(var)

	try:
		word = dictionary[var]
	except:
		word = -1

	print(word)