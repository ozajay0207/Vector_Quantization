// Vector_Quantization.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "iostream"
#include "fstream"
#include "string"
using namespace std;

ifstream in, in1;
ofstream out;

float tokhura_dist[5] = { 0 };

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

int _tmain(int argc, _TCHAR* argv[])
{

	return 0;
}
