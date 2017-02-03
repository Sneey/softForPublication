#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <vector>
#include <map>

using namespace std;

const string WHICH_GESTURE = "srednio.txt";

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

map<int, int> getSurfaceAreaFromFile() {
	string line;
	ifstream myfile("surfaceArea_srednio.txt");
	if (myfile.is_open()) {
		int iterator = 1;
		map<int, int> mapResults;
		while (getline(myfile, line)) {
			vector<string> landmark;
			istringstream iss(line);
			copy(istream_iterator<string>(iss),
				istream_iterator<string>(),
				back_inserter(landmark));
			int frame = stoi(landmark.at(0));
			int surfaceArea = stoi(landmark.at(1)); // stoi - string to int
			
			mapResults[frame] = surfaceArea;
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

void howManyGestures(map<int, map<int, pair<int, int>>> landmark30, map<int, map<int, pair<int, int>>> landmark14, char flag) {
	vector<int> landmark30_x;
	vector<int> landmark14_x;
	vector<int> result;

	for (auto elem : landmark30) {
		for (auto elem2 : elem.second) {
			if(flag == 'x')
				landmark30_x.push_back(elem2.second.first);
			else
				landmark30_x.push_back(elem2.second.second);
		}
	}
	for (auto elem : landmark14) {
		for (auto elem2 : elem.second) {
			if (flag == 'x')
				landmark14_x.push_back(elem2.second.first);
			else
				landmark14_x.push_back(elem2.second.second);
		}
	}

	for (int i = 0; i < landmark14_x.size(); i++) {
		result.push_back(landmark14_x.at(i) - landmark30_x.at(i));
	}
	
	for (int i = 0; i < result.size(); i++) {
		cout << i << ": " << result.at(i) << endl;
	}
}

void howManyGesturesWithNormalization(map<int, int> normalization, map<int, map<int, pair<int, int>>> landmark30, map<int, map<int, pair<int, int>>> landmark14, char flag) {
	vector<int> landmark30_x;
	vector<int> landmark14_x;
	vector<float> result;

	for (auto elem : landmark30) {
		for (auto elem2 : elem.second) {
			if (flag == 'x')
				landmark30_x.push_back(elem2.second.first);
			else
				landmark30_x.push_back(elem2.second.second);
		}
	}
	for (auto elem : landmark14) {
		for (auto elem2 : elem.second) {
			if (flag == 'x')
				landmark14_x.push_back(elem2.second.first);
			else
				landmark14_x.push_back(elem2.second.second);
		}
	}


	int counter = 0;
	for (auto elem : normalization) {
		//cout << (float)(landmark14_x.at(counter) - landmark30_x.at(counter)) / elem.second << endl;
		//cout << elem.second << endl;
		result.push_back((float)(landmark14_x.at(counter) - landmark30_x.at(counter)) / elem.second * 100);
		counter++;
	}

	for (int i = 0; i < result.size(); i++) {
		cout << i << ": " << result.at(i) << endl;
	}
}

int normalization(int surfaceArea) {
	int division = 0;
	if (surfaceArea < 7000) division = 36;
	else if (surfaceArea < 10000) division = 43;
	else if (surfaceArea < 15000) division = 53;
	else if (surfaceArea < 22000) division = 63;
	else if (surfaceArea < 32000) division = 69;
	else if (surfaceArea < 46000) division = 82;
	else if (surfaceArea < 66000) division = 103;
	else if (surfaceArea < 95000) division = 126;
	else if (surfaceArea < 138000) division = 153;
	else division = 157;
	return division;
}

int main() {
	ALL_DATA = getDataAsMap();
	map<int, map<int, pair<int, int>>> LANDMARK_30;
	map<int, map<int, pair<int, int>>> LANDMARK_14;

	LANDMARK_30 = getSpecificLandmark(ALL_DATA, 30);
	LANDMARK_14 = getSpecificLandmark(ALL_DATA, 14);

	//printData(LANDMARK_30);
	//printData(LANDMARK_14);
	//cout << 'x' << endl;
	//howManyGestures(LANDMARK_30, LANDMARK_14, 'x');
	//cout << 'y' << endl;
	//howManyGestures(LANDMARK_30, LANDMARK_14, 'y');
	map<int, int> resultOfNormalization;
	map<int, int> mapOfSurfaceArea = getSurfaceAreaFromFile();
	int counter = 0;
	for (auto elem : mapOfSurfaceArea) {
		resultOfNormalization[counter] = normalization(elem.second);
		counter++;
	}
	//for (int i = 0; i < mapOfSurfaceArea.size(); i++) {
	//	resultOfNormalization[i] = normalization(mapOfSurfaceArea.at(i));
	//	cout << resultOfNormalization.at(i);
	//}
	cout << 'x' << endl;
	howManyGesturesWithNormalization(resultOfNormalization, LANDMARK_30, LANDMARK_14, 'x');
	cout << 'y' << endl;
	howManyGesturesWithNormalization(resultOfNormalization, LANDMARK_30, LANDMARK_14, 'y');
}