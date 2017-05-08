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

	f = open('input.txt', 'w')
	t_length = len(tokens)

	i = 0
	for token in tokens:
		summary = ""
		try:
			position = dictionary.index(token)
			position = str(position)
			summary = summary + position
		except:
			continue
		for j in range(3):
			if (i - j - 1) >= 0 :
				try:
					out = dictionary.index(tokens[i-j-1])
					out = str(out)
					summary = summary + ' ' + out
				except:
					continue
			if (i + j + 1) < t_length:
				try:
					out = dictionary.index(tokens[i+j+1])
					out = str(out)
					summary = summary + ' ' + out
				except:
					continue
		summary += '\n'
		f.write(summary)
		i += 1

	f.close()
	print("complete: write input file")


