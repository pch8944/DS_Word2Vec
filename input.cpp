#include"input.h"

#define WINDOW 6

/*
void call_input(int* arr);
void call_output(int** arr);
void call_frequency(int* arr);
int line_of_file();
int line_of_freq();
int get_index(char* word);
char* get_word(int index, char* word);

void main()
{

	//인풋 벡터 얻는 함수
	int* input;
	int i, no;

	no = line_of_file();
	input = (int*)malloc(sizeof(int) * no);
	call_input(input);

	printf("%d\n", input[2]);

	// 동적할당 해제하는 부분
	free(input);
	
	// 아웃풋벡터 얻는 함수 및 동적할당 해제
	int** output;
	int j;

	no = line_of_file();
	output = (int**)malloc(sizeof(int*) * no);
	for (i = 0; i < no; i++) {
		output[i] = (int*)malloc(sizeof(int) * WINDOW);
		for (j = 0; j < WINDOW; j++) {
			output[i][j] = 0;
		}
	}
	call_output(output);
	printf("%d\n", output[1][2]);

	for (i = 0; i < no; i++) {
		free(output[i]);
	}
	free(output);


	// frequency 호출 및 동적할당 해제
	int* frequency;

	no = line_of_freq();
	frequency = (int*)malloc(sizeof(int) * no);

	call_frequency(frequency);
	printf("%d\n", frequency[2]);

	free(frequency);

	// dictionary 내의 위치 찾기
	int index;
	index = get_index("hello");
	printf("%d\n", index);

	// 번호를 얻었을때 매칭되는 단어 찾기
	char word[50];
	get_word(2, word);
	printf("%s\n", word);

	return;

}
*/

void call_input(int* arr) {
	FILE *fp;
	errno_t err;

	char buffer[50];
	int no, i=0, j=0, position, sum=0;

	no = line_of_file();

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

		arr[j] = sum;
		j++;

	}

	fclose(fp);

	return;
}

void call_output(int** arr) {
	FILE *fp;
	errno_t err;

	char buffer[50];
	int no, i, j, k=0, position=0, sum, front, end;

	no = line_of_file();

	err = fopen_s(&fp, "D://input.txt", "r");

	j = 0;
	while (j<no) {	//한 줄씩 호출
		fgets(buffer, sizeof(buffer), fp);
		k = 0;
		position = 0;
		while (buffer[k] != 10) { // 공백으로 구분
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
			if (buffer[end] != 'x') {
				for (i = 0, sum = 0; end >= front; i++, end--) {
					sum += pow(10, i)*(buffer[end] - '0');
				}
			}
			else
				sum = -1;

			arr[j][position - 2] = sum;

		}
		if (position != 7)
			arr[j][position - 1] = -1;
		j++;
	}

	fclose(fp);
	return;
}

void call_frequency(int* arr) {
	FILE *fp;
	errno_t err;

	char buffer[50];
	int no, i, j, position = 0, sum;

	no = line_of_freq();

	err = fopen_s(&fp, "D://frequency.txt", "r");

	
	j = 0;
	while (j<no) {
		fgets(buffer, sizeof(buffer), fp);
		for (i = 0; buffer[i] != 10; i++) {
			position = i;
		}
		for (i = 0, sum = 0; position >= 0; i++, position--) {
			sum += pow(10, i)*(buffer[position] - '0');
		}

		arr[j] = sum;
		j++;

	}

	fclose(fp);

	return;
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

int line_of_freq() {
	FILE *fp;
	errno_t err;

	char file_check[50];
	int i = 0;

	err = fopen_s(&fp, "D://frequency.txt", "r");
	while (!feof(fp)) {
		fgets(file_check, sizeof(file_check), fp);
		i++;
	}

	fclose(fp);

	return i - 1;
}

int get_index(char* word) {
	char buffer[50];
	char command[50] = "python D://word_getIndex.py ";
	FILE *fp;
	int result;

	strcat_s(command, sizeof(command), word);

	fp = _popen(command, "r");
	if (NULL == fp)
	{
		perror("error: cannot run python file\n");
		return -1;
	}

	fgets(buffer, sizeof(buffer), fp);

	result = atoi(buffer);

	_pclose(fp);

	return result;

}

char* get_word(int index, char* word) {
	char buffer[50];
	char number[10] = { 0, };
	char command[50] = "python D://word_getWord.py ";
	FILE *fp;
	int i;

	_itoa_s(index, number, 10);

	strcat_s(command, sizeof(command), number);

	fp = _popen(command, "r");
	if (NULL == fp)
	{
		perror("error: cannot run python file\n");
		word[0] = -1;
		word[1] = '\0';
		return word;
	}

	fgets(buffer, sizeof(buffer), fp);
	for (i = 0; buffer[i] != '\n'; i++) {
		word[i] = buffer[i];
	}
	word[i] = '\0';
	printf("%s", buffer);

	_pclose(fp);

	return word;
}
