// Vector_Quantization.cpp : Defines the entry point for the console application.


#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
#define UNIVERSE_FILE_SIZE 6340
#define CODE_BOOK_SIZE 8

using namespace std;

ifstream in, in1;
ofstream out;

int current_cb_size = 1;
int bin_index[UNIVERSE_FILE_SIZE] = { -1 };

float tokhura_weight[12] = { 1, 3, 7, 13, 19, 22, 25, 33, 42, 50, 56, 61 };
float universe_arr[UNIVERSE_FILE_SIZE][12] = {0};
float code_book[CODE_BOOK_SIZE][12] = { 0 };
float min_tokhura_universe[UNIVERSE_FILE_SIZE];

//Used Files
char* input_file = "input.txt";
char* universe_file = "Universe1.txt";

//To Print CodeBook
void print_code_book(){
	cout << "\nCode Book" << endl;
	for (int i = 0; i < CODE_BOOK_SIZE; i++){
		for (int j = 0; j < 12; j++){
			cout << " " << code_book[i][j];
		}
		cout << endl;
	}
}

//Calculating Tokhura's Distance Using Code Book
void calculate_tokhura_distance(float c[12],int universe_index){
	int  min_index = 0;
	float min = 99999;
	float sum[CODE_BOOK_SIZE] = { 0 };
	string temp, temp1;

	for (int j = 0; j < current_cb_size; j++){		
		for (int i = 0; i < 12; i++){
			sum[j] += tokhura_weight[i] * (c[i] - code_book[j][i])*(c[i] - code_book[j][i]);
		}		
		if (sum[j] < min){
			min = sum[j];
			min_index = j;
		}
	}

	min_tokhura_universe[universe_index] = min;
	bin_index[universe_index] = min_index;
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

		
		for (int i = 0; i < UNIVERSE_FILE_SIZE; i++){
			for (int j = 0; j < 12; j++){
				code_book[0][j] += universe_arr[i][j];
			}
		}
		
		for (int i = 0; i < CODE_BOOK_SIZE; i++){
			for (int j = 0; j < 12; j++){
				code_book[i][j] /= UNIVERSE_FILE_SIZE;
			}
		}

		in.close();
}

//To perform Binary Split operation
void binary_split(){
	float epsilon = 0.03;
	float temp = 0;
	cout << "\nCurrent Code Book Size : " << current_cb_size << endl;

	for (int i = current_cb_size-2; i >=0 ; i--){
		for (int j = 0; j < 12; j++){
			temp = code_book[i][j];
			code_book[2*i][j] = temp - epsilon;
			code_book[2*i + 1][j] = temp + epsilon;
		}
	}
	
}

//To optimize the CodeBook
void update_code_book(){
	int no_of_entries[CODE_BOOK_SIZE] = { 0 };
	float temp_code_book_sum[CODE_BOOK_SIZE][12] = { 0 };
	for (int i = 0; i < UNIVERSE_FILE_SIZE; i++){		
		no_of_entries[bin_index[i]] += 1;
		for (int j = 0; j < 12; j++){
			temp_code_book_sum[bin_index[i]][j] += universe_arr[i][j];				
		}
	}

	for (int i = 0; i < current_cb_size; i++){
		cout << "No of Entries for :" << i << " : " << no_of_entries[i]<<endl;
		for (int j = 0; j < 12; j++){
			code_book[i][j] = temp_code_book_sum[i][j] / no_of_entries[i];
			temp_code_book_sum[i][j] = 0;			
		}		
	}

}

//Performs K Means Clustering for training vectors
void K_means(){
	for (int i = 0; i < UNIVERSE_FILE_SIZE; i++){
		calculate_tokhura_distance(universe_arr[i], i);
	}

	update_code_book();
}

//To Calculate Average Distortion using minimum tokhura distortion of training vectors
long double calculate_avg_distortion(float min_tokhura_universe[UNIVERSE_FILE_SIZE]){
	long double sum = 0;
	for (int i = 0; i < UNIVERSE_FILE_SIZE; i++){
		sum += min_tokhura_universe[i];
		min_tokhura_universe[i] = 0;
	}
		
	return sum/UNIVERSE_FILE_SIZE;
}

//To perform Binary Split Operation
void Linde_Buzo_Gram(){
	out.open("Distortion.txt");
	double prev_distortion = 0, distortion;

	get_intial_centroid();

	while (current_cb_size < CODE_BOOK_SIZE){
		current_cb_size = current_cb_size * 2;
		binary_split();
		for (int i = 0; i < 20; i++){
			K_means();
			distortion = calculate_avg_distortion(min_tokhura_universe);
			cout << "\nPrev Distortion:" << prev_distortion << endl;
			cout << "Current Distortion:" << distortion << endl;
			cout << "Difference:" << prev_distortion - distortion << endl;
			prev_distortion = distortion;
			out << distortion << endl;
		}
		out << endl;
	}
	out.close();
	print_code_book();
}

int _tmain(int argc, _TCHAR* argv[])
{	
	Linde_Buzo_Gram();
	return 0;
}

