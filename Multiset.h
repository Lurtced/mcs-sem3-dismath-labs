#include <string>
#include <map>
#include <vector>

class Multiset {
private:
    std::map<std::string, int> elements; // Multiset
    static std::vector<std::string> universe; // universum
    static int maxMultiplicity; // universum's multiplicity
    
public:
    // setting up universum and the multisets
    static std::vector<std::string> generateGrayCode(int n);
    static void createUniverse(int depth);
    static void setMaxMultiplicity(int limit);
    static int getMaxMultiplicity();
    int size() const;
    void fillManual(int size);
    void fillRandom(int size);

    // Modifying multisets methods
    int frequency(const std::string& code) const;
    bool canAdd(const std::string& code) const;
    bool isValidCode(const std::string& code) const;
    int add(const std::string& code, int count);

    // Operations with multisets
    Multiset unionWith(const Multiset& other) const;
    Multiset intersectionWith(const Multiset& other) const;
	Multiset differenceWith(const Multiset& other) const;
    Multiset arithmeticDifferenceWith(const Multiset& other) const;
    Multiset symmetricDifferenceWith(const Multiset& other) const;
    Multiset arithmeticSum(const Multiset& other) const;
    Multiset arithmeticProduct(const Multiset& other) const;
    Multiset arithmeticDivision(const Multiset& other) const;
    Multiset complement() const;

    // Printing
    void print() const;
    void printU() const;
    static Multiset getUniverse();
};