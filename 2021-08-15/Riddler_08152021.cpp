/*
 * Riddler_08152021.cpp
 *
 *  Created on: Aug 15, 2021
 *      Author: Eric
 */

#include <iostream>
#include <vector>

using namespace std;

// Generates ordered dice rolls
bool GenerateNextOrderedRoll(vector<int>& vec) {
	size_t index = 0;
	while (index < vec.size() && vec[index] == 6) ++index;
	if (index == vec.size()) return false;
	++vec[index];
	while (index > 0) {
		--index;
		vec[index] = vec[index+1];
	}
	return true;
}

// Determines the number of permutations corresponding to the ordered dice roll
unsigned long long GetNumPermutations(const vector<int>& vec) {
	unsigned long long perms = 1;
	int curVal = 0;
	int curCount = 0;
	int totCount = 0;
	for (int v : vec) {
		++totCount;
		perms *= totCount;
		if (v != curVal) {
			curCount = 0;
			curVal = v;
		}
		++curCount;
		perms /= curCount;
	}
	return perms;
}

double SolveNext(const vector<double>& sol) {
	vector<int> vec(sol.size(), 1);
	double total = 0;
	do {
		unsigned long long numPerm = GetNumPermutations(vec);
		int curSum = 0;
		double bestVal = 0;
		// Determine optimal solution by considering each possible number of dice to re-roll
		for (size_t i = 0; i < vec.size(); ++i) {
			curSum += vec[i];
			double curVal = curSum + sol[vec.size() - 1 - i];
			bestVal = max(curVal, bestVal);
		}
		total += (numPerm*bestVal);
	} while (GenerateNextOrderedRoll(vec));
	for (size_t i = 0; i < sol.size(); ++i) total /= 6;
	return total;
}

int main() {
	vector<double> sol = {0};
	cout << "Dice\tAverageScore\n";
	for (int i = 0; i < 28; ++i) {
		sol.push_back(SolveNext(sol));
		cout << (i + 1) << "\t" << sol.back() << "\n";
	}
}
