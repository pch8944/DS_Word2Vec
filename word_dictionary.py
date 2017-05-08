import nltk
import pickle


if __name__ == '__main__':

	name = input("Write file to make dictionary: ")

	f = open(name)
	text = f.read()
	f.close()
	print("complete: read file")

	tokens = nltk.word_tokenize(text)
	print("complete: word tokenize")

	stpwrds = nltk.corpus.stopwords.words('english')
	tokens = [i for i in tokens if i not in stpwrds]
	print("complete: eliminate stopwords")

	dictionary = []
	for token in tokens:
		if token not in dictionary:
			dictionary.append(token)
	print("complete: make dictionary")

	f = open('dictionary', 'wb')
	pickle.dump(dictionary, f)
	f.close()
	print("complete: write binary file")
