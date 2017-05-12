import nltk
import pickle

if __name__ == '__main__':

	f = open('frequency.txt')
	frequency = f.readlines()
	f.close()

	i = 0
	for no in frequency:
		frequency[i] = int(no)
		i += 1
	print("complete: read frequency")

	f = open('dictionary_raw', 'rb')
	dictionary = pickle.load(f)
	f.close()
	print("complete: read dictionary")

	dic_mod = []
	i = 0
	for word in dictionary:
		if frequency[i] >= 4:
			dic_mod.append(word)
		i += 1
	print("complete: modify dictionary")

	f = open('dictionary', 'wb')
	pickle.dump(dic_mod, f)
	f.close()
	print("complete: write binary file")

	print(len(dictionary))
	print(len(dic_mod))