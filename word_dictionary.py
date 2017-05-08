import nltk
import pickle


if __name__ == '__main__':

	name = input("Write file to make dictionary: ")

	f = open(name)
	text = f.read()
	f.close()

	tokens = nltk.word_tokenize(text)
	stpwrds = nltk.corpus.stopwords.words('english')
	tokens = [i for i in tokens if i not in stpwrds]

	dictionary = []
	for token in tokens:
		if token not in dictionary:
			dictionary.append(token)

	f = open('dictionary', 'wb')
	pickle.dump(dictionary, f)
	f.close()
