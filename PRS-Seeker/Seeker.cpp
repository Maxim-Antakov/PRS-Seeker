#include "Seeker.h"
#include "Utils.h"
#include <string>
#include <sstream>
#include <fstream>
#include <time.h>

const double SHOW_TIME = 90;

Seeker::Seeker(int v, int kp, int km)
{
	_v = v;
	_kp = kp;
	_km = km;
	_kz = v - km - kp;
	_sequence = new char[v];
	_fileName = "TEMP";
}

void Seeker::SetBegin(char* startNum)
{
	_initNum = startNum;
}

void Seeker::SetEnd(char* endNum)
{
	_finishNum = endNum;
}

void Seeker::SetMode(Mode mode)
{
	_mode = mode;
}

void Seeker::Start()
{
	std::stringstream ss;

	switch (_mode)
	{
		case Mode::CONVERTIONS:
		{
			ss << "TP_List" << _v << "_" << _kp << "_" << _km 
				<< "_C_(" << _initNum <<"_"<<_finishNum <<").txt";
			_fileName = ss.str();

			seekConvertions();
			break;
		}
		case Mode::PERMUTATIONS:
		{
			ss << "TP_List" << _v << "_" << _kp << "_" << _km
				<< "_P.txt" ;
			_fileName = ss.str();

			makeInitialSequence();
			seekPermutations();
			break;
		}
		default:
		{
			cout << "Wrong MODE! Exit." << endl;
		}
	} 
}

char* Seeker::GetCurrentSeq()
{
	return _sequence; 
};

Seeker::~Seeker()
{
	delete _sequence;
}

void Seeker::seekPermutations()
{
	unsigned long long int N = 0, M;
	double duration, start = clock();

	while (_sequence[0] != 2)
	{
		N++;
		duration = (clock() - start) / (double)CLOCKS_PER_SEC;
		if (duration > SHOW_TIME)
		{
			cout << "Done: " << N << endl;
			start = clock();
			writeTEMP();
			printSeq(_sequence, _v, "tmp");
		}
		if (isTP(_sequence, _v))
		{
			writeSeq2File();
			printSeq(_sequence, _v, "FIND! ");

			break;
		}
		else {
			nextPermute(_sequence, _v);
		}
	}
}

void Seeker::seekConvertions()
{
	unsigned long long int st, fi, i;
	st = stoull(_initNum, 0, 10);
	fi = stoull(_initNum, 0, 10);

	double duration, start = clock();

	for (i = st; i < fi; i++)
	{
		duration = (clock() - start) / (double)CLOCKS_PER_SEC;
		convertNum2Seq(_sequence, i, _base, _v);
		if (duration > SHOW_TIME)
		{
			cout << "Done: " << i-st << endl;
			start = clock();
			writeTEMP();
			printSeq(_sequence, _v, "tmp");
		}
		if (isSuiteToBrookRaiserChovla(_sequence, _v))
		{
			if (isTP(_sequence, _v))
			{
				writeSeq2File();
				printSeq(_sequence, _v, "GOOD!");
			}
		}
	}
}

void Seeker::makeInitialSequence()
{
	if (isFileExist(_fileName)) {
		readInitialSeq(_sequence, _v, _fileName);
	} 
	else
	{
		makeInitialSeq(_sequence, _v, _kp, _km);
	}
}

void Seeker::writeTEMP()
{
	ofstream myfile;
	myfile.open(_fileName);
	cout << "TEMP SEQ:";
	myfile.close();
	writeSeq2File();
}

void Seeker::writeTEMPNum(unsigned long long int num)
{
	ofstream myfile;
	myfile.open(_fileName);
	cout << "TEMP NUM:" << num << endl;
	myfile.close();
}


void Seeker::writeSeq2File()
{
	ofstream myfile;
	int length = _v;
	cout << "Will write to file: " << _fileName << endl;
	myfile.open(_fileName.c_str(), ios::app);
	while (--length >= 0)
		switch (_sequence[length])
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
