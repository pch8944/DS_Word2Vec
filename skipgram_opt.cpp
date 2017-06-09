#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <math.h>
#include <vector>
#include "input.h"
#include <omp.h>
#include <string>
#include <atlcoll.h>
#include <fstream>
#include <atlstr.h>
#include <thread>
#include <algorithm>

#define WORDS 100000
#define FEATURES 300
#define WINDOW 7
#define LRATE 0.025

using namespace std;
string* codeword;
struct wordfreq {
	int num = -1;
	int freq = 0;
	int nodenum = -1;
	wordfreq* dir[2] = { 0 };
}; wordfreq* root;
struct out {
	int num;
	float value;
};

bool operator <(const out &a, const out &b) {
	return a.value < b.value;
}

bool cmp(const wordfreq* a, const wordfreq* b) {
	return a->freq < b->freq;
}

float randnum() {
	return (((float)rand() * 2) / ((float)RAND_MAX + 1)) - 1;
}

int partition(vector<wordfreq*> *a, int l, int r) {
	wordfreq* pivot, *t;
	int i, j;
	pivot = (*a)[l];
	i = l; j = r + 1;

	while (1)
	{
		do ++i; while (i <= r && (*a)[i]->freq <= pivot->freq);
		do --j; while ((*a)[j]->freq > pivot->freq);
		if (i >= j) break;
		t = (*a)[i]; (*a)[i] = (*a)[j]; (*a)[j] = t;
	}
	t = (*a)[l]; (*a)[l] = (*a)[j]; (*a)[j] = t;
	return j;
}

void quickSort(vector<wordfreq*> *a, int l, int r)
{
	int j;

	if (l < r)
	{
		// divide and conquer
		j = partition(a, l, r);
		quickSort(a, l, j - 1);
		quickSort(a, j + 1, r);
	}

}
int partition(vector<out> *a, int l, int r) {
	out pivot, t;
	int i, j;
	pivot = (*a)[l];
	i = l; j = r + 1;

	while (1)
	{
		do ++i; while (i<a->size() && (*a)[i].value <= pivot.value);
		do --j; while (j>0 && (*a)[j].value > pivot.value);
		if (i >= j) break;
		t = (*a)[i]; (*a)[i] = (*a)[j]; (*a)[j] = t;
	}
	t = (*a)[l]; (*a)[l] = (*a)[j]; (*a)[j] = t;
	return j;
}

void quickSort(vector<out> *a, int l, int r)
{
	int j;

	if (l < r)
	{
		// divide and conquer
		/*j = partition(a, l, r);
		if (j - l < r - j) {
			quickSort(a, l, j - 1);
			quickSort(a, j + 1, r);
		}
		else {
			quickSort(a, j + 1, r);
			quickSort(a, l, j - 1);
		}*/
		j = partition(a, l, r);
		quickSort(a, l, j - 1);
		quickSort(a, j + 1, r);
	}

}

wordfreq* buildHTree(int* freq, vector<wordfreq*> &leaf) {
	cout << "Building Huffman Tree.." << endl;

	int na = WORDS;
	if (na > leaf.size())
		na = leaf.size();
	int assigned = 0;
	wordfreq* root = NULL;
	sort(leaf.begin(), leaf.end(), cmp);
	while (na - 1) {
		wordfreq *l1, *l2;
		int l1i, l2i;
		l1 = leaf[0];
		l1i = 0;
		l2 = leaf[1];
		l2i = 1;
		wordfreq *temp = new wordfreq;
		temp->freq = l1->freq + l2->freq;
		temp->dir[0] = l1;
		temp->dir[1] = l2;
		temp->nodenum = assigned++;
		int p = 0;
		while (leaf[p] && leaf[p++]->freq < temp->freq);
		for (int i = 0; i < p - 2; i++) {
			leaf[i] = leaf[i + 2];
		}
		leaf[p - 2] = temp;
		for (int i = p - 1; i < na - 1; i++) {
			leaf[i] = leaf[i + 1];
		}
		leaf[na - 1] = NULL;
		na--;
		root = temp;
	}
	return root;
}

void getcodeword(string* code, wordfreq* node, string codenum) {
	if (node->dir[0]) {
		getcodeword(code, node->dir[0], codenum + "0");
		getcodeword(code, node->dir[1], codenum + "1");

	}
	else {
		code[node->num] = codenum;
	}
}
float sum1;
float sum2;
float calcos(wordfreq* node,float val1, float val2,float* p, float* q) {
	int k = node->nodenum;
	if (val1 == 0 && val2 == 0)
		return 0;
	if (node->dir[0])
		return calcos(node->dir[0], val1 * p[k], val2 * q[k], p, q) + calcos(node->dir[1], val1 * (1 - p[k]), val2 * (1 - q[k]), p, q);
	else {
		sum1 += val1*val1;
		sum2 += val2*val2;
		return val1*val2;
	}
}


int main() {
	CAtlMap<CString, pair<int,int>> wordset;
	FILE *filein;
	filein = fopen("text8", "rb");
	vector<int> inputlist;
	vector<int> freqlist;
	CString word;
	while (!feof(filein)) {
		char c = fgetc(filein);
		if (c == ' ') {
			if (word.GetLength()) {
				if (wordset.Lookup(word))
					wordset.Lookup(word)->m_value.second++;
				else
					wordset.SetAt(word, pair<int,int>(0,1));
				word.Empty();
			}
			continue;
		}
		else {
			word.AppendChar(c);
		}
	}
	fclose(filein);
	
	POSITION pos = wordset.GetStartPosition();
	CAtlMap<CString, pair<int,int>>::CPair *pPair;
	int indc = 0;
	while (pos) {
		pPair = wordset.GetNext(pos);
		if (pPair->m_value.second < 5)
			wordset.RemoveKey(pPair->m_key);
		else {
			pPair->m_value.first = indc++;
			freqlist.push_back(pPair->m_value.second);
		}
	}
	inputlist.push_back(-1);
	inputlist.push_back(-1);
	inputlist.push_back(-1);
	filein = fopen("text8", "rb");
	while (!feof(filein)) {
		char c = fgetc(filein);
		if (c == ' ') {
			if (wordset.Lookup(word)) {
				inputlist.push_back(wordset.Lookup(word)->m_value.first);
			}
			else {
				inputlist.push_back(-1);
			}
			word.Empty();
			continue;
		}
		else {
			word.AppendChar(c);
		}
	}
	fclose(filein);
	inputlist.push_back(-1);
	inputlist.push_back(-1);
	inputlist.push_back(-1);
	cout << inputlist[4];
	int wordnum = indc - 1;
	int *frequency = &freqlist[0];
	codeword = new string[wordnum];

	vector<wordfreq*> leaf(wordnum);
	for (int i = 0; i < wordnum; i++) {
		leaf[i] = new wordfreq;
		leaf[i]->freq = frequency[i];
		leaf[i]->num = i;
	}
	root = buildHTree(frequency, leaf);
	getcodeword(codeword, root, "");

	float *space = (float*)calloc(FEATURES *wordnum * 2 + wordnum + FEATURES, sizeof(float));

	float *weight01 = (float*)space;
	float *weight12 = (float*)&(space[FEATURES*wordnum]);
	float* error01 = (float*)&(space[FEATURES*wordnum * 2]);
	float* output = (float*)&(space[FEATURES*wordnum * 2 + FEATURES]);



	int d = FEATURES;

	for (int i = 0; i < FEATURES*wordnum; i++) {
		space[i] = randnum();
	}
	/*
	ifstream iFile;
	iFile.open("network.net");
	char temp[20];
	for (int i = 0; i < FEATURES*wordnum * 2; i++) {
		iFile.getline(temp, 19, '\t');
		space[i] = atof(temp);
	}*/
	

	int trained = 0;
	
	for (int n = 0; n < 5; n++) {
		for (int i = 0; i < inputlist.size(); i++) { //training section
			int in = inputlist[i];
			if (in == -1)
				continue;
			if (i % 100000 == 0)
				cout << (double)i / inputlist.size() << endl;
			for (int l = 0; l < WINDOW; l++) {
				if (l == 3)
					continue;
				int t = inputlist[i - 3 + l];
				wordfreq* cur = root;
				if (t != -1) {
					for (int j = 0; j < codeword[t].length(); j++) {
						int way = codeword[t][j] - '0';
						int p = cur->nodenum;
						output[p] = 0;
						for (int k = 0; k < FEATURES; k++) {
							output[p] += weight01[in*d + k] * weight12[p*d + k];
							error01[k] = 0;
						}
						if (output[p] < -6 || output[p]>6) {
							cur = cur->dir[way];
							continue;
						}
						output[p] = 1.0 / (1 + exp(-output[p]));
						float error12 = LRATE*(1 - way - output[p]);
						for (int k = 0; k < FEATURES; k++) {
							error01[k] += error12 * weight12[p*d + k];
							weight12[p*d + k] += error12 * weight01[in*d + k];
						}

						cur = cur->dir[way];
					}
					for (int j = 0; j < FEATURES; j++) {
						weight01[in*d+j] += error01[j];
					}
				}
			}
		}
	}
	
	ofstream oFile("vec.vec");
	for (int i = 0; i < wordnum; i++) {
	for (int j = 0; j < FEATURES; j++)
	oFile << space[i*d + j] << ' ';
	oFile << "\n";
	}
	oFile.close();

	char inputstr[50];
	CString k;

	
	
	FILE *in;
	in = fopen("out.vec", "rb");

	float* out1 = new float[wordnum];
	float* out2 = new float[wordnum];
	for (int k = 0; k < 4 * wordnum; k++) {

	}
	
	
	while (true) {
		vector<out> cos;
		cout << "enter a word" << endl;
		cin >> inputstr;
		k = inputstr;
		int inputind = 0;
		if (wordset.Lookup(k)) {
			inputind = wordset.Lookup(k)->m_value.first;
		}
		else
			continue;
		cout << "Input : " << inputstr << "[" << inputind << "]" << endl;

/*
#pragma omp parallel for
		for (int i = 0; i < wordnum; i++) {
			out1[i] = 0;
			for (int n = 0; n < FEATURES; n++) {
				out1[i] += weight01[inputind*d + n] * weight12[i*d + n];
			}
			if (out1[i] < -6)
				out1[i] = 0;
			else if (out1[i] > 6)
				out1[i] = 1;
			else
				out1[i] = 1.0 / (1 + exp(-out1[i]));
		}
		for (int i = 0; i < wordnum; i++) {
			wordfreq* cur = root;
			float prob = 1;
			for (int j = 0; j < codeword[i].length(); j++) {
				int way = codeword[i][j] - '0';
				int p = cur->nodenum;
				prob *= (way + out1[p] - 2*way*out1[p]);
				cur = cur->dir[way];
			}
			out temp;
			temp.num = i;
			temp.value = prob;
			cos.push_back(temp);
		}
		quickSort(&cos, 0, wordnum);
		for (int i = 0; i < 20; i++) {
			char outstr[100];
			cout << get_word(cos[wordnum - i - 1].num, outstr) << " : " << cos[wordnum - i - 1].value << endl;
		}*/

		/*
#pragma omp parallel for
		for (int j = 0; j < wordnum; j++) {
			out1[j] = 0;
			for (int n = 0; n < FEATURES; n++)
				out1[j] += weight01[inputind*d + n] * weight12[j*d + n];
			if (out1[j] < -6)
				out1[j] = 0;
			else if (out1[j] > 6)
				out1[j] = 1;
			else
				out1[j] = 1.0 / (1 + exp(-out1[j]));
		}
		for (int k = 0; k < wordnum; k++) {
			sum1 = 0;
			sum2 = 0;
			if (k % 1000 == 0)
				cout << (float)k / wordnum << endl;
#pragma omp parallel for
			for (int j = 0; j < wordnum; j++) {
				out2[j] = 0;
				for (int n = 0; n < FEATURES; n++)
					out2[j] += weight01[k*d + n] * weight12[j*d + n];
				if (out2[j] < -6)
					out2[j] = 0;
				else if (out2[j] > 6)
					out2[j] = 1;
				else
					out2[j] = 1.0 / (1 + exp(-out2[j]));
			}
			float cosine = calcos(root, 1,1, out1, out2);
			
			out temp;
			temp.num = k;
			temp.value = cosine/sqrt(sum1*sum2);
			cos.push_back(temp);
			if (k == inputind)
				cout << temp.value;
		}
		sort(cos.begin(), cos.end());
		for (int i = 0; i < 20; i++) {
			char outstr[100];
			cout << get_word(cos[cos.size()-i-1].num, outstr) << " : " << cos[cos.size() - i - 1].value << endl;
		}
		for (int i = 0; i < cos.size(); i++)
			if (cos[i].value == 1)
				cout << i;*/
		
		for (int i = 0; i < wordnum; i++) {
			float cosine = 0;
			float l1 = 0, l2 = 0;
			for (int n = 0; n < FEATURES; n++) {
				cosine += weight01[inputind*d + n] * weight01[i*d + n];
				l1 += weight01[inputind*d + n] * weight01[inputind*d + n];
				l2 += weight01[i*d + n] * weight01[i*d + n];
			}
			out temp;
			temp.num = i;
			temp.value = cosine / sqrt(l1*l2);
			cos.push_back(temp);
		}
		sort(cos.begin(), cos.end());
		for (int i = 0; i < 20; i++) {
			int ind = cos[cos.size() - i - 1].num;
			pos = wordset.GetStartPosition();
			for (int j = 0; j < ind; j++) {
				wordset.GetNext(pos);
			}
			char outstr[100];
			cout << wordset.GetKeyAt(pos) << " : " << cos[cos.size() - i - 1].value << endl;
		}
	}
	
	
	
	return 0;
}
