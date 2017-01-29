#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>

using namespace std;

const string WHICH_GESTURE = "w_prawo_podkatem_10.txt";

map<int, map<int, pair<int, int>>> ALL_DATA;

// frameNumber -> landmarkNumber -> x, y
map<int, map<int, pair<int, int>>> getDataAsMap() {
	string line;
	ifstream myfile(WHICH_GESTURE);
	if (myfile.is_open()) {
		int iterator = 1;
		map<int, map<int, pair<int, int>>> mapResults;
		map<int, pair<int, int>> mapInside;
		while (getline(myfile, line)) {
			vector<string> landmark;
			istringstream iss(line);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter(landmark));
			int frame = stoi(landmark.at(0));
			int landmarkNumber = stoi(landmark.at(1)); // stoi - string to int
			int xPositionOfLandmark = stoi(landmark.at(2));
			int yPositionOfLandmark = stoi(landmark.at(3));
			//cout << "frame " << frame << ", landmark: " << landmarkNumber << ", x: " << xPositionOfLandmark << ", y:" << yPositionOfLandmark << endl;

			mapInside[landmarkNumber] = make_pair(xPositionOfLandmark, yPositionOfLandmark);

			mapResults[frame] = mapInside;
		}
		myfile.close();
		return mapResults;
	}
	else {
		cout << "Unable to open file";
	}
}

map<int, map<int, pair<int, int>>> getSpecificLandmark(map<int, map<int, pair<int, int>>> data, int numberOfLandmark) {
	map<int, map<int, pair<int, int>>> result;
	map<int, pair<int, int>> resultInside;

	for (auto elem : data) {
		for (auto elem2 : elem.second) {
			if (elem2.first == numberOfLandmark)
				resultInside[numberOfLandmark] = make_pair(elem2.second.first, elem2.second.second);

				result[elem.first] = resultInside;
		}
	}
	return result;
}

void printData(map<int, map<int, pair<int, int>>> data) {
	for (auto elem : data) {
		for (auto elem2 : elem.second) {
			cout << "frame " << elem.first << ", landmark: " << elem2.first << ", x: " << elem2.second.first << ", y:" << elem2.second.second << endl;
		}
	}
}

void howManyGestures(map<int, map<int, pair<int, int>>> landmark30, map<int, map<int, pair<int, int>>> landmark14) {
	vector<int> landmark30_x;
	vector<int> landmark14_x;
	vector<int> result;

	for (auto elem : landmark30) {
		for (auto elem2 : elem.second) {
			landmark30_x.push_back(elem2.second.first);
		}
	}
	for (auto elem : landmark14) {
		for (auto elem2 : elem.second) {
			landmark14_x.push_back(elem2.second.first);
		}
	}

	for (int i = 0; i < landmark14_x.size(); i++) {
		result.push_back(landmark14_x.at(i) - landmark30_x.at(i));
	}
	
	for (int i = 0; i < result.size(); i++) {
		cout << i << ": " << result.at(i) << endl;
	}
}

int main() {
	ALL_DATA = getDataAsMap();
	map<int, map<int, pair<int, int>>> LANDMARK_30;
	map<int, map<int, pair<int, int>>> LANDMARK_14;

	LANDMARK_30 = getSpecificLandmark(ALL_DATA, 30);
	LANDMARK_14 = getSpecificLandmark(ALL_DATA, 14);

	//printData(LANDMARK_30);
	//printData(LANDMARK_14);

	howManyGestures(LANDMARK_30, LANDMARK_14);
}