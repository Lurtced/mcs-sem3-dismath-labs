#include "Multiset.h"
#include <iostream>
#include <stdexcept>
#include <sstream>

int Multiset::maxMultiplicity;

void Multiset::setMaxMultiplicity(int limit) {
    maxMultiplicity = limit;
}

int Multiset::getMaxMultiplicity() {
    return maxMultiplicity;
}

std::vector<std::string> Multiset::generateGrayCode(int n) {
    if (n <= 0) return {};
    std::vector<std::string> gray;
    int total = 1 << n;

    for (int i = 0; i < total; ++i) {
        int grayCode = i ^ (i >> 1);
        std::string code = "";
        for (int j = n - 1; j >= 0; --j) {
            code += (grayCode & (1 << j)) ? '1' : '0';
        }
        gray.push_back(code);
    }
    return gray;
}

std::vector<std::string> Multiset::universe; // default; will change in Interface

void Multiset::createUniverse(int depth) {
    universe = generateGrayCode(depth);
}

bool Multiset::isValidCode(const std::string& code) const {
    for (const auto& u : universe) {
        if (code == u) return true;
    }
    return false;
}

bool Multiset::canAdd(const std::string& code) const {
	// Check if code is valid and if adding one more would exceed maxMultiplicity
    if (!isValidCode(code)) return false;
    int current = frequency(code);
    return current < maxMultiplicity;
}

int Multiset::frequency(const std::string& code) const {
    auto it = elements.find(code);
    return (it != elements.end()) ? it->second : 0;
}

int Multiset::add(const std::string& code, int count = 1) {
    /* 
    * Adds up to 'count' instances of 'code', but not exceeding maxMultiplicity.
    * Returns the actual number of instances added (which may be less than 'count' or == zero).
    */
    if (!isValidCode(code) || count <= 0) return 0;

    int current = frequency(code);
    int available = maxMultiplicity - current;
    int actualToAdd = std::min(count, available);

    if (actualToAdd > 0) {
        elements[code] += actualToAdd;
    }

    return actualToAdd;
}

void Multiset::fillManual(int size) {
    std::cin.ignore();
    if (size < 0) return;

    if (size != 0)
        std::cout << "Enter " << size << " Gray codes (limit: " << maxMultiplicity << ")\n"
            << "(You can enter either Gray code only or Gray code [space] count; other numbers won't be accounted)" << std::endl;
    
    for (int i = 0; i < size; ) {
        std::string code;
        while (true) {
            std::cout << "Code #" << (i + 1) << ": ";
            
            std::string line;
            std::getline(std::cin, line);
            std::istringstream iss(line);

            std::string code;
            if (!(iss >> code)) {
                std::cout << "Empty input. Try again.\n";
                continue;
            }

            int count = 1;
            iss >> count;

            if (!isValidCode(code)) {
                std::cout << "Invalid code. Valid: ";
                for (const auto& c : universe) std::cout << c << " ";
                std::cout << "\nTry again.\n";
                continue;
            }

            if (!canAdd(code)) {
                std::cout << "You've reached your limit(" << maxMultiplicity 
                          << ") for the code \"" << code << "\". Please pick different.\n";
                continue;
            }

            int currentCount = frequency(code);
            if (currentCount + count > maxMultiplicity) {
                std::cout << "Adding " << count << " of \"" << code
                    << "\" would exceed limit (" << maxMultiplicity
                    << "). Currently have " << currentCount << ".\n"
                    << "Try again with fewer or different code.\n";
                continue;
            }

            for (int j = 0; j < count; ++j) {
                add(code);
            }

            i += count;
            break;
        }
    }
}

void Multiset::fillRandom(int size) {
	/* 
    * Fills the multiset with 'size' random codes from the universe,
	* and ensures not to exceed maxMultiplicity for any code.
    */

    if (size <= 0 || universe.empty()) return;
    
    for (int i = 0; i < size; ++i) {
        std::string code;
        do {
            code = universe[rand() % universe.size()];
		} while (!canAdd(code));

        add(code);
    }
}

int Multiset::size() const {
    int total = 0;
    for (const auto& p : elements) total += p.second;
    return total;
}

Multiset Multiset::unionWith(const Multiset& other) const {
    /*
	* For union, we take the maximum frequency of each element from both multisets,
    * and put it in the temporary multiset 'result'.
    */
    Multiset result;
    result.universe = universe;

    for (const auto& p : elements) {
        if (p.second > 0) {
            result.elements[p.first] = p.second;
        }
    }
    for (const auto& p : other.elements) {
        if (p.second > 0) {
			result.elements[p.first] = std::max(result.elements[p.first], p.second);
        }
    }
    return result;
}

Multiset Multiset::intersectionWith(const Multiset& other) const {
    Multiset result;
    result.universe = universe;

    for (const auto& p : elements) {
        if (other.elements.count(p.first)) {
            result.elements[p.first] = std::min(p.second, other.elements.at(p.first));
        }
    }
    return result;
}

Multiset Multiset::differenceWith(const Multiset& other) const {
	return intersectionWith(other.complement());
}

Multiset Multiset::arithmeticDifferenceWith(const Multiset& other) const {
    Multiset result;
    result.universe = universe;

    for (const auto& p : elements) {
        int count = p.second;
        if (other.elements.count(p.first)) {
            count -= other.elements.at(p.first);
        }
        if (count > 0) {
            result.elements[p.first] = count;
        }
    }
    return result;
}

Multiset Multiset::symmetricDifferenceWith(const Multiset& other) const {
    /*auto diffA = arithmeticDifferenceWith(other);
    auto diffB = other.arithmeticDifferenceWith(*this);*/

    Multiset result;
    result.universe = universe;

    /*for (const auto& p : diffA.elements) result.elements[p.first] = p.second;
    for (const auto& p : diffB.elements) result.elements[p.first] = p.second;*/

    // disunction \ conjunction
	auto uni = unionWith(other);
	auto inter = intersectionWith(other);
	result = uni.differenceWith(inter);

    return result;
}

Multiset Multiset::arithmeticSum(const Multiset& other) const {
    Multiset result;
    result.universe = universe;

    for (const auto& p : elements) result.elements[p.first] += p.second;
    for (const auto& p : other.elements) result.elements[p.first] += p.second;

    return result;
}

Multiset Multiset::arithmeticProduct(const Multiset& other) const {
    Multiset result;
    result.universe = universe;

    for (const auto& p : elements) {
        if (other.elements.count(p.first)) {
            result.elements[p.first] = std::min(p.second * other.elements.at(p.first), maxMultiplicity);
        }
    }

    return result;
}

Multiset Multiset::arithmeticDivision(const Multiset& other) const {
    Multiset result;
    result.universe = universe;

    for (const auto& p : elements)
        if (other.elements.count(p.first) && other.elements.at(p.first) > 0)
			if (p.second / other.elements.at(p.first) > 0)
                result.elements[p.first] = p.second / other.elements.at(p.first);

    return result;
}

Multiset Multiset::complement() const {
    Multiset result;

    for (const auto& code : universe) {
        int current = frequency(code);
        int needed = maxMultiplicity - current;
        if (needed > 0) {
            result.elements[code] = needed;
        }
    }

    return result;
}

void Multiset::print() const {
    std::cout << "{ ";
    bool first = true;
    for (const auto& p : elements) {
        if (!first) std::cout << ", ";
        std::cout << "\"" << p.first << "\"^" << p.second;
        first = false;
    }
    std::cout << "}" << std::endl;
}

void Multiset::printU() const {
    std::cout << "{ ";
    bool first = true;
    for (const auto& p : universe) {
        if (!first) std::cout << ", ";
        std::cout << "\"" << p << "\"^" << maxMultiplicity;
        first = false;
    }
    std::cout << "}" << std::endl;
}

Multiset Multiset::getUniverse() {
    Multiset uni;
    for (const auto& elem : universe) {
        uni.elements[elem] = maxMultiplicity;
    }
    return uni;
}