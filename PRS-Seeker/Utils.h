#pragma once
#include <unordered_set>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include "InfInt.h"

using namespace std;

bool isFileExist(std::string name) {
	struct stat buffer;
	return (stat(std::string(name).c_str(), &buffer) == 0);
}

void printSeq(char* seq, int length, char* prefix)
{
	cout << "SEQ: ";
	while (--length >= 0)
		switch (seq[length])
	{
		case -1:
			cout << "-";
			break;
		case 0:
			cout << "0";
			break;
		case 1:
			cout << "1";
			break;
	}
	cout << endl;
}

void printSeq(int* seq, int length, char* prefix)
{
	cout << prefix;
	while (--length >= 0)
			cout << seq[length] << " ";
	cout << endl;
}

void convertNum2Seq(char* dest, unsigned long long int Num, int base, int size)
{
	int length = 0;
	memset(dest, 0, sizeof(dest));
	while (Num > 0)
	{
		dest[length] = char ((Num % base) - 1);
		Num /= base;
		length++;
	}
}

int sumProd(char* first, char* second, int seqLen)
{
	int result = 0, i = 0;
	//printSeq(first, seqLen);
	//printSeq(second, seqLen);
	for (i = 0; i < seqLen; i++) 
	{
		result += first[i] * second[i];
	}
	return result;
}

int *getACF(char* sequence, int seqLen) {
	int i = 0, j = 0, a;
	char* tmp = new char[seqLen];
	int* acf = new int[seqLen]; 
	for (i = 0; i < seqLen; i++) {
		tmp[i] = sequence[i];
	}
	//printSeq(tmp, seqLen);
	for (i = 0; i < seqLen; i++)
	{
		a = tmp[0];
		for (j = 0; j < seqLen - 1; j++)
		{
			tmp[j] = tmp[j + 1];
		}
		tmp[seqLen - 1] = a;
		//printSeq(tmp, seqLen);
		acf[i] = sumProd(sequence, tmp, seqLen);
	}
	delete tmp;
	return acf;
}

char countUniques(int* acf, int length) 
{
	char num = 0, tmp;
	char i = 0, j = 0;
	for (j = 0; j < length; j++){
		tmp = acf[j];
		bool seenThisElementBefore = false;
		for (i = 0; i < j; i++){
			if (tmp == acf[i]){
				seenThisElementBefore = true;
			}
		}
		if (!seenThisElementBefore){
			num++;
		}
	}
	return num;
}

bool isTP(char* seq, int length)
{
	int* acf = getACF(seq, length);
	//printSeq(acf, length);
	//std::unordered_set<char> s(acf, acf + length);
	char uniqs = countUniques(acf, length);
	delete acf;
	//std::cout << "UN: " << uniqs << std::endl;
 	return uniqs == 2;
}

void nextPermute(char* seq, int length)
{
	int	j = length - 2;
	while (j != -1 && seq[j] >= seq[j + 1])
		j--;

	if (j == -1) {
		seq[0] = 2;
	}
	else
	{
		char tmp;
		int k = length - 1;
		while (seq[j] >= seq[k])
			k--;
		tmp = seq[j];
		seq[j] = seq[k];
		seq[k] = tmp;
		int l = j + 1;
		int r = length - 1;
		while (l < r)
		{
			tmp = seq[l];
			seq[l] = seq[r];
			seq[r] = tmp;
			l++;
			r--;
		}
	}
}

void makeInitialSeq(char* seq, int length, int kp, int km)
{
	int i, t = 1;
	int kz = length - km - kp;
	for (i = 0; i < km; i++)
	{
		seq[i] = -1;
	}
	if ((length - km) * (kz) == 0)
	{
		t = (length - km) / (kz);
		cout << "have period: " << t << endl;


		for (int k = 0; k < kz; k++)
		{
			for (i = 0; i < kp / kz; i++)
			{
		//		cout << i + k*t + km << ": 1" << endl;
				seq[i + k*t + km + 1] = 1;
			}
	//		cout << t*(k + 1) + km - 1 << ": 0" << endl;
			seq[t*(k + 1) + km] = 0;
		}
	}
	else
	{
		for (i = 0; i < kz; i++)
		{
			seq[km + i] = 0;
		}
		for (i = 0; i < kp; i++)
		{
			seq[km + kz+i] = 1;
		}
	}

}

void readInitialSeq(char* seq, int v, string fileName)
{
	//TODO: Implement
}

unsigned long long int countTotal(int V, int kp, int km)
{

	int kz = V - km - kp;
	int max = 0, rest1 = 0, rest2= 0;
	if (kp > max) {
		max = kp;
		rest1 = km;
		rest2 = kz;
	}
	if (km > max) {
		max = km;
		rest1 = kp;
		rest2 = kz;
	}
	if (kz > max) {
		max = kz;
		rest1 = kp;
		rest2 = km;
	}
	InfInt res = 1;
	for (int c = max + 1; c <= V; c++){
		res *= c;
	}
	for (int c = 1; c <= rest1; c++) {
		res /= c;
	}
	for (int c = 1; c <= rest2; c++) {
		res /= c;
	}
	return res.toUnsignedLongLong();
}

bool isSuiteToBrookRaiserChovla(char* seq, int length) 
{
	int kp = 0, km = 0, kz = 0, i;
	for (i = 0; i < length; i++)
	{
		switch (seq[i]) 
		{
		case -1:
			km++;
			break;
		case 0:
			kz++;
			break;
		case 1:
			kp++;
			break;
		}
	}
	int v = (kp - km)*(kp - km) - kp - km;

	return (kp*km*kz != 0) && (kp != km) &&
		(kp != length) && (km != length) &&
		(v % (length - 1) == 0);
}

void writeSeq2FileState(char* seq, int length, string filename, char* state)
{
	ofstream myfile;
	myfile.open(filename, ios::app);
	myfile << "STATE: " << state<< endl;
	while (--length >= 0)
		switch (seq[length])
	{
		case -1:
			myfile << "-";
			break;
		case 0:
			myfile << "0";
			break;
		case 1:
			myfile << "1";
			break;
	}
	myfile << endl;
	myfile.close();
}

unsigned long long int ipow(int base, int exp)
{
	unsigned long long int result = 1ULL;
	while (exp)
	{
		if (exp & 1)
		{
			result *= (unsigned long long int)base;
		}
		exp >>= 1;
		base *= base;
	}
	return result;
}