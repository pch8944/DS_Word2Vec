#include"input.h"

#define DIMENSION 10000
#define WINDOW 6

/*
int** call_input();
int** call_output();
int line_of_file();

void main()
{
	//인풋 벡터 얻는 함수
	int** input;
	int i, no;

	input = call_input();

	// 동적할당 해제하는 부분
	no = line_of_file();
	for (i = 0; i < no; i++) {
		free(input[i]);
	}
	free(input);

	int** output;
	
	// 아웃풋벡터 얻는 함수 및 동적할당 해제
	output = call_output();
	for (i = 0; i < no; i++) {
		free(output[i]);
	}
	free(output);

}
*/

int** call_input() {
	FILE *fp;
	errno_t err;

	char buffer[50];
	int no, i=0, j=0, position, sum=0;
	int **arr;

	no = line_of_file();
	arr = (int**)malloc(sizeof(int*) * no);
	for (i = 0; i < no; i++) {
		arr[i] = (int*)malloc(sizeof(int) * DIMENSION);
		for (j = 0; j < DIMENSION; j++) {
			arr[i][j] = 0;
		}
	}

	err = fopen_s(&fp, "D://input.txt", "r");

	j = 0;
	while (j<no) {
		fgets(buffer, sizeof(buffer), fp);
		for (i = 0; buffer[i] != 32; i++) {
			position = i;
		}
		for (i=0,sum=0; position >= 0; i++, position--) {
			sum += pow(10, i)*(buffer[position] - '0');
		}

		arr[j][sum] = 1;
		j++;

	}

	fclose(fp);

	return arr;
}

int** call_output() {
	FILE *fp;
	errno_t err;

	char buffer[50];
	int no, i, j, k=0, position=0, sum, front, end;
	int **arr;

	no = line_of_file();
	arr = (int**)malloc(sizeof(int*) * no);
	for (i = 0; i < no; i++) {
		arr[i] = (int*)malloc(sizeof(int) * WINDOW);
		for (j = 0; j < WINDOW; j++) {
			arr[i][j] = 0;
		}
	}

	err = fopen_s(&fp, "D://input.txt", "r");

	j = 0;
	while (j<no) {
		fgets(buffer, sizeof(buffer), fp);
		k = 0;
		position = 0;
		while (buffer[k] != 10) {
			if (k == 0)
				front = 0;
			else
				front = k+1;

			for (i = front; buffer[i] != 32 && buffer[i] != 10; i++) {
				end = i;
			}
			k = i;

			if (position++ == 0)
				continue;
			for (i = 0, sum = 0; end >= front; i++, end--) {
				sum += pow(10, i)*(buffer[end] - '0');
			}

			arr[j][position - 2] = sum;

		}
		if (position != 7)
			arr[j][position - 1] = -1;
		j++;
	}

	fclose(fp);
	return arr;
}

int line_of_file() {
	FILE *fp;
	errno_t err;

	char file_check[50];
	int i = 0;

	err = fopen_s(&fp, "D://input.txt", "r");
	while (!feof(fp)) {
		fgets(file_check, sizeof(file_check), fp);
		i++;
	}

	fclose(fp);

	return i-1;
}