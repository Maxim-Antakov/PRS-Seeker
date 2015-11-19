#pragma once
#include <string>
class Seeker
{

public:
	enum Mode {
		PERMUTATIONS,
		CONVERTIONS
	};

	Seeker(int V, int kp, int km);

	void SetMode(Mode mode);
	void SetBegin(char* initNum);
	void SetEnd(char* finishNum);
	
	void Start();

	char *GetCurrentSeq();

	~Seeker();

private:
	int _v;
	int _base = 3; //TODO: change dynamicaly
	int _kp;
	int _km;
	int _kz;
	char*  _initNum;
	char*  _finishNum;
	char* _sequence;
	Mode _mode;
	std::string _fileName;

	void makeInitialSequence();
	void seekPermutations();
	void seekConvertions();

	void writeTEMP();
	void writeTEMPNum(unsigned long long int num);

	void writeSeq2File();
	void writeSeq2File(std::string message);
};

