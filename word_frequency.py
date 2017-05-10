import pickle
import nltk

if __name__ == '__main__':

	name = input("Write training data file: ")

	f = open('dictionary', 'rb')
	dictionary = pickle.load(f)
	f.close()
	print("complete: read dictionary")

	length = len(dictionary)

	f = open(name)
	text = f.read()
	f.close()
	print("complete: read training data file")

	tokens = nltk.word_tokenize(text)
	print("complete: word tokenize")

	stpwrds = nltk.corpus.stopwords.words('english')
	tokens = [i for i in tokens if i not in stpwrds]
	print("complete: eliminate stopwords")

	check_list = [0] * length

	for token in tokens:
		try:
			check_list[dictionary.index(token)] += 1
		except:
			continue
	print("complete: analyze word frequency")

	f = open('frequency.txt', 'w')

	for frequency in check_list:
		result = str(frequency)
		result += '\n'
		f.write(result)

	print("complete: write frequency file")
