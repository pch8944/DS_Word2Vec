#include <iostream>
#include <math.h>

#define WORDS 100000
#define FEATURES 300
#define WINDOW 5
#define LRATE 0.001

using namespace std;

struct node {
	double **inputs;
	double *output;
	double threshold;
	double dthreshold;
	double *weights;
	double *oldWeights;
	int inputnum;
	double *error;
};

struct layer {
	int num;
	node *nodes;
	double* outputs;
};

struct network {
	int inputnum;
	double *inputs;
	double **outputs;
	int layernum;
	layer *layers;
};

double randnum() {
	return (((double)rand() * 2) / ((double)RAND_MAX + 1)) - 1;
}

network* createNet(int layers, int *nodes, int *inputs) {

	network* NN = new network;
	NN->inputnum = inputs[0];
	NN->inputs = new double[NN->inputnum];
	NN->layernum = layers;
	NN->layers = new layer[layers];
	layer *old = NULL;
	for (int i = 0; i < layers; i++) {
		layer *temp = &(NN->layers[i]);
		temp->num = nodes[i];
		temp->nodes = new node[nodes[i]];
		temp->outputs = new double[nodes[i]];
		node *tempnodes = temp->nodes;
		for (int j = 0; j < nodes[i]; j++) {
			tempnodes[j].inputnum = inputs[i];
			if (!old) {
				tempnodes[j].inputs = &(NN->inputs);
			}
			else {
				tempnodes[j].inputs = &(old->outputs);
			}
			tempnodes[j].output = new double;
			tempnodes[j].error = new double;
			temp->outputs[j] = *(tempnodes->output);
			tempnodes[j].weights = new double[inputs[i]];
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

void put_input(network* NN, double *inputs) {
	for (int i=0; i < NN->inputnum; i++) {
		NN->inputs[i] = inputs[i];
	}
}

double get_output(node *Node) {
	double sum = 0;

	for (int i = 0; i < Node->inputnum; i++) {
		//cout << (*Node->inputs)[i]<<" ";
		sum += (*Node->inputs)[i] * Node->weights[i];
	}
	sum += -1 * Node->threshold;
	double temp = -sum;
	*(Node->output) = 1.0 / (1 + exp(temp));
	//cout << *(Node->output) << endl;
	return *(Node->output);
}

void calculation(network *NN) {
	for (int i = 0; i < NN->layernum; i++) {
		for (int j = 0; j < NN->layers[i].num; j++) {
			NN->layers[i].outputs[j]=get_output(&(NN->layers[i].nodes[j]));
		}
	}
}

double derivative(node *Node) {
	double temp = *(Node->output) * (1.0 - *(Node->output));
	return temp;
}

void training(network *NN, double LR, double* targets) {
	layer *cur;
	for (int i = NN->layernum - 1; i >= 0; i--) {
		cur = &NN->layers[i];
		if (i == NN->layernum - 1) {
			for (int j = 0; j < cur->num; j++) {
				*(cur->nodes[j].error) = derivative(&cur->nodes[j])*(targets[j] - *(cur->nodes[j].output));
			}
		}
		else {
			layer *next = &NN->layers[i + 1];
			for (int j = 0; j < cur->num; j++) {
				double temp = 0;
				for (int k = 0; k < next->num; k++) {
					temp += *(next->nodes[k].error)*next->nodes[k].weights[j];
				}
				*(cur->nodes[j].error) = derivative(&cur->nodes[j])*temp;
			}
		}
	}

	double tempWeight;
	for (int i = NN->layernum - 1; i >= 0; i--) {
		cur = &NN->layers[i];
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

double* output(network* NN) {
	return *(NN->outputs);
}

int main() {
	int layernum = 2;
	int neuronnum[] = { FEATURES,WORDS };
	int inputnum[] = { WORDS,FEATURES };
	network* net = createNet(2, neuronnum, inputnum);
	double* input;
	double* target;
	for (int i = 0; i < 100000; i++) {
		put_input(net, input);
		calculation(net);
		training(net, 0.1, target);
	}
	double *outputs;
	put_input(net, input);
	calculation(net);
	outputs = output(net);
	getchar();
	return 0;
}
