// Vector_Quantization.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#define UNIVERSE_FILE_SIZE 125
#define CODE_BOOK_SIZE 8

using namespace std;

ifstream in, in1;
ofstream out;

float tokhura_dist[5] = { 0 };
float tokhura_weight[12] = { 1, 3, 7, 13, 19, 22, 25, 33, 42, 50, 56, 61 };
float universe_arr[UNIVERSE_FILE_SIZE][12] = {0};
float code_book[CODE_BOOK_SIZE][12] = { 0 };

//Used Files
char* input_file = "input.txt";
char* reference_file[5] = { "ref_file_prime_a.txt", "ref_file_prime_e.txt", "ref_file_prime_i.txt", "ref_file_prime_o.txt", "ref_file_prime_u.txt" };
char* silence_file = "silence.txt";
char* c_prime_file = "delete_it.txt";
char* universe_file = "universe_vowel.txt";
char* hamming_file = "Hamming_window.txt";


//Calculating Tokhura's Distance Using Reference Files for vowels
void calculate_tokhura_distance(){
	int count = 0, j = 0, min_index = 0;
	float ref_sample, input_sample, min = 9999;
	float sum[5] = { 0 }, grand_sum = 0;
	string temp, temp1;
	for (int i = 0; i < 5; i++){
		in.open(reference_file[i]);
		in1.open(c_prime_file);
		//cout << "----New File----" << endl;

		while (in >> temp && in1 >> temp1){
			count++;
			ref_sample = stof(temp);
			input_sample = stof(temp1);
			sum[j] += ((ref_sample - input_sample)*(ref_sample - input_sample))*tokhura_weight[count - 1];
			if (count == 12){
				grand_sum += sum[j];
				//cout << "Grand_Sum : " << grand_sum << endl;
				sum[j] = 0;
				j++;
				count = 0;
			}
			j = 0;
		}
		tokhura_dist[i] = grand_sum / 5;
		grand_sum = 0;
		in1.close();
		in.close();
	}
	cout << "\n\nTokhura Distances are in order of A E I O U" << endl;
	for (int i = 0; i < 5; i++){
		printf("\nTokhura :%.6f", tokhura_dist[i]);
		if (tokhura_dist[i] < min){
			min = tokhura_dist[i];
			min_index = i;
		}
	}
	if (min_index == 0)
		cout << "\n\nVowel is A" << endl;
	else if (min_index == 1)
		cout << "\n\nVowel is E" << endl;
	else if (min_index == 2)
		cout << "\n\nVowel is I" << endl;
	else if (min_index == 3)
		cout << "\n\nVowel is O" << endl;
	else
		cout << "\n\nVowel is U" << endl;

	cout << endl;
}

//To get Centroid for CB of size 1
void get_intial_centroid(){
	int line=0, col=0;
	string temp;	

	in.open(universe_file);
	
		while (in >> temp){
			universe_arr[line][col++] = stof(temp);
			if (col == 12){
				line = line + 1;
				col = 0;
			}
		}

		printf("\nLine : %d", line);
		printf("\nCol : %d", col);

		for (int i = 0; i < UNIVERSE_FILE_SIZE; i++){
			for (int j = 0; j < 12; j++){
				code_book[0][j] += universe_arr[i][j];
			}
		}
		
		for (int i = 0; i < CODE_BOOK_SIZE; i++){
			for (int j = 0; j < 12; j++){
				code_book[i][j] /= 125;
			}
		}

		for (int i = 0; i < CODE_BOOK_SIZE; i++){
			for (int j = 0; j < 12; j++){

				printf("\n %f", code_book[i][j]);
			}
		}

		in.close();
}

void binary_split(){

}

int _tmain(int argc, _TCHAR* argv[])
{
	get_intial_centroid();
	return 0;
}

