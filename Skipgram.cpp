#include <iostream>
#include <math.h>
#include <vector>
#include "input.h"
#include <omp.h>

#define WORDS 100000
#define FEATURES 300
#define WINDOW 5
#define LRATE 0.001

using namespace std;

struct wordfreq {
	int num = -1;
	int freq=0;
	node* node=0;
	wordfreq* left = 0;
	wordfreq* right = 0;
};


struct node {
	char type;
	char num;
	float **inputs;
	float *output;
	float threshold;
	float dthreshold;
	float *weights;
	float *oldWeights;
	int inputnum;
	float *error;
};

struct layer {
	int num;
	node *nodes;
	float* outputs;
};

struct network {
	int inputnum;
	float *inputs;
	float **outputs;
	int layernum;
	layer *layers;
};

float randnum() {
	return (((float)rand() * 2) / ((float)RAND_MAX + 1)) - 1;
}

network* createNet(int layers, int *nodes, int *inputs, char* types) {

	network* NN = new network;
	NN->inputnum = inputs[0];
	NN->inputs = new float[NN->inputnum];
	NN->layernum = layers;
	NN->layers = new layer[layers];
	layer *old = NULL;
	for (int i = 0; i < layers; i++) {
		layer *temp = &(NN->layers[i]);
		temp->num = nodes[i];
		temp->nodes = new node[nodes[i]];
		temp->outputs = new float[nodes[i]];
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
			tempnodes[j].error = new float;
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


float get_output(node *Node) {
	float sum = 0;

	for (int i = 0; i < Node->inputnum; i++) {
		//cout << (*Node->inputs)[i]<<" ";
		//sum += (*Node->inputs)[i] * Node->weights[i];
		sum = fma1((*Node->inputs)[i], Node->weights[i], sum);
	}

	sum += -1 * Node->threshold;
	float temp;
	switch (Node->type) {
	case 'g':
		temp = -sum;
		*(Node->output) = 1.0 / (1 + exp(temp));
		break;
	case 's':
		if (sum > 5) *(Node->output) = exp(5);
		else if (sum < -5) *(Node->output) = 0;
		else *(Node->output) = exp(sum);
		break;
	}
	//cout << *(Node->output) << endl;
	return *(Node->output);
}
float expsum;
void calculation(network *NN) {
	for (int i = 0; i < NN->layernum; i++) {
		#pragma omp parallel for schedule(dynamic)
		for (int j = 0; j < NN->layers[i].num; j++) {
			NN->layers[i].outputs[j] = get_output(&(NN->layers[i].nodes[j]));
		}
	}
}

float derivative(node *Node, float* targets) {
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
}

void training(network *NN, float LR, float* targets) {
	layer *cur;
	for (int i = NN->layernum - 1; i >= 0; i--) {
		cur = &NN->layers[i];
		if (i == NN->layernum - 1) {
			#pragma omp parallel for schedule(dynamic)
			for (int j = 0; j < cur->num; j++) {
				*(cur->nodes[j].error) = derivative(&cur->nodes[j],targets)*(targets[j] - *(cur->nodes[j].output));
			}
		}
		else {
			layer *next = &NN->layers[i + 1];
			#pragma omp parallel for schedule(dynamic)
			for (int j = 0; j < cur->num; j++) {
				float temp = 0;
				for (int k = 0; k < next->num; k++) {
					temp += *(next->nodes[k].error)*next->nodes[k].weights[j];
				}
				*(cur->nodes[j].error) = derivative(&cur->nodes[j],targets)*temp;
			}
		}
	}

	float tempWeight;
	for (int i = NN->layernum - 1; i >= 0; i--) {
		cur = &NN->layers[i];
		#pragma omp parallel for schedule(dynamic)
		for (int j = 0; j < cur->num; j++) {
			tempWeight = cur->nodes[j].threshold;
			cur->nodes[j].threshold += (LR * *(cur->nodes[j].error) * -1);

			for (int k = 0; k < cur->nodes[j].inputnum; k++) {
				tempWeight = cur->nodes[j].weights[k];
				cur->nodes[j].weights[k] += (LR * *(cur->nodes[j].error) * (*cur->nodes[j].inputs)[k]);
			}
		}
	}
}

float* output(network* NN) {
	return *(NN->outputs);
}

void bulidHTree(int* freq, network* NN) {
	cout << "Building Huffman Tree.." << endl;
	vector<wordfreq*> leaf(WORDS);
	for (int i = 0; i < WORDS; i++) {
		leaf[i] = new wordfreq;
		leaf[i]->freq = freq[i];
		leaf[i]->num = i;
	}
	
	int na = WORDS;
	while (na) {
		wordfreq *l1 ,*l2;
		int l1i, l2i;
		if (leaf[0]->freq < leaf[1]->freq) {
			l1 = leaf[0];
			l1i = 0;
			l2 = leaf[1];
			l2i = 1;
		}
		else {
			l1 = leaf[1];
			l1i = 1;
			l2 = leaf[0];
			l2i = 0;
		}
		for (int i = 0; i < na; i++) {
			if (l2->freq > leaf[i]->freq) {
				l2 = leaf[i];
				l2i = i;
				if (l1->freq > l2->freq) {
					wordfreq *temp = l1;
					int tempi = l1i;
					l1 = l2;
					l1i = l2i;
					l2 = temp;
					l2i = tempi;
				}
			}
			wordfreq *temp = new wordfreq;
			temp->freq = l1->freq + l2->freq;
			temp->left = l1;
			temp->right = l2;
			temp->node = &NN->layers[NN->layernum - 1].nodes[WORDS - na];
			
			leaf[l1i] = temp;
			leaf[l2i] = leaf[na-1];
			leaf[na - 1] = leaf[l2i];
			na--;
		}
	}
	

}

int main() {
	int layernum = 2;
	int neuronnum[] = { FEATURES,WORDS };
	int inputnum[] = { WORDS,FEATURES };
	char types[] = { 'g','s' };
	network* net = createNet(2, neuronnum, inputnum, types); //model initialization
	float* input = (float*)calloc(WORDS,sizeof(float)); //input vector
	float* target = (float*)calloc(WORDS,sizeof(float)); //training target vector
	int *inputindex = call_input();
	int **targetindex = call_output();
	for (int i = 0; i < 100000; i++) { //training section
		net->inputs[inputindex[i]] = 1;
		target[targetindex[i][0]] = 1;
		put_input(net, input);
		calculation(net);
		training(net, 0.1, target);
		net->inputs[inputindex[i]] = 0;
		target[targetindex[i][0]] = 0;
		if (!(i % 100))
			cout << i / 100000.0 << endl;
	}
	float *outputs;
	put_input(net, input);
	calculation(net);
	outputs = output(net);
	getchar();
	return 0;
}
