#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

int read_file(std::string* fname);
int delete_from_file(std::string* fname);
int update_file(std::string* fname);
int add_to_file(std::string* fname);
int menu();

int main(void) {
    setlocale(LC_ALL, "fi-FI");                                                         //Use scandinavian vowels in std::cout
    std::string filename = "animals.csv";                                               //Filename of the file to be used as the data storage
    int choice, success;
    do {
        choice = menu();
        switch (choice) {
        case 1:
            success = read_file(&filename);
            break;
        case 2:
            success = delete_from_file(&filename);
            break;
        case 3:
            success = update_file(&filename);
            break;
        case 4:
            success = add_to_file(&filename);
            break;
        case 5:
            std::cout << "Näkemiin!";
            break;
        default:
            break;
        }
    } while (choice != 5);
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
    std::cout << "5. Poistu" << std::endl;


    std::getline(std::cin, input);
    while (std::cin.fail() || std::cin.eof() || input.find_first_not_of("0123456789") != std::string::npos) {
        std::cout << "Väärä syöte, anna numero 1-5: " << std::endl;

        if (input.find_first_not_of("0123456789") == std::string::npos) {
            std::cin.clear();
            std::cin.ignore(256, '\n');
        }
        std::getline(std::cin, input);
    }


    retval = std::stoi(input, &st);

    if (retval < 1 || retval > 5) {
        std::cout << "Virheellinen numero, anna numero 1-5: " << std::endl;
        std::cin.clear();
        std::getline(std::cin, input);
    }

    retval = std::stoi(input, &st);
    return retval;
}

int read_file(std::string* fname) {

    std::string filename = *fname;
    std::vector<std::string> csv_row;
    std::fstream file_in;
    std::string temp_str, row, word;

    int retval = 0;

    file_in.open(filename, std::fstream::in);

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
            std::cout << "**********************************" << std::endl;
            std::cout << "Nimi: " << csv_row[0] << std::endl;
            std::cout << "Syntymäaika: " << csv_row[1] << std::endl;
            std::cout << "Laji: " << csv_row[2] << std::endl;
            std::cout << "Väri: " << csv_row[3] << std::endl;
            std::cout << "Sukupuoli: " << csv_row[4] << std::endl;
            std::cout << "Status: " << csv_row[5] << std::endl;
            std::cout << "**********************************" << std::endl;
            std::cout << std::endl;
        }
        retval = 0;
    }

    file_in.close();
    return retval;
}

int delete_from_file(std::string* fname) {

    /*Delete animal from file, search by name, 
    copy all lines with not matching name to temp file 
    then rename the temp file as the original */

    std::string filename = *fname;
    std::string temp_name = "animals_temp.csv";                                         //Temporary file to copy all lines that will not be deleted

    int retval = 0;
    std::string name, row, word;
    std::vector<std::string> csv_row;

    std::ifstream file_in(filename, std::fstream::in);
    std::ofstream temp_file(temp_name, std::fstream::out | std::fstream::app);

    if (!file_in.is_open()) {
        std::cout << "Luettavaa tiedostoa ei voitu avata!" << std::endl;
        retval = 1;
    }
    else if(!temp_file.is_open()) {
        std::cout << "Väliaikaista tiedostoa ei voitu avata!" << std::endl;
        retval = 2;
    }
    else {
        std::cout << "Kenet haluat poistaa?: ";
        std::getline(std::cin, name);
        std::cout << "Poistetaan: " << name << std::endl;

        while (std::getline(file_in, row)) {
            csv_row.clear();

            std::stringstream str_s(row);

            while (std::getline(str_s, word, ',')) {
                csv_row.push_back(word);
            }
            if (name != csv_row[0]) {
                temp_file << row << '\n';
            }
        }
        
        file_in.close();                                                                //Close first to make renaming possible
        temp_file.close();

        //const char* c_filename = filename.c_str();
        //const char* c_temp_file = temp_name.c_str();

        remove(filename.c_str());
        if (rename(temp_name.c_str(), filename.c_str()) != 0) {
            std::cout << "Uudelleen nimeäminen ei onnistunut!" << std::endl;
            retval = 3;
        }

    }

    return retval;
}

int update_file(std::string* fname) {
    std::cout << "Updating file" << std::endl;
    return 0;
}

int add_to_file(std::string* fname) {
    
    std::ofstream file_out;
    std::string name, age, species, colour, gender, status;
    std::string filename = *fname;
    int retval;

    file_out.open(filename, std::fstream::out | std::fstream::app);

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
        std::cin.ignore();                                                          //Have to ignore so the next iteration of menu() does not break
    }
    
    file_out.close();
    return retval;
}