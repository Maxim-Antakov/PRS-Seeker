#include <iostream>
#include <fstream>
#include <ctime>
#include "math.h"
#include "Seeker.h"

using namespace std;

int main(int argc, char** argv) {
	int V, kp = 0, km = 0;
	char* start = "0";
	char* finish = "-1";
	Seeker::Mode mode;
	for (int i = 1; i < argc; ++i) {
		if (std::string(argv[i]) == "--vkpkm") {
			if (i + 3 < argc) { // Make sure we aren't at the end of argv!
				V = atoi(argv[i + 1]);
				kp = atoi(argv[i + 2]);
				km = atoi(argv[i + 3]);
				mode = Seeker::Mode::PERMUTATIONS;
			}
			else { // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}
		}
		if (std::string(argv[i]) == "--vsf") {
			if (i + 3 < argc) { // Make sure we aren't at the end of argv!
				V = atoi(argv[i + 1]);
				start = argv[i + 2];
				finish = argv[i + 3];
				mode = Seeker::Mode::CONVERTIONS;
			}
			else { // Uh-oh, there was no argument to the destination option.
				std::cerr << "--destination option requires one argument." << std::endl;
				return 1;
			}
		}
	}


	cout << "------------------------" << endl;
	cout << "---------BEGIN----------" << endl;
	cout << "Start seeking for V: " << V << endl;
	Seeker seeker = Seeker(V, kp, km);
	seeker.SetMode(mode);
	if (mode==Seeker::Mode::PERMUTATIONS)
	{
		cout << "KP: " << kp << "; KM: " << km << endl;
	}

	if (mode == Seeker::Mode::CONVERTIONS)
	{
		cout << "START: " << start << endl << "FINISH: " << finish << endl;
		seeker.SetBegin(start);
		seeker.SetEnd(finish);
	}
	seeker.Start();

	cout << "----------END-----------" << endl;
	cout << "------------------------" << endl;

}