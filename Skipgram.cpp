#include <iostream>
#include <math.h>
#include <vector>
#include "input.h"
#include <omp.h>
#include <string>
#include <atlcoll.h>
#include <fstream>


#define WORDS 100000
#define FEATURES 100
#define WINDOW 6
#define LRATE 0.001

using namespace std;


struct node {
	char type;
	int num;
	float **inputs;
	float *output;
	float threshold;
	float dthreshold;
	float *weights;
	float *oldWeights;
	int inputnum;
	float *error;
};

struct wordfreq {
	int num = -1;
	int freq = 0;
	node *pnode = 0;
	wordfreq* dir[2] = { 0 };
};

struct out {
	int num;
	float value;
};

struct layer {
	int num;
	node *nodes;
	float* outputs;
	float **error;
};

struct network {
	int inputnum;
	int inputindex;
	float *inputs;
	float **outputs;
	int layernum;
	layer *layers;
};

string* codeword;
wordfreq* root;

float randnum() {
	return (((float)rand() * 2) / ((float)RAND_MAX + 1)) - 1;
}


network* createNet(int layers, int *nodes, int *inputs, char* types) {

	network* NN = new network;
	NN->inputnum = inputs[0];
	NN->inputs = (float*)calloc(NN->inputnum,sizeof(float));
	NN->layernum = layers;
	NN->layers = new layer[layers];
	layer *old = NULL;
	for (int i = 0; i < layers; i++) {
		layer *temp = &(NN->layers[i]);
		temp->num = nodes[i];
		temp->nodes = new node[nodes[i]];
		temp->outputs = new float[nodes[i]];
		temp->error = new float*;
		temp->error[0] = new float[nodes[i]];
		node *tempnodes = temp->nodes;
		for (int j = 0; j < nodes[i]; j++) {
			tempnodes[j].inputnum = inputs[i];
			if (!old) {
				tempnodes[j].inputs = &(NN->inputs);
			}
			else {
				tempnodes[j].inputs = &(old->outputs);
			}
			tempnodes[j].output = new float;
			tempnodes[j].error = &(temp->error[0][j]);
			*(tempnodes[j].error) = 0;
			tempnodes[j].type = types[i];
			tempnodes[j].num = j;
			temp->outputs[j] = *(tempnodes->output);
			tempnodes[j].weights = new float[inputs[i]];
			for (int k = 0; k < inputs[i]; k++) {
				tempnodes[j].weights[k] = randnum();
			}
			tempnodes[j].threshold = randnum();
		}
		old = temp;
	}
	NN->outputs = &(old->outputs);
	return NN;
}

void put_input(network* NN, float *inputs) {
	for (int i = 0; i < NN->inputnum; i++) {
		NN->inputs[i] = inputs[i];
	}
}

float fma1(float x, float y, float z)
{
	return x * y + z;
}


void get_output_1(node *Node) {
	float sum = 0;

	for (int i = 0; i < FEATURES; i++) {
		//cout << (*Node->inputs)[i]<<" ";
		//sum += (*Node->inputs)[i] * Node->weights[i];
		sum = fma1((*Node->inputs)[i], Node->weights[i], sum);
	}
	*(Node->output) = sum;
	/*//sum += -1 * Node->threshold;
	float temp;
	temp = -sum;
	*(Node->output) = 1.0 / (1 + exp(temp));
	//cout << *(Node->output) << endl;*/
	//return *(Node->output);
}

float get_output_0(node *Node,int i) {
	/*float sum = 0;


		//cout << (*Node->inputs)[i]<<" ";
		//sum += (*Node->inputs)[i] * Node->weights[i];
	sum = (*Node->inputs)[i] * Node->weights[i];


	//sum += -1 * Node->threshold;
	float temp;
	temp = -sum;*/
	*(Node->output) = Node->weights[i];
	//cout << *(Node->output) << endl;
	return *(Node->output);
}

float expsum;
void calculation(network *NN) {
#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < FEATURES; j++) {
		NN->layers[0].outputs[j] = get_output_0(&(NN->layers[0].nodes[j]),NN->inputindex);
		//cout << NN->layers[0].outputs[j];
	}
#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < NN->layers[1].num; j++) {
		get_output_1(&(NN->layers[1].nodes[j]));
	}
}

/*float derivative(node *Node, float* targets) {
float temp;
float *k = Node->output;
switch (Node->type) {
case 'g':
temp = *k * (1.0 - *k);
break;
case 's':
temp = 1;
break;
}
return temp;
}*/

void training(network *NN, float LR, int* targets) {
	/*
#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < cur->num; j++) {
		*(cur->nodes[j].error) = - *(cur->nodes[j].output);
		cur->nodes[j].threshold += (LR * *(cur->nodes[j].error) * -1);
		for (int k = 0; k < FEATURES; k++) {
			cur->nodes[j].weights[k] += (LR * *(cur->nodes[j].error) * (*cur->nodes[j].inputs)[k]);
		}
	}
	*/
	
	for (int l = 1; l < WINDOW-1; l++) {
		calculation(NN);
		layer *cur;

		cur = &NN->layers[1];
		vector<int> actlist;
		wordfreq* curnode = root;
		node* route;

		int target = targets[l];
		if (target != -1) {
			for (int i = 0; i < codeword[target].length(); i++) {
				route = curnode->pnode;
				int way = codeword[target][i] - '0';
				if (!(*(route->output) < 6 && *(route->output) > -6)) {
					curnode = curnode->dir[way];
					continue;
				}
				actlist.push_back(route->num);
				*(route->output) = 1.0 / (1 + exp(-*(route->output)));
				*(route->error) = LR*(1 - way - *(route->output));
				//cout << *(route->error);
				//route->threshold += (LR * *(route->error) * -1);
#pragma omp parallel for schedule(dynamic)
				for (int k = 0; k < FEATURES; k++) {
					route->weights[k] += (*(route->error) * (*route->inputs)[k]);
				}
				curnode = curnode->dir[way];
			}



			cur = &NN->layers[0];
			layer *next = &NN->layers[1];
#pragma omp parallel for schedule(dynamic)
			for (int j = 0; j < FEATURES; j++) {
				float temp = 0;
				/*for (int k = 0; k < next->num; k++) {
					temp = fma1(next->error[0][k],next->nodes[k].weights[j],temp);
					next->error[0][k] = 0;
				}*/

				for (vector<int>::iterator iter = actlist.begin(); iter != actlist.end(); iter++) {
					int n = *iter;
					temp = fma1(next->error[0][n], next->nodes[n].weights[j], temp);
				}

				//*(cur->nodes[j].error) = LR * *(cur->nodes[j].output) * (1 - *(cur->nodes[j].output)) * temp;

				//cur->nodes[j].threshold += (LR * *(cur->nodes[j].error) * -1);
				/*for (int k = 0; k < next->num; k++) {
					cur->nodes[j].weights[k] = fma1(*(cur->nodes[j].error), (*cur->nodes[j].inputs)[k], cur->nodes[j].weights[k]);
				}*/
				int k = NN->inputindex;
				cur->nodes[j].weights[k] += temp;
			}
		}
	}
	/*
	cur = &NN->layers[1];
	#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < WORDS; j++) {
	cur->nodes[j].threshold += (LR * *(cur->nodes[j].error) * -1);
	for (int k = 0; k < FEATURES; k++) {
	cur->nodes[j].weights[k] += (LR * *(cur->nodes[j].error) * (*cur->nodes[j].inputs)[k]);
	}
	}

	cur = &NN->layers[0];
	#pragma omp parallel for schedule(dynamic)
	for (int j = 0; j < FEATURES; j++) {
	cur->nodes[j].threshold += (LR * *(cur->nodes[j].error) * -1);
	for (int k = 0; k < WORDS; k++) {
	cur->nodes[j].weights[k] += (LR * *(cur->nodes[j].error) * (*cur->nodes[j].inputs)[k]);
	}
	}*/

}

float* output(network* NN) {
	return *(NN->outputs);
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
		do ++i; while ((*a)[i].value <= pivot.value && i <= r);
		do --j; while ((*a)[j].value > pivot.value);
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
		j = partition(a, l, r);
		if (j - l < r - j) {
			quickSort(a, l, j - 1);
			quickSort(a, j + 1, r);
		}
		else {
			quickSort(a, j + 1, r);
			quickSort(a, l, j - 1);
		}
	}

}

wordfreq* buildHTree(int* freq, network* NN, vector<wordfreq*> &leaf) {
	cout << "Building Huffman Tree.." << endl;
	
	int na = WORDS;
	if (na > leaf.size())
		na = leaf.size();
	int assigned = 0;
	wordfreq* root=NULL;
	quickSort(&leaf, 0, na - 1);
	while (na-1) {
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
		temp->pnode = &NN->layers[NN->layernum - 1].nodes[assigned];
		int p = 0;
		while (leaf[p] && leaf[p++]->freq < temp->freq);
		for (int i = 0; i < p-2; i++) {
			leaf[i] = leaf[i + 2];
		}
		leaf[p - 2] = temp;
		for (int i = p - 1; i < na - 1; i++) {
			leaf[i] = leaf[i + 1];
		}
		leaf[na - 1] = NULL;
		na--;
		assigned++;
		root = temp;
	}
	return root;
}

void getcodeword(string* code, wordfreq* node, string codenum) {
	if (node->dir[0]) {
		getcodeword(code, node->dir[0], codenum+"0");
		getcodeword(code, node->dir[1], codenum+"1");
		
	}
	else {
		code[node->num] = codenum;
	}
}

void savenet(network* NN){
	ofstream oFile("network.net");

	layer* cur = &(NN->layers[0]);
	for (int i = 0; i < cur->num; i++) {
		for (int j = 0; j < cur->nodes[i].inputnum; j++)
			oFile << cur->nodes[i].weights[j] << '\t';
	}
	cur = &(NN->layers[1]);
	for (int i = 0; i < cur->num; i++) {
		for (int j = 0; j < cur->nodes[i].inputnum; j++)
			oFile << cur->nodes[i].weights[j] << '\t';
	}
}

void loadnet(network* NN) {
	ifstream iFile("network.net");

	layer* cur = &(NN->layers[0]);
	for (int i = 0; i < cur->num; i++) {
		
	}
}

int main() {

	int* frequency;
	int no = line_of_freq();
	int wordnum = no;
	frequency = (int*)malloc(sizeof(int) * no);
	call_frequency(frequency);

	vector<wordfreq*> leaf(no);
	for (int i = 0; i < no; i++) {
		leaf[i] = new wordfreq;
		leaf[i]->freq = frequency[i];
		leaf[i]->num = i;
	}

	/*
	for (vector<wordfreq*>::iterator iter = leaf.begin(); iter != leaf.end();) {
		if ((*iter)->freq < 4) {
			delete(*iter);
			iter = leaf.erase(iter);
		}
		else
			++iter;
	}*/

	int layernum = 2;
	int neuronnum[] = { FEATURES,no };
	int inputnum[] = { no,FEATURES };
	codeword = new string[no];

	char types[] = { 'g','s' };
	network* net = createNet(2, neuronnum, inputnum, types); //model initialization
	float* input = (float*)calloc(no, sizeof(float)); //input vector
	//float* target = (float*)calloc(WORDS, sizeof(float)); //training target vector
	int* inputindex;
	char inputstr[30];
	

	root = buildHTree(frequency, net, leaf);
	getcodeword(codeword, root, "");


	no = line_of_file();
	inputindex = (int*)malloc(sizeof(int) * no);
	call_input(inputindex);
	int** targetindex;

	no = line_of_file();
	targetindex = (int**)malloc(sizeof(int*) * no);
	for (int i = 0; i < no; i++) {
		targetindex[i] = (int*)calloc(WINDOW, sizeof(int));
	}
	call_output(targetindex);
	int trained = 0;
	for (int n = 0; n < 3; n++) {
		for (int i = 0; i < no; i++) { //training section
			/*if (inputindex[i] > wordnum || targetindex[i][0] == -1)
				continue;*/
			//get_word(inputindex[i], inputstr);
			//cout << inputstr << " ";
			net->inputs[inputindex[i]] = 1;
			net->inputindex = inputindex[i];
			//put_input(net, input);
			
			training(net, 0.025, targetindex[i]);
			//cout << endl;

			net->inputs[inputindex[i]] = 0;
			if (!(i % 1000))
				cout << (double)i / no << endl;
		}
	}
	savenet(net);
	
	while (true) {
		cin >> inputstr;
		int inputind = get_index(inputstr);
		get_word(inputind, inputstr);
		cout << "Input : " << inputstr << "[" << inputind << "]" << endl;
		net->inputs[inputind] = 1;
		net->inputindex = inputind;
		calculation(net);
		net->inputs[inputind] = 0;
		vector<out> result;
		for (int i = 0; i < wordnum; i++) {
			wordfreq* cur = root;
			float prob = 1;
			for (int j = 0; j < codeword[i].length(); j++) {
				int way = codeword[i][j] - '0';
				node* curnode = cur->pnode;
				float sig = 1.0 / (1.0 + exp(-*(curnode->output)));
				prob *= (sig + way - 2 * sig * way);
				if (prob > 1)
					prob = 0;
				cur = cur->dir[way];
			}
			out* temp = new out;
			temp->num = i;
			temp->value = prob;
			result.push_back(*temp);
		}
		quickSort(&result, 0, wordnum);
		for (int i = 0; i < 20; i++) {
			get_word(result[wordnum - i].num, inputstr);
			cout << inputstr << " : " << result[wordnum - i].value << endl;
		}
	}
	
	getchar();
	return 0;
}
