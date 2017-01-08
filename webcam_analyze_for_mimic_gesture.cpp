#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>

using namespace std;

class Memory {

};

class Landmark {
public:
	Landmark() {};
	Landmark(vector<string>);
	int numberOfLandmark;
};

Landmark::Landmark(vector<string> informationAboutLandmark) {

}

class Frame {
public:
	Frame() {};
	Frame(Landmark);
};

Frame::Frame(Landmark landmark) {

}

class Point {
public:
	int x;
	int y;
};



int main() {
	string line;
	ifstream myfile("dane.txt");
	if (myfile.is_open()) {
		int iterator = 1;
		while (getline(myfile, line)) {
			vector<string> landmark;
			istringstream iss(line);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter(landmark));
			int landmarkNumber = stoi(landmark.at(0)); // stoi - string to int
			int xPositionOfLandmark = stoi(landmark.at(1));
			int yPositionOfLandmark = stoi(landmark.at(2));
			Frame(Landmark(landmark));
		}
		myfile.close();
	}
	else {
		cout << "Unable to open file";
	}
}


