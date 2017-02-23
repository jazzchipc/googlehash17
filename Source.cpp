#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

using namespace std;


class Endpoint {
public:
	map <int, int> video_requests;	// key = video, value = requests
	map <int, int> latencies;	// key = server, value = latencies
	int data_center_latency;
};

class CacheServer {
public:
	int* videos_stored; // videos stored on the server
	int free_space;
};

bool getData(vector<int> &videos, vector<Endpoint *> &endpoints) {
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
		int i_requestDescriptions = atoi(info.substr(0, info.find(" ")).c_str());
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

		// Get endpoints data
		for (int ep = 0; ep < i_endpoints; ep++) {
			getline(file, info);
			Endpoint *endpoint = new Endpoint();

			endpoint->data_center_latency = atoi(info.substr(0, info.find(" ")).c_str());
			info = info.substr(info.find(" ") + 1);
			int numCacheServers = atoi(info.substr(0, info.find(" ")).c_str());
			
			// Get latency for each cache server
			for (int cs = 0; cs < numCacheServers; cs++) {
				getline(file, info);
				int cacheServerIndex = atoi(info.substr(0, info.find(" ")).c_str());
				info = info.substr(info.find(" ") + 1);
				int cacheServerLatency = atoi(info.substr(0, info.find(" ")).c_str());
				endpoint->latencies.insert(*(new pair<int, int>(cacheServerIndex, cacheServerLatency)));
			}
			
			endpoints.push_back(endpoint);
		}

		// Get video request descriptions
		for (int rd = 0; rd < i_requestDescriptions; rd++) {
			getline(file, info);
			int videoIdx = atoi(info.substr(0, info.find(" ")).c_str());
			info = info.substr(info.find(" ") + 1);
			int endpointIdx = atoi(info.substr(0, info.find(" ")).c_str());
			info = info.substr(info.find(" ") + 1);
			int numRequests = atoi(info.substr(0, info.find(" ")).c_str());
			Endpoint *endpoint = endpoints.at(endpointIdx);
			endpoint->video_requests.insert(*(new pair<int, int>(videoIdx, numRequests)));
			//cout << endpointIdx << " " << videoIdx << " " << numRequests << endl;
		}

		file.close();
		return true;
	}
	return false;
}

int main() {
	vector<int> videos;
	vector<Endpoint *> endpoints;
	getData(videos, endpoints);
	cout << "--------" << endpoints.at(0)->video_requests.size();
	
	return 1;
}