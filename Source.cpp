#include <iostream>
#include <string>
#include <cstdlib>
#include <fstream>
#include <vector>
#include <map>

using namespace std;

class Endpoint;
class CacheServer;
class Video;

class Endpoint {
public:
	map <int, int> video_requests;	// vector holding all of the requests. key = video (as index), value = requests
	map <int, int> latencies;	// key = server, value = latencies
	int data_center_latency;
};

class CacheServer {
public:
	vector<Video> videos_stored; // videos stored on the server
	int free_space;
	int storage;
};

class Video {
public:
	int id;
	int size;
	int currSavedTime = 0;
};

class CacheData {
public:
	int numCaches;
	int cacheSize;
};

bool getData(vector<Video *> &videos, vector<Endpoint *> &endpoints, CacheData * cacheData) {
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
		cacheData->cacheSize = i_cacheSize;
		cacheData->numCaches = i_caches;

		// Get videos sizes
		getline(file, info);
		for (int i = 0; i < i_videos; i++) {
			int videoSize = atoi(info.substr(0, info.find(" ")).c_str());
			Video *video = new Video;
			video->id = i;
			video->size = videoSize;
			videos.push_back(video);
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
		}

		file.close();
		return true;
	}
	return false;
}

int printCacheServerVideos(CacheServer server, ofstream& output)
{
	for (size_t i = 0; i < server.videos_stored.size(); i++)
	{
		output << server.videos_stored[i].id;

		if (i < server.videos_stored.size() - 1)
		{
			output << " ";
		}
	}
}

int outputResults(vector <CacheServer> cacheServers) {

	ofstream file;
	file.open("me_at_the_zoo.out");

	if (!file.is_open())
	{
		cout << "Could not open file for output.\n";
		return -1;
	}
	else
	{
		int numOfServers = cacheServers.size();

		file << numOfServers << "\n";

		for (vector<CacheServer>::size_type i = 0; i != cacheServers.size(); i++) {
			file << i << " ";

			printCacheServerVideos(cacheServers[i], file);

			// Adding new line except for last line
			if (i < cacheServers.size() - 1)
			{
				file << "\n";
			}
		}

	}

	file.close();

	return 0;
}

int main() {

	vector<Video *> videos;
	vector<Endpoint *> endpoints;
	vector<CacheServer> caches;
	CacheData *cacheData = new CacheData;

	/*Video v1;
	v1.size = 50;
	v1.id = 3;
	videos.push_back(v1);

	Endpoint endP1, endP2, endP4;
	endP1.video_requests.insert(pair<int, int>(3, 1500));
	endP1.data_center_latency = 1000;
	
	endP2.video_requests.insert(pair<int, int>(3, 2500));
	endP4.video_requests.insert(pair<int, int>(3, 200));
	endpoints.push_back(endP1);
	endpoints.push_back(endP2);
	endpoints.push_back(endP4);

	vector<CacheServer> caches;
	CacheServer cs1, cs2;
	cs1.storage = 100;
	cs1.videos_stored.push_back(v1);
	caches.push_back(cs1);
	caches.push_back(cs2);*/

	getData(videos, endpoints, cacheData);

	for (int i = 0; i < cacheData->numCaches; i++) {
		CacheServer cacheServer;
		cacheServer.storage = cacheData->cacheSize;
		caches.push_back(cacheServer);
	}

	for (int i = 0; i < videos.size(); i++) {
		cout << "1";
		map<int, int> infoCache;		// Info for the comparison of caches.

		for (int j = 0; j < caches.size(); j++) {
			vector<int> endPVideoSum;
			vector<int> endPRequests;
			cout << "2";

			for (int k = 0; k < endpoints.size(); k++) {
				map<int, int>::iterator it;
				if ((it = endpoints[k]->video_requests.find(i)) != endpoints[k]->video_requests.end()) {
					int numRequests = it->second;
					int serverTime = numRequests * endpoints[k]->data_center_latency;

					map<int, int>::iterator itr;
					if ((itr = endpoints[k]->latencies.find(j)) != endpoints[k]->latencies.end()) {
						int serverLatency = itr->second;
						int cacheTime = numRequests * serverLatency;

						endPVideoSum.push_back(serverTime - cacheTime);
						endPRequests.push_back(numRequests);
					}
				}
			}

			int sumLatencies = 0, sumRequests = 0;
			for (int k = 0; k < endPVideoSum.size(); k++) {
				sumLatencies += endPVideoSum[k];
				sumRequests += endPRequests[k];
			}

			if (sumLatencies == 0 || sumRequests == 0)
				continue;
			int w = sumLatencies / sumRequests;
			infoCache.insert(pair<int, int>(j, w));
		}

		/*Selects the best cache for a particular video*/
		int maxCacheIndex = 0;
		int maxTimeSaved = 0;
		int index = 0;
		map<int, int>::iterator it;
		for (it = infoCache.begin(); it != infoCache.end(); it++) {
			if (it->second > maxTimeSaved) {
				maxTimeSaved = it->second;
				maxCacheIndex = index;
			}
			index++;
		}

		cout << "Max = " << maxCacheIndex << endl;

		caches[maxCacheIndex].free_space -= videos[i]->size;
		caches[maxCacheIndex].videos_stored.push_back(*videos[i]);
	}

	for (int i = 0; i < caches.size(); i++) {
		for (int j = 0; j < caches[i].videos_stored.size(); j++)
			cout << "Cache " << i << " com video : " << caches[i].videos_stored[j].id << endl;
	}

		//getData(videos);
		//cout << videos.size();


		return 1;
	}