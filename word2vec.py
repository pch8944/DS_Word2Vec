import nltk

def input_sampling():
	pass

def pre_processing(text):
	text = text.lower()
	tokens = nltk.word_tokenize(text)
	stpwrds=nltk.corpus.stopwords.words('english')
	tokens = [i for i in tokens if i not in stpwrds]
	ptm = [',', '.', '?', ':', '\'\'', '-', '--', '...', '(', ')', "'", '``', '!', ';', '[', ']']
	tokens = [i for i in tokens if i not in ptm]
	porter_stemmer=nltk.stem.porter.PorterStemmer()
	i = 0
	for token in tokens:
		tokens[i] = porter_stemmer.stem(token)
		i += 1

	return tokens

def make_voca(tokens):
	voca = []
	for token in tokens:
		if token not in voca:
			voca.append(token)
	voca.sort()

	return voca

def make_pair(tokens):

	pass

if __name__ == '__main__':
	f = open("input.txt", r)
	input = f.read()
	f.close()

	tokens = pre_processing(input)
	voca = make_voca(tokens)

