import nltk
import numpy

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

def make_input(voca):
	x = []
	for i in voca:
		one_hot = [0] * len(voca)
		one_hot[i] = 1
		x.append(one_hot)

	return x

def init_hidden_layer(voca):
	w1 = []
	for i in voca:
		node = []
		for j in range(3):
			node.append(random.random())
		w1.append(node)

	return w1

def init_output_layer(voca):
	w2 = []
	for i in range(3):
		node = []
		for j in voca:
			node.append(random.random())
		w2.append(node)

	return w2

if __name__ == '__main__':
	f = open("input.txt", r)
	input = f.read()
	f.close()

	tokens = pre_processing(input)
	voca = make_voca(tokens)
	input_x = make_input(voca)
	w1 = init_hidden_layer(voca)
	w2 = init_output_layer(voca)

	
