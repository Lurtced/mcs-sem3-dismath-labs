#include "Interface.h"
#include "Multiset.h"

#include <iostream>
#include <conio.h>
#include <string>

#define CL system("cls")
#define W std::cout << "Press any key to continue..."; trash = _getch()

bool Interface::isValidName(const std::string& name) {
    return data_.find(name) == data_.end() && name != "";
}

bool Interface::canAdd(const std::string& name) {
    if (!isValidName(name)) {
        std::cout << "Multiset with this name already exists or the name is invalid.\n";
        return false;
    }
    return true;
}

void Interface::createMultisets(bool rand) {
	if (data_.size() == 0) { // Here we set the universe parameters
        std::cout << "Enter bit depth: ";
        int depth;
        while (!(std::cin >> depth) || depth < 0) {
            std::cout << "Invalid input. Enter non-negative integer: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }

        bitDepth_ = depth;

        if (bitDepth_ == 0)
			goto AddUniverse;

        Multiset::createUniverse(depth);

        std::cout << "Enter universum multiplicity: ";
        int multi;
        while (!(std::cin >> multi) || multi < 0) {
            std::cout << "Invalid input. Enter non-negative integer: ";
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        Multiset::setMaxMultiplicity(multi);
        uniMultiplicity_ = multi;

    AddUniverse:
        if (uniMultiplicity_ == 0)
			Multiset::createUniverse(0);
        data_["U"] = Multiset::getUniverse();
    }

    if (rand) {
        createRandom();
    }
    else {
        Multiset A;

        std::cout << "Enter name of the multiset: ";
        std::string name;
        std::cin >> name;
        while (!canAdd(name)) {
            std::cout << "The name is already taken or invalid. Please, try again: ";
            std::cin >> name;
        }
        if (bitDepth_ * uniMultiplicity_ == 0) {
            goto AddMultiset;
		}
        std::cout << "\nHow to fill multiset?\n1. Manual\n2. Auto\n> ";
        int choice;
        while (!(std::cin >> choice) || (choice != 1 && choice != 2)) {
            std::cout << "Enter 1 or 2: ";
        }

        int sizeA;
        std::cout << "Enter size of the multiset " << name << ": ";
        while (!(std::cin >> sizeA) || sizeA < 0 || sizeA > bitDepth_ * uniMultiplicity_) {
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

void Interface::performAll(Multiset& A, Multiset& B) {
    std::cout << "\n======= MULTISETS =======\n";
    std::cout << "U: "; A.printU(); std::cout << '\n';
    std::cout << "A: "; A.print(); std::cout << '\n';
    std::cout << "B: "; B.print(); std::cout << '\n';

    std::cout << "\n======= OPERATIONS =======\n";
    std::cout << "A union B: "; A.unionWith(B).print(); std::cout << '\n';
    std::cout << "A inter B: "; A.intersectionWith(B).print(); std::cout << '\n';
    std::cout << "A \\ B: "; A.differenceWith(B).print(); std::cout << '\n';
    std::cout << "B \\ A: "; B.differenceWith(A).print(); std::cout << '\n';
    std::cout << "A /\\ B: "; A.symmetricDifferenceWith(B).print(); std::cout << '\n';
    std::cout << "A + B: "; A.arithmeticSum(B).print(); std::cout << '\n';
	std::cout << "A - B: "; A.arithmeticDifferenceWith(B).print(); std::cout << '\n';
    std::cout << "B - A: "; B.arithmeticDifferenceWith(A).print(); std::cout << '\n';
    std::cout << "A * B: "; A.arithmeticProduct(B).print(); std::cout << '\n';
    std::cout << "A / B: "; A.arithmeticDivision(B).print(); std::cout << '\n';
    std::cout << "B / A: "; B.arithmeticDivision(A).print(); std::cout << '\n';
    std::cout << "Complement of A: "; auto compA = A.complement(); compA.print(); std::cout << '\n';
    std::cout << "Complement of B: "; auto compB = B.complement(); compB.print(); std::cout << '\n';
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
            << "1. A union B\n"
            << "2. A inter B\n"
            << "3. A \\ B\n"
            << "4. B \\ A\n"
            << "5. A /\\ B\n"
            << "6. A + B\n"
            << "7. A - B\n"
            << "8. A * B\n"
            << "9. A / B\n"
            << "0. B / A\n"
            << "d. Do all operations\n"
            << "a. Complement of A\n"
            << "b. Complement of B\n"
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
			performAll(data_[nameA], data_[nameB]);
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
        << "7. Reset\n"
		<< "0. Exit\n> ";
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