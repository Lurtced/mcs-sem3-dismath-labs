#include "Interface.h"
#include "Multiset.h"

#include <iostream>
#include <conio.h>
#include <string>

#define CL system("cls")
#define W std::cout << "Press any key to continue..."; trash = _getch()
#define IGN std::cin.clear(); std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

bool Interface::isValidName(const std::string& name) {
    bool isValid = data_.find(name) == data_.end() && name != "";
    if (isValid)
        return true;
    else
        std::cout << "Multiset with this name already exists or name is invalid.\n"
            << "Please, try again: ";
    return false;
}

void Interface::createMultisets(bool rand) {
	if (data_.size() == 0) { // Here we set the universe parameters
        std::cout << "Enter bit depth: ";
        int depth;
        while (!(std::cin >> depth) || depth < 0) {
            std::cout << "Invalid input. Enter non-negative integer: ";
            IGN
        }

        bitDepth_ = depth;

        if (bitDepth_ == 0)
			goto AddUniverse;

        Multiset::createUniverse(depth);

        std::cout << "Enter universum multiplicity: ";
        int multi;
        IGN
        while (!(std::cin >> multi) || multi < 0) {
            std::cout << "Invalid input. Enter non-negative integer: ";
            IGN
        }
        Multiset::setMaxMultiplicity(multi);
        uniMultiplicity_ = multi;

    AddUniverse:
        if (uniMultiplicity_ == 0)
			Multiset::createUniverse(0);
        data_["U"] = Multiset::getUniverse();
		uniSize_ = Multiset::getUniverse().size();
    }

    if (rand) {
        createRandom();
    }
    else {
        Multiset A;

        std::cout << "Enter name of the multiset: ";
        std::string name;
        std::cin >> name;
        while (!isValidName(name)) {
            IGN
            std::cin >> name;
        }
        if (bitDepth_ * uniMultiplicity_ == 0) {
            goto AddMultiset;
		}
        std::cout << "\nHow to fill multiset?\n1. Manual\n2. Auto\n> ";
        int choice;
        IGN
        while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
            IGN
            std::cout << "Enter 1 or 2: ";
        }

        int sizeA;
        std::cout << "Enter size of the multiset " << name << ": ";
        IGN
        while (!(std::cin >> sizeA) || sizeA < 0 || sizeA > uniSize_) {
            IGN
            std::cout << "Size can't be negative or more than universe's size: ";
        }

        if (choice == 1) {
            std::cout << "Fill Multiset " << name << ':' << std::endl;
            A.fillManual(sizeA);
        }
        else {
            A.fillRandom(sizeA);
        }
        AddMultiset:
        data_[name] = A;
    }
}

void Interface::performAll(Multiset& A, Multiset& B, std::string& nameA, std::string& nameB) {
    std::cout << "\n======= MULTISETS =======\n";
    std::cout << "U: "; A.printU(); std::cout << '\n';
	std::cout << nameA << ": "; A.print(); std::cout << '\n';
	std::cout << nameB << ": "; B.print(); std::cout << '\n';
	std::cout << "\n======= OPERATIONS =======\n";
	std::cout << nameA << " union " << nameB << ": "; A.unionWith(B).print(); std::cout << '\n';
	std::cout << nameA << " inter " << nameB << ": "; A.intersectionWith(B).print(); std::cout << '\n';
	std::cout << nameA << " \\ " << nameB << ": "; A.differenceWith(B).print(); std::cout << '\n';
	std::cout << nameB << " \\ " << nameA << ": "; B.differenceWith(A).print(); std::cout << '\n';
	std::cout << nameA << " /\\ " << nameB << ": "; A.symmetricDifferenceWith(B).print(); std::cout << '\n';
	std::cout << nameA << " + " << nameB << ": "; A.arithmeticSum(B).print(); std::cout << '\n';
	std::cout << nameA << " - " << nameB << ": "; A.arithmeticDifferenceWith(B).print(); std::cout << '\n';
	std::cout << nameB << " - " << nameA << ": "; B.arithmeticDifferenceWith(A).print(); std::cout << '\n';
	std::cout << nameA << " * " << nameB << ": "; A.arithmeticProduct(B).print(); std::cout << '\n';
	std::cout << nameA << " / " << nameB << ": "; A.arithmeticDivision(B).print(); std::cout << '\n';
	std::cout << nameB << " / " << nameA << ": "; B.arithmeticDivision(A).print(); std::cout << '\n';
	std::cout << "Complement of " << nameA << ": "; auto compA = A.complement(); compA.print(); std::cout << '\n';
	std::cout << "Complement of " << nameB << ": "; auto compB = B.complement(); compB.print(); std::cout << '\n';
}

void Interface::perform(std::string& nameA, std::string& nameB) {
    std::cout << "Enter the name of the 1st multiset: ";
    std::cin >> nameA;
    while (isValidName(nameA)) {
        std::cout << "Multiset with this name doesn't exist. Please, try again: ";
        std::cin >> nameA;
    }
    std::cout << "Enter the name of the 2nd multiset: ";
    std::cin >> nameB;
    while (isValidName(nameB)) {
        std::cout << "Multiset with this name doesn't exist. Please, try again: ";
        std::cin >> nameB;
    }

    Multiset compA;
	Multiset compB;
    int trash;
    int choice;

    while (true) {
        CL;
        std::cout << "Choose an operation:\n"
            << "1. '" << nameA << "' union '" << nameB << "'\n"
			<< "2. '" << nameB << "' inter '" << nameB << "'\n"
			<< "3. '" << nameA << "' \\ '" << nameB << "'\n"
			<< "4. '" << nameB << "' \\ '" << nameA << "'\n"
			<< "5. '" << nameA << "' /\\ '" << nameB << "'\n"
			<< "6. '" << nameA << "' + '" << nameB << "'\n"
			<< "7. '" << nameA << "' - '" << nameB << "'\n"
			<< "8. '" << nameA << "' * '" << nameB << "'\n"
			<< "9. '" << nameA << "' / '" << nameB << "'\n"
			<< "0. '" << nameB << "' / '" << nameA << "'\n"
			<< "d. Do all operations\n"
			<< "a. Complement of '" << nameA << "'\n"
			<< "b. Complement of '" << nameB << "'\n"
            << "q. Go to main menu\n>";

        choice = _getch();

        switch (choice) {
        case '1':
            std::cout << "A union B: "; data_[nameA].unionWith(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
        case '2':
            std::cout << "A inter B: "; data_[nameA].intersectionWith(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
        case '3':
            std::cout << "A \\ B: "; data_[nameA].arithmeticDifferenceWith(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
        case '4':
            std::cout << "B \\ A: "; data_[nameB].arithmeticDifferenceWith(data_[nameA]).print(); std::cout << '\n'; 
            W;
            break;
        case '5':
            std::cout << "A sym \\ B: "; data_[nameA].symmetricDifferenceWith(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
        case '6':
            std::cout << "A + B: "; data_[nameA].arithmeticSum(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
		case '7':
            std::cout << "A - B: "; data_[nameA].differenceWith(data_[nameB]).print(); std::cout << '\n';
            W;
			break;
        case '8':
            std::cout << "A * B: "; data_[nameA].arithmeticProduct(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
        case '9':
            std::cout << "A / B: "; data_[nameA].arithmeticDivision(data_[nameB]).print(); std::cout << '\n';
            W;
            break;
        case '0':
            std::cout << "B / A: "; data_[nameB].arithmeticDivision(data_[nameA]).print(); std::cout << '\n';
            W;
            break;
        case 'd':
			performAll(data_[nameA], data_[nameB], nameA, nameB);
            W;
            break;
        case 'a':
            std::cout << "Complement of A: "; compA = data_[nameA].complement(); compA.print(); std::cout << '\n';
            W;
            break;
        case 'b':
            std::cout << "Complement of B: "; compB = data_[nameB].complement(); compB.print(); std::cout << '\n';
            W;
            break;
        case 'q':
            goto Break;
        default:
            std::cout << "Invalid option. Please, try again!\n";
            W;
        }
        continue;
    Break:
        break;
    }
}

void Interface::printMultiset(std::string& name) {
    data_[name].print();
}

void Interface::printNames() {
    int i = 1;
    for (const auto& pair : data_) {
        std::cout << "#" << i << ": " << pair.first << std::endl;
        i++;
    }
}

void Interface::printAllMultisets() {
    int i = 1;
    for (const auto& pair : data_) {
        std::cout << '#' << i << " \"" << pair.first << "\": ";
        pair.second.print();
        i++;
    }
}

void Interface::createRandom() {
    std::string name;
    int i = 10000;
    do { name = std::to_string(rand() % i++); } while (!isValidName(name));
    Multiset A;
    i = 100;
    int size = rand() % i;
    while (size > uniMultiplicity_ * bitDepth_)
        size--;
    A.fillRandom(size);
    data_[name] = A;
    std::cout << "Random multiset created.\n"
        << "Name: " << name
        << "\nSize: " << A.size() << std::endl;
}

void Interface::displayMenu() {
    std::cout << "Choose an option:\n"
        << "1. Create new multisets\n"
        << "2. Perform an operation on existing multisets\n"
        << "3. Print a multiset\n"
        << "4. Print all multisets' names\n"
        << "5. Print all multisets\n"
        << "6. Create a random multiset\n"
        << "7. Delete a multiset\n"
		<< "8. Reset (delete all multisets and universe)\n"
		<< "0. Exit\n> ";
}

void Interface::deleteMultiset() {
    std::string name;
    std::cout << "Enter the name of the multiset to delete: ";
    std::cin >> name;
    while (isValidName(name) || name == "U") { // Checks if it DOESN'T exist yet.
        if (isValidName(name))
            std::cout << "Multiset with this name doesn't exist. Please, try again: ";
        else
			std::cout << "Can't delete universe. Please, try again: ";
        std::cin >> name;
    }
    data_.erase(name);
	std::cout << "Deleted." << std::endl;
}

void Interface::reset() {
    data_.clear();
    bitDepth_ = 0;
    uniMultiplicity_ = 0;
}

int Interface::run() {
    int trash;
    std::string nameA;
    std::string nameB;

    std::cout << "Hello!\n"
        << "Here you can create multisets and perform operations upon them.\n"
        << "Press any key to continue.";
    trash = _getch();

    while (true) {
        CL;
        displayMenu();
        Again:
        char option = _getch();
        switch (option) {
        case '1':
            CL;
            createMultisets();
			std::cout << "Press any key to continue.";
            trash = _getch();
			break;

        case '2':
            CL;
            if (data_.size() < 2) {
                std::cout << "You need at least two multisets to perform operations.\n"
                          << "Press any key to continue.";
                trash = _getch();
                break;
			}
            perform(nameA, nameB);
            break;

        case '3':
            CL;
            if (data_.size() == 0) {
                std::cout << "Nothing to print. No multisets available." << std::endl;
                W;
                break;
            }
            std::cout << "Enter the name of a multiset to print: ";
            std::cin >> nameA;
            while (isValidName(nameA)) { // Checks if it DOESN'T exist yet.
                std::cout << "Wrong name. Please try again: ";
                std::cin >> nameA;
            }
            printMultiset(nameA);
            W;
            break;

        case '4':
            CL;
            if (data_.size() == 0) {
                std::cout << "Nothing to print. No multisets available." << std::endl;
                W;
                break;
            }
            printNames();
            W;
            break;

        case '5':
            CL;
            if (data_.size() == 0) {
                std::cout << "Nothing to print. No multisets available." << std::endl;
                W;
                break;
            }
            printAllMultisets();
            W;
            break;

        case '6':
            CL;
            if (data_.size() == 0)
                createMultisets(true);
            else
                createRandom();
            W;
            break;

        case '7':
            CL;
            if (data_.size() == 0) {
                std::cout << "Nothing to delete. No multisets available." << std::endl;
                W;
                break;
            }
            deleteMultiset();
            W;
			break;

        case '8':
            CL;
            reset();
            std::cout << "Resetted." << std::endl;
            W;
            break;

        case '0':
            return 0;

        default:
			std::cout << "Invalid option. Please, try again!\n";
            goto Again;
        }
    }
}