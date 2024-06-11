#include <iostream>
#include <vector>
#include <unordered_set>
#include <algorithm>

using namespace std;

// Define page structure
struct Page {
	int id;
	int lastUsed;
};

// FIFO page replacement algorithm
int FIFOPageFaults(const vector<int>& pages, int frameSize) {
	unordered_set<int> frame;
	int faults = 0;

	for (int page : pages) {
		if (frame.size() < frameSize) {
			if (frame.find(page) == frame.end()) {
				frame.insert(page);
				faults++;
			}
		}
		else {
			if (frame.find(page) == frame.end()) {
				frame.erase(frame.begin());
				frame.insert(page);
				faults++;
			}
		}
	}

	return faults;
}

// LRU page replacement algorithm
int LRUPageFaults(const vector<int>& pages, int frameSize) {
	vector<Page> frame(frameSize);
	int faults = 0;

	for (int i = 0; i < pages.size(); ++i) {
		bool found = false;
		for (int j = 0; j < frameSize; ++j) {
			if (frame[j].id == pages[i]) {
				frame[j].lastUsed = i;
				found = true;
				break;
			}
		}
		if (!found) {
			faults++;
			int minIdx = 0;
			for (int j = 1; j < frameSize; ++j) {
				if (frame[j].lastUsed < frame[minIdx].lastUsed) {
					minIdx = j;
				}
			}
			frame[minIdx].id = pages[i];
			frame[minIdx].lastUsed = i;
		}
	}

	return faults;
}

// OPT page replacement algorithm
int OPTPageFaults(const vector<int>& pages, int frameSize) {
	vector<int> frame(frameSize, -1);
	int faults = 0;

	for (int i = 0; i < pages.size(); ++i) {
		bool found = false;
		for (int j = 0; j < frameSize; ++j) {
			if (frame[j] == pages[i]) {
				found = true;
				break;
			}
		}
		if (!found) {
			faults++;
			int idxToReplace = 0;
			int farthest = -1;
			for (int j = 0; j < frameSize; ++j) {
				int k;
				for (k = i + 1; k < pages.size(); ++k) {
					if (pages[k] == frame[j]) {
						break;
					}
				}
				if (k == pages.size()) {
					idxToReplace = j;
					break;
				}
				else if (k > farthest) {
					farthest = k;
					idxToReplace = j;
				}
			}
			frame[idxToReplace] = pages[i];
		}
	}

	return faults;
}

int main() {
	vector<int> pages = { 1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5 }; // Example page sequence
	int frameSize = 3; // Frame size

	cout << "FIFO Page Faults: " << FIFOPageFaults(pages, frameSize) << endl;
	cout << "LRU Page Faults: " << LRUPageFaults(pages, frameSize) << endl;
	cout << "OPT Page Faults: " << OPTPageFaults(pages, frameSize) << endl;

	return 0;
}
