#include <iostream>
#include <math.h>
#include <vector>
#include "input.h"
#include <omp.h>


#define WORDS 100000
#define FEATURES 100
#define WINDOW 5
#define LRATE 0.025

using namespace std;
string* codeword;
struct wordfreq {
	int num = -1;
	int freq = 0;
	int nodenum = -1;
	wordfreq* dir[2] = { 0 };
}; wordfreq* root;


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

wordfreq* buildHTree(int* freq,vector<wordfreq*> &leaf) {
	cout << "Building Huffman Tree.." << endl;

	int na = WORDS;
	if (na > leaf.size())
		na = leaf.size();
	int assigned = 0;
	wordfreq* root = NULL;
	quickSort(&leaf, 0, na - 1);
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

int main() {
	int wordnum = line_of_freq();
	int *frequency = (int*)malloc(sizeof(int) * wordnum);
	call_frequency(frequency);
	codeword = new string[wordnum];

	vector<wordfreq*> leaf(wordnum);
	for (int i = 0; i < wordnum; i++) {
		leaf[i] = new wordfreq;
		leaf[i]->freq = frequency[i];
		leaf[i]->num = i;
	}
	root = buildHTree(frequency, leaf);
	getcodeword(codeword, root, "");

	float *space = (float*)malloc((FEATURES + 1)*wordnum * 2);

	float **weight01 = (float**)space;
	float **weight12 = (float**)&(space[FEATURES*wordnum]);
	float *error = (float*)&(space[FEATURES*wordnum * 2]);
	float* output = (float*)&(space[FEATURES*wordnum * 2 + wordnum]);
	
	int linenum = line_of_file();
	int* input = (int*)malloc(sizeof(int) * linenum);
	call_input(input);
	int** target = (int**)malloc(sizeof(int*) * linenum);
	for (int i = 0; i < linenum; i++) {
		target[i] = (int*)calloc(WINDOW, sizeof(int));
	}
	call_output(target);

	

	int trained = 0;
	for (int n = 0; n < 3; n++) {
		for (int i = 0; i < linenum; i++) { //training section
			for (int l = 0; l < WINDOW; l++) {
				int in = input[i];
				int t = target[i][l];
				wordfreq* cur = root;
				vector<int> actlist;
				for (int j = 0; j < wordnum; j++) {
					output[j] = 0;
					for (int k = 0; k < FEATURES; k++)
						output[j] += weight01[in][k] * weight12[j][k];
				}
				
				if (t != -1) {
					for (int j = 0; j < codeword[t].length(); j++) {
						int way = codeword[t][j] - '0';
						int p = cur->nodenum;
						if (output[p] < -6 || output[p]>6) {
							cur = cur->dir[way];
							continue;
						}
						actlist.push_back(p);
						output[p] = 1.0 / (1 + exp(-output[p]));
						error[p] = LRATE*(1 - way - output[p]);
						for (int k = 0; k < FEATURES; k++) {
							weight12[p][k] += error[p] * weight01[in][k];
						}
						
						cur = cur->dir[way];
					}
					for (int j = 0; j < FEATURES; j++) {
						float g = 0;
						for (vector<int>::iterator iter = actlist.begin(); iter != actlist.end(); iter++) {
							int n = *iter;
							g += error[n] * weight12[n][j];
						}
						weight01[in][j] += g;
					}
				}
			}
		}
	}

	return 0;
}
