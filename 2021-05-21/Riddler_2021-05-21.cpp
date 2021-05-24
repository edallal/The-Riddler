/*
 * Riddler_2021-05-21.cpp
 *
 *  Created on: May 23, 2021
 *      Author: Eric
 *
 *  Note: I tried to optimize this as much as I could. This code won't be pretty.
 */

#include <iostream>
#include <utility>
#include <bitset>
#include <unordered_map>
#include <set>
#include <cassert>

#define N 11

using namespace std;

using BS = bitset<N*N>;

inline size_t ReflectX(size_t ind) {
	return (ind / N)*N + N - 1 - (ind % N);
}

inline size_t ReflectY(size_t ind) {
	return (N - 1 - (ind / N))*N + (ind % N);
}

inline size_t ReflectDiag(size_t ind) {
	return (ind % N)*N + (ind / N);
}

// This method adds the index to the set of disallowed bits, but also its various reflections
void AddToDisallowedBits(BS& disallowedBits, size_t curIndex) {
	disallowedBits.set(curIndex);
	curIndex = ReflectX(curIndex);
	disallowedBits.set(curIndex);
	curIndex = ReflectY(curIndex);
	disallowedBits.set(curIndex);
	curIndex = ReflectX(curIndex);
	disallowedBits.set(curIndex);
	curIndex = ReflectDiag(curIndex);
	disallowedBits.set(curIndex);
	curIndex = ReflectX(curIndex);
	disallowedBits.set(curIndex);
	curIndex = ReflectY(curIndex);
	disallowedBits.set(curIndex);
	curIndex = ReflectX(curIndex);
	disallowedBits.set(curIndex);
}

// This method adds the pair to the set of disallowed pairs, but also its various reflections
void AddToDisallowedPairs(unordered_map<size_t, set<size_t>>& disallowedPairs, size_t ind1, size_t ind2) {
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectX(ind1);
	ind2 = ReflectX(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectY(ind1);
	ind2 = ReflectY(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectX(ind1);
	ind2 = ReflectX(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectDiag(ind1);
	ind2 = ReflectDiag(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectX(ind1);
	ind2 = ReflectX(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectY(ind1);
	ind2 = ReflectY(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
	ind1 = ReflectX(ind1);
	ind2 = ReflectX(ind2);
	disallowedPairs[ind1].insert(ind2);
	disallowedPairs[ind2].insert(ind1);
}

pair<size_t, BS> Solve(BS curBS, size_t curSize, size_t curIndex, size_t maxSize, size_t targetSize, BS& disallowedBits, unordered_map<size_t, set<size_t>>& disallowedPairs) {
	if (maxSize < targetSize) return make_pair(curSize, curBS); // Already failed
	if (curIndex == N*N || curSize == targetSize) return make_pair(curSize, curBS); // Done, return

	if (disallowedBits[curIndex]) { // If curIndex is disallowed, then try next one instead
		return Solve(curBS, curSize, curIndex + 1, maxSize, targetSize, disallowedBits, disallowedPairs);
	}

	// Disallowed combinations are pre-computed, so no need to check pairs or triples before adding this bit
	curBS.set(curIndex);

	// Determine bits that become disallowed by adding this one, and determine what maxSize becomes
	BS newDisallowedBits;
	int newMaxSize = maxSize;

	// First examine pairs
	const auto it1 = disallowedPairs.find(curIndex);
	if (it1 != disallowedPairs.cend()) {
		for (const auto& it2 : it1->second) {
			if (!curBS[it2]) return make_pair(curSize, curBS);
			if (it2 > curIndex) { // A bit not yet considered becomes disallowed by adding curIndex
				newDisallowedBits.set(it2);
				--newMaxSize;
			}
		}
	}

	// Now examine triples
	size_t x2 = curIndex % N;
	size_t y2 = curIndex / N;
	for (size_t p3 = curIndex + 1; p3 < N*N; ++p3) {
		if (disallowedBits[p3]) continue;
		size_t x3 = p3 % N;
		size_t y3 = p3 / N;
		size_t d23 = (x3 - x2)*(x3 - x2) + (y3 - y2)*(y3 - y2);
		for (size_t p1 = 0; p1 < curIndex; ++p1) {
			if (!curBS[p1]) continue;
			size_t x1 = p1 % N;
			size_t y1 = p1 / N;
			size_t d13 = (x3 - x1)*(x3 - x1) + (y3 - y1)*(y3 - y1);
			size_t d12 = (x2 - x1)*(x2 - x1) + (y2 - y1)*(y2 - y1);
			if (d12 == d13 || d12 == d23 || d13 == d23) {
				newDisallowedBits.set(p3);
				--newMaxSize;
				break;
			}
		}
	}

	disallowedBits ^= newDisallowedBits;
	pair<size_t, BS> retUse = Solve(curBS, curSize + 1, curIndex + 1, newMaxSize, targetSize, disallowedBits, disallowedPairs);
	// Undo changes to disallowed bits before returning since disallowedBits is a reference
	disallowedBits ^= newDisallowedBits;

	if (retUse.first == targetSize) return retUse; // Success!

	// If here, then using the bit failed to produce a solution of the target size
	if (curSize == 0) { // Can't use this bit -> add to disallowed bits
		AddToDisallowedBits(disallowedBits, curIndex);
	} else if (curSize == 1) { // Can't use this bit and some previously used one -> add to disallowed pairs
		for (size_t othIndex = 0; othIndex < curIndex; ++othIndex) {
			if (curBS[othIndex]) {
				AddToDisallowedPairs(disallowedPairs, curIndex, othIndex);
				break; // There can only be one
			}
		}
	}

	// Now try not using curIndex
	curBS.reset(curIndex);
	pair<size_t, BS> retNoUse = Solve(curBS, curSize, curIndex + 1, maxSize - 1, targetSize, disallowedBits, disallowedPairs);

	return (retUse.first > retNoUse.first ? retUse : retNoUse);
}

pair<size_t, BS> Solve() {
	BS disallowedBits;
	unordered_map<size_t, set<size_t>> disallowedPairs;
	size_t targetSize = 1;
	auto retCur = Solve(BS(), 0, 0, N*N, targetSize, disallowedBits, disallowedPairs);
	while (retCur.first == targetSize) {
		++targetSize;
		auto ret = Solve(BS(), 0, 0, N*N, targetSize, disallowedBits, disallowedPairs);
		if (ret.first > retCur.first) retCur = ret;
	}
	return retCur;
}

void PrintSolution(const pair<size_t, BS> sol) {
	cout << "Solution for " << N << " x " << N << " Grid has " << sol.first << " Points:\n";
	size_t index = 0;
	for (size_t y = 0; y < N; ++y) {
		for (size_t x = 0; x < N; ++x) {
			cout << (sol.second[index++] ? "O" : ".");
		}
		cout << "\n";
	}
	cout << "\n";
}

int main() {
	const auto sol = Solve();
	PrintSolution(sol);
}
