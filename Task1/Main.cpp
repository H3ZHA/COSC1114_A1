#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <wait.h>

bool *Graceful_Exit;
std::deque<std::string> TaskFilter(std::string input_file);
void write_file(std::deque<std::string> deque, std::string output_file);

int main(int argc, char **argv) {
    // create global variable
    Graceful_Exit = (bool *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if(Graceful_Exit == MAP_FAILED){
        std::cerr << "mmap fail!" << std::endl;
        exit(0);
    }
    *Graceful_Exit = false;

    // create fork
    pid_t main_fork = fork();
    if(main_fork < 0){
        std::cerr << "Create fork fail!" << std::endl;
        exit(0);
    }
    // parent
    if(main_fork != 0){
        std::string input_file_name;
        std::string output_file_name;

        std::cout << "Checking input file name..." << std::endl;

        // push to next argument(frist is exe program name)
        ++argv;
        if(*argv == NULL){
            std::cerr << "Input file name is empty!" << std::endl;
            return 0;
        }
        else{
            input_file_name = *argv;
            std::cout << "Input file name exist, checking output file name..." << std::endl;
        }
        
        // push to next argument
        ++argv;
        if(*argv == NULL){
            std::cerr << "OUtput file name is empty!" << std::endl;
            return 0;
        }
        else{
            output_file_name = *argv;
        }

        std::deque<std::string> output_data;

        output_data = TaskFilter(input_file_name);

        if(*Graceful_Exit == false){
            write_file(output_data, output_file_name);

            // stop sleep countdown
            kill(main_fork, SIGKILL);
        }
    }
    // child
    else{
        // sleep 30s
        sleep(30);
        *Graceful_Exit = true;
    }

    wait(NULL);
    return 0;
}

std::deque<std::string> TaskFilter(std::string input_file){
    std::cout << "Output file name exist, loading file..." << std::endl;
    std::deque<std::string> filtered;
    std::deque<std::string> filtered_sorted;
    std::ifstream file;
    char filename[input_file.length()];
    std::strcpy(filename, input_file.c_str());
    file.open(filename);

    // filter file
    if(file.is_open()){
        std::cout << "File exist, start filtering data..." << std::endl;

        // read line until end of file
        bool data_end = false;
        while(data_end == false && *Graceful_Exit == false){
            try{
                std::string temp = "";
                getline(file, temp);
                if(temp == ""){
                    data_end = true;
                }
                else{
                    // filter out the words with length less than 3 or more than 15
                    if(temp.length() >= 3 && temp.length() <= 15){
                        filtered.push_back(temp);
                    }
                }
            }
            catch(...){
                std::cerr << "Data load error!" << std::endl;
                data_end = true;
            }
        }
    }
    else{
        std::cerr << "File doesn't exist!" << std::endl;
        exit(0);
    }

    std::cout << "Data filtered done! Sorting by third character..." << std::endl;
    int progress_count = 0;
    int total_words = filtered.size();
    // sort by third character
    while(filtered.size() != 0 && *Graceful_Exit == false){
        // print progress
        std:: cout << "\r";
        std:: cout << progress_count << "/" << total_words;

        std::string word;
        word = filtered.at(0);

        // add first word
        if(filtered_sorted.size() == 0){
            filtered_sorted.push_back(word);
        }
        // otherwise sort
        else{
            std::deque<std::string>::iterator filtered_i = filtered_sorted.begin();
            // Iterate sorted deque
            for(std::size_t i = 0; i < filtered_sorted.size(); i++){
                std::string compare_word = filtered_sorted.at(i);

                // compare character by ASCII
                // if have same character, compare next character
                if(int(word[2]) == int(compare_word[2])){
                    bool insert_done = false;
                    bool go_next = false;
                    
                    // loop compare until last character of shorter word
                    for(std::size_t character_i = 3; character_i < word.length() && character_i < compare_word.length(); character_i++){
                        // insert word when meet first greater character
                        if(int(word[character_i]) < int(compare_word[character_i])){
                            filtered_sorted.insert(filtered_i, word);
                            insert_done = true;
                            break;
                        }
                        // stop compare and go to next compare word when meet first smaller character
                        else if(int(word[character_i]) > int(compare_word[character_i])){
                            go_next = true;
                            break;
                        }
                        // if this character still same, compare next character
                    }

                    // stop this loop if insert done
                    if(insert_done == true){
                        break;
                    }

                    // if go_next is false, mean 2 words have same front section, then need to compare word length
                    if(go_next == false){
                        // if words have same length, mean they are same word, stop loop without insert to ignore duplicates
                        if(word.length() == compare_word.length()){
                            break;
                        }
                        // if new word is shorter, insert word
                        else if(word.length() < compare_word.length()){
                            filtered_sorted.insert(filtered_i, word);
                            break;
                        }
                        // if new word is longer, do nothing and go to next word
                    }
                    
                }
                // insert word when meet first greater character
                else if(int(word[2]) < int(compare_word[2])){
                    filtered_sorted.insert(filtered_i, word);
                    break;
                }
                
                // if is last word in sorted deque, add word to end of deque
                // otherwise do nothing to compare next word in sorted deque
                if(i == filtered_sorted.size() - 1){
                    filtered_sorted.push_back(word);
                    break;
                }

                // update iterator to compare next word
                filtered_i++;
            }
        }

        // delete word that processed
        filtered.pop_front();

        // update variable
        progress_count++;
    }

    if(*Graceful_Exit == false){
        std:: cout << "\r";
        std:: cout << total_words << "/" << total_words << std::endl;
        std::cout << "Sort done!" << std::endl;
    }
    else{
        std::cout << std::endl;
        std::cout << "Time up! Graceful exiting..." << std::endl;
    }

    return filtered_sorted;
}

void write_file(std::deque<std::string> deque, std::string output_file){
    std::cout << "Outputting file..." << std::endl;
    try{
        std::ofstream output;
        char filename[output_file.length()];
        std::strcpy(filename, output_file.c_str());
        output.open(filename);

        while(deque.size() != 0){
            std::string word;
            word = deque.at(0);

            output << word << "\n";

            deque.pop_front();
        }

        output.close();

        std::cout << "Output file success!" << std::endl;
    }
    catch(...){
        std::cerr << "Fail while outputting!" << std::endl;
    }
}
