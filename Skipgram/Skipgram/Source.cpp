#include <iostream>
#include <math.h>

#define WORDS 100000
#define FEATURES 300
#define WINDOW 5

using namespace std;

float input[WORDS];
float hidden[FEATURES];
float output[WORDS];
float target[WORDS];
float prob_dist[WORDS];

float error;

float weight01[WORDS][FEATURES];
float weight12[FEATURES][WORDS];

void cost_func() {
	float entropy = 0;
	for (int i = 0; i < WORDS; i++) {
		entropy += -target[i] * log2(prob_dist[i]) - (1 - target[i]) * log2(1 - prob_dist[i]);
	}
}

void flush_arrays() {
	for (int i = 0; i < WORDS; i++) {
		input[i] = 0;
		output[i] = 0;
	}
	for (int i = 0; i < FEATURES; i++) {
		hidden[i] = 0;
	}
	for (int i = 0; i < WORDS; i++) {
		for (int j = 0; j < FEATURES; j++) {
			weight01[i][j] = 0;
			weight12[j][i] = 0;
		}
	}
}

void input_to_hidden() {
	for (int i = 0; i < FEATURES; i++) {
		for (int j = 0; j < WORDS; j++) {
			hidden[i] += input[j] * weight01[j][i];
		}
	}
}

void hidden_to_output() {
	for (int i = 0; i < WORDS; i++) {
		for (int j = 0; j < FEATURES; j++) {
			output[i] += hidden[j] * weight12[j][i];
		}
	}
}

void softmax() {
	float sum = 0;
	for (int i = 0; i < WORDS; i++) {
		prob_dist[i] = exp(output[i]);
		sum += prob_dist[i];
	}
	for (int i = 0; i < WORDS; i++) {
		prob_dist[i] = prob_dist[i]/sum;
	}
}

void feed_forward() {
	input_to_hidden();
	hidden_to_output();
	softmax();
}

void back_prop() {

}

void main() {

}