import os
import numpy

def make_vector(index_list, wv, directory):
	f = open(directory+"/mcc.dat")
	lines = f.readlines()
	f.close()
	print("complete: read "+ directory)

	word_list = []
	tf_idf = []
	for line in lines:
		point = line.find(":")
		point2 = line.find("\n")
		word = line[:point]
		value = line[point+1:point2]
		try:
			tf_idf.append(float(value))
			word_list.append(word)
		except:
			continue
	#print("complete: make tf-idf vector")
	
	wv_list = []
	del_list = []
	for word in word_list:
		try:
			n = index_list.index(word)
			wv_list.append(wv[n])
		except:
			del_list.append(word)

	# delete word not in word2vec
	for word in del_list:
		n = word_list.index(word)
		del word_list[n]
		del tf_idf[n]

	vec_tf_idf = numpy.array(tf_idf)
	vec_wv = numpy.array(wv_list)
	#print("complete: make word vector")


	tf_idf_wv = numpy.dot(vec_tf_idf, vec_wv)
	#print("complete: make tf-idf-wv vector")

	f = open(directory+'/tfidfwv.dat', 'w')
	i = 0
	for n in tf_idf_wv:
		f.write('w'+str(i)+':'+str(n)+'\n')
		i += 1
	f.close()
	print("complete: write vector file for "+directory)

	return

def path_find(index_list, wv, directory):
	directories = os.listdir(directory)
	for name in directories:
		if name == "mcc.dat":
			make_vector(index_list, wv, directory)
		elif name == "tfidfwv.dat":
			continue
		else:
			path_find(index_list, wv, directory+"/"+name)

	return

if __name__ == '__main__':

	directory = input("Insert top folder directory: ")

	f = open('vec.txt')
	lines = f.readlines()
	f.close()

	index_list = []
	wv = []
	for line in lines:
		point = line.find(" ")
		word = line[:point]
		weight = line[point+1:].split()
		i = 0
		for n in weight:
			weight[i] = float(n)
			i += 1
		index_list.append(word)
		wv.append(weight)
	del index_list[0]
	del wv[0]
	print("complete: read word2vec output")

	path_find(index_list, wv, directory)