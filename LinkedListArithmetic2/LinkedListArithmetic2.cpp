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
	void populateList(int x, std::list<int> &numbers)
	{
		if (x >= 1000 || x <= -1000) {
			populateList(x / 1000, numbers); // Recursive function to grab chunks of 3 digits
		}
		digit = x % 1000;
		numbers.push_back(digit);
	}
	// Adds values two lists, maintaining three digits per node
	void add(std::list<int> &val1, std::list<int> &val2, std::list<int> &end) {
		std::list<int>::reverse_iterator rit1 = val1.rbegin();
		std::list<int>::reverse_iterator rit2 = val2.rbegin();
		int carry = 0;
		int sum;
		while (rit1 != val1.rend()) {
			sum = *rit1 + *rit2 + carry;
			carry = 0;
			if (sum / 1000 == 1) { // Carries values to keep nodes at 3 digits
				carry = 1;
				sum -= 1000;
			}
			end.push_front(sum);
			++rit1;
			++rit2;
			}
		
	}
	// Subtracts values in two lists, maintaining three digits per node 
	void subtract(std::list<int> &val1, std::list<int> &val2, std::list<int> &end) {
		std::list<int>::reverse_iterator rit1 = val1.rbegin();
		std::list<int>::reverse_iterator rit2 = val2.rbegin();
		bool flag = false; // marks when a value was borrowed against last cycle
		while (rit1 != val1.rend()) {
			if  ((*rit1 >= *rit2 && !flag) || (*rit1 - 1) >= *rit2) {
				if (!flag) {
					end.push_front(*rit1 - *rit2);
				}
				else {
						end.push_front(*rit1 - 1 - *rit2); // "pays back" lent value
						flag = false;
					}
			}
			else { // If first value is too small borrows against higher value in next node
				if (!flag) {
					end.push_front(*rit1 + 1000 - *rit2);
					flag = true;
				}
				
				else {
					end.push_front(*rit1 + 999 - *rit2); // pays back by only borrowing 999
				}
			}
			++rit1;
			++rit2;
		}
	}

	
	// Takes linked list with nodes of 3 digits, changes to nodes of one digit
	void singlify(std::list<int> &in, std::list<int> &out) {
		std::list<int>::reverse_iterator rit = in.rbegin();
		int digits;
		while (rit != in.rend()) {
			digits = *rit;
			for (int i = 0; i < 3; i++) {
				out.push_front(digits % 10);
				digits /= 10;
			}
			++rit;
		}
	}



	/* NOT IN USE
	// Multiplies values in two lists, maintaining three digits per node
	void multiply(std::list<int> &val1, std::list<int> &val2, std::list<int> &end) {
		std::list<int> single1;
		std::list<int> single2;
		std::list<int> temp;
		singlify(val1, single1); // must shift into single digits or multiplication is a mess
		singlify(val2, single2);
		std::list<int>::reverse_iterator rit1 = single1.rbegin();
		std::list<int>::reverse_iterator rit2 = single2.rbegin();
		std::list<int>::iterator it;
		int carry = 0;
		int multiplier = 1;
		int multiplierCount = 1;
		int tempDigit = 0;
		int partialProduct;
		int productSums = 0;
		int i = 0;
		while (rit1 != single1.rend()) {
			while (rit2 != single2.rend()) {
				partialProduct = ((*rit1) *  (*rit2) + carry) * multiplier;
				carry = partialProduct / (10 * multiplier);
				if (carry != 0) { // Carries values to keep nodes at 1 digit
					partialProduct -= carry * (10 * multiplier);
				}
				++rit2;
				multiplier *= 10; // Think long multiplication by hand
				productSums += partialProduct;
			}
			multiplier = pow(10, multiplierCount);
			rit2 = single2.rbegin();
			++rit1;
		}
		productSums += carry;
		populateList(productSums, end);
		/*
		it = temp.begin();
		while (it != temp.end() && *it == 0) { // Removes nodes that are just 0's
			it++;
			temp.pop_front();
		}
		rit1 = temp.rbegin();
		while (rit1 != temp.rend()) {
			for (int i = 0; i < 3; i++) {
				tempDigit += *rit1 * pow(10, i);
				++rit1;
			}
			end.push_front(tempDigit);
		}
		if (carry > 1) {
			populateList(carry, extra);
		}
		// INNER COMMENT OUT GOES HERE
	}
	**/
};


int main()
{
	std::list<int> first;
	std::list<int> second;
	std::list<int> end;
	std::list<int> nums;
	std::vector<bool> signs;
	std::vector<std::list<int>> lists;
	std::list<int>::iterator it;
	std::list<int>::iterator it2;
	bool flag;
	nums.push_back(3453);
	nums.push_back(6454);
	nums.push_back(-542);
	nums.push_back(47842124);
	it = nums.begin();
	NodeArithmetic na;
	while (it != nums.end()) {
		na.populateList(abs(*it), first);
		lists.push_back(first);
		first.clear();
		if (*it < 0) {
			flag = false;
		}
		else {
			flag = true;
		}
		signs.push_back(flag);
		++it;
	}
	int firstNum = 454;
	int firstAbs = abs(firstNum);
	int secondNum = 665;
	int secondAbs = abs(secondNum);
	int larger;
	int smaller;
	bool flag1 = true; 
	bool flag2 = true;
	std::string sign = ""; // holds sign for negative results
	//Determines if any numbers are negative
	if (firstNum < 0) {
		flag1 = false;
	}
	if (secondNum < 0) {
		flag2 = false;
	}
	na.populateList(firstAbs, first);
	na.populateList(secondAbs, second);
	while (first.size() < second.size()) {
		first.push_front(0);
	}
	while (second.size() < first.size()) {
		second.push_front(0);
	}
	if ((flag1 && flag2) || (!flag1 && !flag2)) {
		na.add(first, second, end);
		if (!flag1) {
			sign = "-";
		}
	}
	else {
		if (abs(firstNum) >= abs(secondNum)) {
			na.subtract(first, second, end);
			if (!flag1) {
				sign = "-";
			}
		}	
		else { // adding values with different signs is subtraction
			if (secondAbs > firstAbs) {
				na.subtract(second, first, end);
				if (!flag2) {
					sign = "-";
				}
			}
			else {
				na.subtract(first, second, end);
				if (!flag1) {
					sign = "-";
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
	if ((!flag1 && flag2) || (flag1 && !flag2)) { // negative - positve and positve - negative both get further from 0.
		na.add(first, second, end);
		if (!flag1) {
			sign = "-";   
		}
		else sign = "";
	}
	else {
		if (firstAbs > secondAbs) {
			na.subtract(first, second, end);
			if (!flag1) {
				sign = "-";
			}
			else {
				sign = "";
			}
		}
		else {
			na.subtract(second, first, end);
			if (flag1) {
				sign = "-";
			}
			else {
				sign = "";
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
	/*
	na.multiply(first, second, end);
	if (flag1 != flag2) {
		sign = "-";
	}
	else {
		sign = "";
	}
	it = end.begin(); // Set iterator for multiplication
	std::cout << "Values multiplied:" << std::endl << sign;
	while (it != end.end()) {
		std::cout << std::setfill('0') << std::setw(3) << *it << std::endl;
		++it;
	}
	*/
	return 0;
}

