#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>

using namespace std;

bool getData(vector<int> &videos) {
	ifstream file;
	file.open("me_at_the_zoo.in");
	if (file.is_open()) {
		string info;
		
		// Gets first line information
		getline(file, info);
		int i_videos = atoi(info.substr(0, info.find(" ")).c_str());
		info = info.substr(info.find(" ") + 1);
		int i_endpoints = atoi(info.substr(0, info.find(" ")).c_str());
		info = info.substr(info.find(" ") + 1);
		int i_request = atoi(info.substr(0, info.find(" ")).c_str());
		info = info.substr(info.find(" ") + 1);
		int i_caches = atoi(info.substr(0, info.find(" ")).c_str());
		info = info.substr(info.find(" ") + 1);
		int i_cacheSize = atoi(info.substr(0, info.find(" ")).c_str());
		
		// Get videos sizes
		getline(file, info);
		for (int i = 0; i < i_videos; i++) {
			videos.push_back(atoi(info.substr(0, info.find(" ")).c_str()));
			if (i < i_videos + 1)
				info = info.substr(info.find(" ") + 1);
		}

		// 
		getline(file, info);
		for (int i = 0; i < i_videos; i++) {
			videos.push_back(atoi(info.substr(0, info.find(" ")).c_str()));
			if (i < i_videos + 1)
				info = info.substr(info.find(" ") + 1);
		}


		file.close();
		return true;
	}
	
	
	//info = info.substr(info.substr(info.find(" ") + 1));


	/*while (getline(file, line)) {
		cout << i++ <<" - " << line << endl;
	}*/

	return false;	
}

int main() {
	//cout << "ola";
	vector<int> videos;
	getData(videos);
	cout << videos.size();
	return 1;
}