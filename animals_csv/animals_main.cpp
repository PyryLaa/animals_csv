#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int read_file();
int delete_from_file();
int update_file();
int add_to_file();
int menu();

int main(void) {
    setlocale(LC_ALL, "fi-FI"); //Use scandinavian vowels in std::cout

    std::string input;
    int choice, success;
    do {
        choice = menu();
        switch (choice) {
        case 1:
            success = read_file();
            break;
        case 2:
            success = delete_from_file();
            break;
        case 3:
            success = update_file();
            break;
        case 4:
            success = add_to_file();
            break;
        default:
            break;
        }
        std::cout << "Haluatko jatkaa? (K/E): " << std::endl;
        std::cin >> input;
        std::cin.ignore();
    } while (input == "K" || input == "k");
    return 0;
}


int menu() {
    std::string input;
    int retval;
    std::string::size_type st;
    std::cout << "Mitä haluat tehdä?" << std::endl;
    std::cout << "1. Näytä eläin" << std::endl;
    std::cout << "2. Poista eläin" << std::endl;
    std::cout << "3. Päivitä eläin" << std::endl;
    std::cout << "4. Lisää eläin" << std::endl;


    std::getline(std::cin, input);
    while (std::cin.fail() || std::cin.eof() || input.find_first_not_of("0123456789") != std::string::npos) {
        std::cout << "Väärä syöte, anna numero 1-4: " << std::endl;

        if (input.find_first_not_of("0123456789") == std::string::npos) {
            std::cin.clear();
            std::cin.ignore(256, '\n');
        }
        std::getline(std::cin, input);
    }


    retval = std::stoi(input, &st);

    if (retval < 1 || retval > 4) {
        std::cout << "Virheellinen numero, anna numero 1-4: " << std::endl;
        std::cin.clear();
        std::getline(std::cin, input);
    }

    retval = std::stoi(input, &st);
    return retval;
}

int read_file() {
    std::vector<std::string> csv_row;
    std::fstream file_in;
    std::string temp_str, row, word;

    int retval = 0;

    file_in.open("animals.csv", std::fstream::in);

    if (!file_in.is_open()) {
        std::cout << "File could not be opened" << std::endl;
        retval = -1;
    }
    else {
        while (std::getline(file_in, row)) {
            csv_row.clear();


            std::stringstream str_s(row);

            while (std::getline(str_s, word, ',')) {
                csv_row.push_back(word);
            }
            std::cout << std::endl;
            std::cout << "Nimi: " << csv_row[0] << std::endl;
            std::cout << "Syntymäaika: " << csv_row[1] << std::endl;
            std::cout << "Laji: " << csv_row[2] << std::endl;
            std::cout << "Väri: " << csv_row[3] << std::endl;
            std::cout << "Sukupuoli: " << csv_row[4] << std::endl;
            std::cout << "Status: " << csv_row[5] << std::endl;

        }
        retval = 0;
    }

    file_in.close();
    return retval;
}

int delete_from_file() {
    std::cout << "Deleting from file" << std::endl;
    return 0;
}

int update_file() {
    std::cout << "Updating file" << std::endl;
    return 0;
}

int add_to_file() {
    std::ofstream file_out;
    std::string name, age, species, colour, gender, status;
    int retval;

    file_out.open("animals.csv", std::fstream::out | std::fstream::app);

    if (!file_out.is_open()) {
        std::cout << "File could not be opened" << std::endl;
        retval = -1;
    }
    else {
        std::cout << "Anna eläimen tiedot seuraavassa järjestyksessä välilyönnillä eroteltuna: " << std::endl;
        std::cout << "Nimi, syntymäaika, laji, väri, sukupuoli, status(esim. lemmikki/elossa)" << std::endl;
        std::cin >> name >> age >> species >> colour >> gender >> status;

        file_out << name << ", " << age << ", " << species << ", " << colour << ", " <<
            gender << ", " << status << '\n';
        retval = 0;
    }
    return retval;
}