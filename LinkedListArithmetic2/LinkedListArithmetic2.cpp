// LinkedListArithmetic2.cpp
// Adam Flammino 6/19  SDEV 245
// Takes two integers, groups them into nodes of three in a linked list
// and performs addition and subtraction while maintaining groups of 3

#include "stdafx.h"
#include <list>
#include <iostream>
#include <math.h>
#include <iomanip>
#include <string>
#include <vector>


class NodeArithmetic {
private:
	int digit;

public:
	// Populates linked list with each node holding 3 digits
	void populateList(int x, std::list<int> &numbers){
		if (x >= 1000 || x <= -1000) {
			populateList(x / 1000, numbers); // Recursive function to grab chunks of 3 digits
		}
		digit = x % 1000;
		numbers.push_back(digit);
	}
	// Adds values two lists, maintaining three digits per node
	void add(std::list<int> &val1, std::list<int> &val2, std::list<int> &end) {
		std::list<int> temp;
		std::list<int>::iterator it;
		std::list<int>::reverse_iterator rit1 = val1.rbegin();
		std::list<int>::reverse_iterator rit2 = val2.rbegin();
		int carry = 0;
		int sum;
		while (rit1 != val1.rend() && rit2 != val2.rend()) {
			sum = *rit1 + *rit2 + carry;
			carry = 0;
			if (sum / 1000 == 1) { // Carries values to keep nodes at 3 digits
				carry = 1;
				sum -= 1000;
			}
			temp.push_front(sum);
			++rit1;
			++rit2;
			}
		end.clear(); // empties old total
		it = temp.begin();
		while (it != temp.end()) { // copies sum back into total
			end.push_back(*it);
			++it;
		}
	}
	// Subtracts values in two lists, maintaining three digits per node 
	void subtract(std::list<int> &val1, std::list<int> &val2, std::list<int> &end) {
		std::list<int> temp;
		std::list<int>::iterator it;
		std::list<int>::reverse_iterator rit1 = val1.rbegin();
		std::list<int>::reverse_iterator rit2 = val2.rbegin();
		bool flag = false; // marks when a value was borrowed against last cycle
		while (rit1 != val1.rend()) {
			if  ((*rit1 >= *rit2 && !flag) || (*rit1 - 1) >= *rit2) {
				if (!flag) {
					temp.push_front(*rit1 - *rit2);
				}
				else {
						temp.push_front(*rit1 - 1 - *rit2); // "pays back" lent value
						flag = false;
					}
			}
			else { // If first value is too small borrows against higher value in next node
				if (!flag) {
					temp.push_front(*rit1 + 1000 - *rit2);
					flag = true;
				}
				
				else {
					temp.push_front(*rit1 + 999 - *rit2); // pays back by only borrowing 999
				}
			}
			++rit1;
			++rit2;
		}
		end.clear(); // empties old total
		it = temp.begin();
		while (it != temp.end()) { // copies sum back into total
			end.push_back(*it);
			++it;
		}
	}
};


int main()
{

	std::list<int> end;
	std::list<int> nums;
	std::list<int> threeDigits;
	std::vector<bool> signs;
	std::vector<std::list<int>> lists;
	std::list<int>::iterator it;
	std::list<int>::iterator valCheck1;
	std::list<int>::iterator valCheck2;
	bool flag;
	nums.push_back(3453);
	nums.push_back(785);
	nums.push_back(-542);
	nums.push_back(47842124);
	
	it = nums.begin();
	NodeArithmetic na;
	//signs.push_back(true); // First value will be 0, the rest need to be offset by one
	while (it != nums.end()) {
		na.populateList(abs(*it), threeDigits);
		lists.push_back(threeDigits);
		threeDigits.clear();
		if (*it < 0) {
			flag = false;
		}
		else {
			flag = true;
		}
		signs.push_back(flag);
		++it;
	}

	std::string sign = ""; // holds sign for negative results

	for (int i = 0; i < lists.size(); i++) { // adds nodes of leading 0's
		for (int j = 0; j < lists.size(); j++) {
			while (lists[j].size() < lists[i].size()) {
				lists[j].push_front(0);
			}
		}
	}
	while (end.size() < lists[1].size()) {
		end.push_front(0); // nodes of leading 0's to results list
	}
	na.add(end, lists[0], end); // loads first number into results list
	flag = signs[0];
	for (int i = 1; i < lists.size(); i++) {
		if ((signs[i] && flag) || (!signs[i] && !flag)) {
			na.add(end, lists[i], end);
			if (!signs[i]) {
				sign = "-";
				flag = false;
			}
			else {
				flag = true;
				sign = "";
			}
		}
		else {
			valCheck1 = end.begin();
			valCheck2 = lists[i].begin();
			while (*valCheck1 == *valCheck2 && valCheck1 != end.end()) { // Finds largest different digit
				++valCheck1;
				++valCheck2;
			}
			if (*valCheck1 >= *valCheck2) { // adding values with different signs is subtraction
				na.subtract(end, lists[i], end);
				if (!flag) {
					sign = "-";
				}
			}
			else { 
				if (*valCheck2 > *valCheck1) {
					na.subtract(lists[i], end, end);
					if (!signs[i]) {
						sign = "-";
						flag = false;
					}
				}

			}
		}
	}
	
	it = end.begin(); // Set iterator for addition
	std::cout << "Values added:" << std::endl << sign;
	while (it != end.end()) {
		std::cout << std::setfill('0') << std::setw(3) << *it << std::endl;
		++it;
	}
	end.clear(); // Clears results list
	while (end.size() < lists[1].size()) {
		end.push_front(0); // nodes of leading 0's to results list
	}
	na.add(end, lists[0], end); // loads first number into results list
	flag = signs[0];
	for (int i = 1; i < lists.size(); i++) {
		if ((!flag && signs[i]) || (flag && !signs[i])) { // negative - positve and positve - negative both get further from 0.
			na.add(end, lists[i], end);
			if (!flag) {
				sign = "-";
				flag = false;
			}
			else sign = "";
		}
		else {
			valCheck1 = end.begin();
			valCheck2 = lists[i].begin();
			while (*valCheck1 == *valCheck2 && valCheck1 != end.end()) { // Finds largest different digit
				++valCheck1;
				++valCheck2;
			}
			if (*valCheck1 > *valCheck2) {
				na.subtract(end, lists[i], end);
				if (!flag) {
					sign = "-";
				}
				else {
					sign = "";
				}
			}
			else {
				na.subtract(lists[i], end, end);
				if (flag) {
					sign = "-";
				}
				else {
					sign = "";
				}
			}
		}
	}
	it = end.begin(); // Set iterator for subtraction
	std::cout << "Values subtracted:" << std::endl << sign;
	while (it != end.end()) {
		std::cout << std::setfill('0') << std::setw(3) << *it << std::endl;
		++it;
	}
	end.clear(); // Clear result list
	return 0;
}

