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

        remove(filename.c_str());
        if (rename(temp_name.c_str(), filename.c_str()) != 0) {
            std::cout << "Uudelleen nimeäminen ei onnistunut!" << std::endl;
            retval = 3;
        }

    }
    return retval;
}

int update_file(std::string* fname) {
    std::ifstream file_in;
    std::ofstream file_out;
    std::string name, filename_old = *fname, details, line, word, input;
    std::string::size_type st;
    std::string filename_new = "animals_temp.csv";
    std::vector<std::string> csv_row;
    int retval = 0, choice, found = 0;
    char cont;

    std::cout << "Kenen tietoja haluat päivittää?" << std::endl;
    std::getline(std::cin, name);
    
    /*Open two files, the old or current "main" data file
      and the new file which will contain the updated data*/
    file_in.open(filename_old, std::fstream::in);
    file_out.open(filename_new, std::fstream::out | std::fstream::app);

    if (!file_out.is_open()) {
        std::cout << "Alkuperäistä tiedostoa ei voitu avata!" << std::endl;
        retval = -1;
    }else if(!file_in.is_open()) {
        std::cout << "Väliaikaista tiedostoa ei voitu avata!";
        retval = 1;
    }else {
        while (std::getline(file_in, line)) {
            csv_row.clear();
            std::stringstream str_s(line);

            while (std::getline(str_s, word, ',')) {
                csv_row.push_back(word);
            }
            if (name != csv_row[0]) { //If the given name does not match the name of the animal in this current line, write it to the new file
                file_out << line << '\n';
            }
            else {
                found = 1;
                std::cout << "********Eläimen nykyiset tiedot********" << std::endl;
                std::cout << "Nimi: " << csv_row[0] << std::endl;
                std::cout << "Syntymäaika: " << csv_row[1] << std::endl;
                std::cout << "Laji: " << csv_row[2] << std::endl;
                std::cout << "Väri: " << csv_row[3] << std::endl;
                std::cout << "Sukupuoli: " << csv_row[4] << std::endl;
                std::cout << "Status: " << csv_row[5] << std::endl;
                do {
                    std::cout << "Mitä haluat päivittää?" << std::endl;
                    std::cout << "1. Nimi" << std::endl << "2. Syntymäaika" << std::endl <<
                        "3. Laji" << std::endl << "4. Väri" << std::endl << "5. Sukupuoli" <<
                        std::endl << "6. Status" << std::endl;

                    std::cin >> choice;
                    switch (choice) {
                    case 1:
                        std::cin.ignore();
                        std::cout << "Anna uusi nimi: ";
                        std::getline(std::cin, details);
                        break;
                    case 2:
                        std::cin.ignore();
                        std::cout << "Anna uusi syntymäaika: ";
                        std::getline(std::cin, details);
                        break;
                    case 3:
                        std::cin.ignore();
                        std::cout << "Anna uusi laji: ";
                        std::getline(std::cin, details);
                        break;
                    case 4:
                        std::cin.ignore();
                        std::cout << "Anna uusi väri: ";
                        std::getline(std::cin, details);
                        break;
                    case 5:
                        std::cin.ignore();
                        std::cout << "Anna uusi sukupuoli: ";
                        std::getline(std::cin, details);
                        break;
                    case 6:
                        std::cin.ignore();
                        std::cout << "Anna uusi status: ";
                        std::getline(std::cin, details);
                        break;
                    }
                    csv_row[choice - 1] = details;
                    std::cout << "Päivitetäänkö lisää? (K/E)";
                    cont = std::getchar();
                } while (cont != 'E' && cont != 'e');

                //Write the updated data to the new file
                for (int i = 0; i < csv_row.size() - 1; i++) {
                    file_out << csv_row[i] << ',';
                }
                file_out << csv_row[csv_row.size() - 1] << '\n';
            }
        }
    }
    if (found == 0){
        std::cout << "Nimellä ei löytynyt yhtään eläintä!" << std::endl;
    }
    file_in.close();
    file_out.close();

    remove(filename_old.c_str()); //Remove the old copy of the file
    if (rename(filename_new.c_str(), filename_old.c_str()) != 0) {  //Rename the newly made file with the name of the "old" file
        std::cout << "Uudelleen nimeäminen ei onnistunut!";
        retval = 3;
    };
    std::cin.ignore();
    return retval;
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