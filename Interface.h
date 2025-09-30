#pragma once
#include <string>
#include <map>

class Multiset;

class Interface
{
private:
	std::map<std::string, Multiset> data_;
	int uniMultiplicity_;
	int bitDepth_;

	bool isValidName(const std::string& name);
	bool canAdd(const std::string& name);
	void createMultisets(bool rand = false);
	void displayMenu();
	void performAll(Multiset& A, Multiset& B);
	void perform(std::string& nameA, std::string& nameB);
	void printMultiset(std::string& name);
	void printNames();
	void printAllMultisets();
	void createRandom();
	void deleteMultiset();
	void reset();

public:
	int run();
};