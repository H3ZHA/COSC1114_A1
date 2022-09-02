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
int *fork_count;
std::deque<std::string> TaskFilter(std::string input_file);
void write_file(std::deque<std::string> deque, std::string output_file);
std::deque<std::string> map2(std::deque<std::string> deque);
void sort(std::deque<std::string> filtered, std::string filename);
int compare_2_words(std::string word1, std::string word2);
std::deque<std::string> reduce2(std::string files[]);

int main(int argc, char **argv) {
    // create global variable
    Graceful_Exit = (bool *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    fork_count = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    if(Graceful_Exit == MAP_FAILED || fork_count == MAP_FAILED){
        std::cerr << "mmap fail!" << std::endl;
        exit(0);
    }
    *fork_count = 0;
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
        output_data = map2(output_data);


        if(*Graceful_Exit == false){
            std::cout << "Merge files done! Outputting final file..." << std::endl;
        }
        write_file(output_data, output_file_name);
        if(*Graceful_Exit == false){
            std::cout << "Output fianl file success!" << std::endl;
        }

        if(*Graceful_Exit == false){
            // stop sleep countdown
            kill(main_fork, SIGKILL);
        }
        else{
            std::cout << std::endl;
            std::cout << "Time up! Graceful exiting..." << std::endl;
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
    file.close();

    return filtered;
}

void write_file(std::deque<std::string> deque, std::string output_file){
    try{
        std::ofstream output;
        char filename[output_file.length()];
        std::strcpy(filename, output_file.c_str());
        output.open(filename);

        while(deque.size() != 0 && *Graceful_Exit == false){
            std::string word;
            word = deque.at(0);

            output << word << "\n";

            deque.pop_front();
        }

        output.close();
    }
    catch(...){
        std::cerr << "Fail while outputting!" << std::endl;
    }
}

std::deque<std::string> map2(std::deque<std::string> deque){
    if(*Graceful_Exit == false){
        std::cout << "Data filtered done! Separating the words list..." << std::endl;
    }
    std::deque<std::string> words_3, words_4, words_5, words_6, words_7, words_8, words_9, words_10, words_11, words_12, 
                        words_13, words_14, words_15;
    std::deque<std::string> *word_list[13] = {&words_3, &words_4, &words_5, &words_6, &words_7, &words_8, &words_9, &words_10
                                                , &words_11 , &words_12, &words_13, &words_14, &words_15};
    std::string word_list_name[13] = {"words_3.txt", "words_4.txt", "words_5.txt", "words_6.txt", "words_7.txt", "words_8.txt"
            , "words_9.txt", "words_10.txt", "words_11.txt", "words_12.txt", "words_13.txt", "words_14.txt", "words_15.txt"};

    while(deque.size() != 0 && *Graceful_Exit == false){
        std::string word = deque.at(0);

        // Separate words
        if(word.length() == 3){
            words_3.push_back(word);
        }
        else if(word.length() == 4){
            words_4.push_back(word);
        }
        else if(word.length() == 5){
            words_5.push_back(word);
        }
        else if(word.length() == 6){
            words_6.push_back(word);
        }
        else if(word.length() == 7){
            words_7.push_back(word);
        }
        else if(word.length() == 8){
            words_8.push_back(word);
        }
        else if(word.length() == 9){
            words_9.push_back(word);
        }
        else if(word.length() == 10){
            words_10.push_back(word);
        }
        else if(word.length() == 11){
            words_11.push_back(word);
        }
        else if(word.length() == 12){
            words_12.push_back(word);
        }
        else if(word.length() == 13){
            words_13.push_back(word);
        }
        else if(word.length() == 14){
            words_14.push_back(word);
        }
        else if(word.length() == 15){
            words_15.push_back(word);
        }
        
        // delete word that processed
        deque.pop_front();
    }

    if(*Graceful_Exit == false){
        std::cout << "Separate done! Sorting each list..." << std::endl;
    }

    // create forks and do sort
    for(int i = 0; i < 13; i++){
        pid_t sort_fork = fork();
        if(sort_fork < 0){
            std::cerr << "Create fork fail!" << std::endl;
            exit(0);
        }
        // child do sort then exit
        if(sort_fork == 0){
            sort(*word_list[i], word_list_name[i]);
            // update global variable
            *fork_count = *fork_count + 1;
            exit(0);
        }
        // parent do nothing
    }

    // do meaningless loop until all forks done sort
    // wait() can only detect outermost fork
    while(*fork_count < 13){
    }

    std::deque<std::string> output;
    output = reduce2(word_list_name);

    return output;
}

void sort(std::deque<std::string> filtered, std::string filename){
    std::deque<std::string> filtered_sorted;
    int progress_count = 0;
    int total_words = filtered.size();
    // sort by third character
    while(filtered.size() != 0 && *Graceful_Exit == false){
        // print progress
        std::cout << "\r";
        std::cout << progress_count << "/" << total_words;
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

                // compare words
                int result = compare_2_words(word, compare_word);

                // return 1 means word is smaller, insert here
                if(result == 1){
                    filtered_sorted.insert(filtered_i, word);
                    break;
                }
                // 3 means same words, ignore duplicates
                else if(result == 3){
                    break;
                }
                // 0 means word is greater, do nothing to compare next word
                
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
        std::cout << "\r";
        std::cout << progress_count << "/" << total_words;
        std::cout << " - " << filename << " sort done!" << std::endl;
    }

    write_file(filtered_sorted, filename);
}

std::deque<std::string> reduce2(std::string files[]){
    if(*Graceful_Exit == false){
        std::cout << "All file sorted and output! Merging all files..." << std::endl;
    }
    std::deque<std::string> output;
    std::deque<std::string> words_3, words_4, words_5, words_6, words_7, words_8, words_9, words_10, words_11, words_12, 
                        words_13, words_14, words_15;
    std::deque<std::string> *word_list[13] = {&words_3, &words_4, &words_5, &words_6, &words_7, &words_8, &words_9, &words_10
                                                , &words_11 , &words_12, &words_13, &words_14, &words_15};

    // open files
    int word_remain = 0;
    for(int i = 0; i < 13 && *Graceful_Exit == false; i++){
        std::ifstream file;
        char filename[files[i].length()];
        std::strcpy(filename, files[i].c_str());
        file.open(filename);

        if(file.is_open()){
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
                        word_list[i]->push_back(temp);
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

        file.close();
        word_remain = word_remain + word_list[i]->size();
    }

    // merge files
    std::string word_in_3, word_in_4, word_in_5, word_in_6, word_in_7, word_in_8, word_in_9, word_in_10, word_in_11
                        , word_in_12, word_in_13, word_in_14, word_in_15;
    std::string word_in_temp[13] = {word_in_3, word_in_4, word_in_5, word_in_6, word_in_7, word_in_8, word_in_9, word_in_10
                        , word_in_11, word_in_12, word_in_13, word_in_14, word_in_15};
    int total_words = word_remain;

    // read first words in each list
    for(int i = 0; i < 13; i++){
        if(word_list[i]->size() != 0){
            word_in_temp[i] = word_list[i]->at(0);
            word_list[i]->pop_front();
        }
    }

    while(word_remain > 0 && *Graceful_Exit == false){
        // print progress
        std::cout << "\r";
        std::cout << (total_words - word_remain) << "/" << total_words;

        // find smallest word
        // smallest_word_list_number is current smallest word
        int smallest_word_list_number = 0;
        for(int i = 1; i < 13; i++){
            // if current smallest word is empty, update to next
            if(word_in_temp[smallest_word_list_number].length() == 0){
                smallest_word_list_number = i;
            }
            // utill valid current smallest word
            else{
                // skip empty word list
                if(word_in_temp[i].length() != 0){
                    // compare 2 words
                    int result = compare_2_words(word_in_temp[smallest_word_list_number], word_in_temp[i]);

                    // result == 0 mean current smallest word is greater than compare word, update it
                    if(result == 0){
                        smallest_word_list_number = i;
                    }
                }
            }
        }

        // write smallest word into output
        output.push_back(word_in_temp[smallest_word_list_number]);

        // refill word in temp, set "" to empty word list
        if(word_list[smallest_word_list_number]->size() != 0){
            word_in_temp[smallest_word_list_number] = word_list[smallest_word_list_number]->at(0);
            word_list[smallest_word_list_number]->pop_front();
        }
        else{
            word_in_temp[smallest_word_list_number] = "";
        }

        // update word remain
        word_remain--;
    }

    if(*Graceful_Exit == false){
        std::cout << "\r";
        std::cout << (total_words - word_remain) << "/" << total_words << std::endl;
    }
    

    return output;
}

int compare_2_words(std::string word1, std::string word2){
    // return 1 means word1 is smaller, 0 means word1 is greater, 3 means same words
    // compare character by ASCII
    // if have same character, compare next character
    if(int(word1[2]) == int(word2[2])){
        // loop compare until last character of shorter word
        for(std::size_t character_i = 3; character_i < word1.length() && character_i < word2.length(); character_i++){
            // return 1 when meet first greater character
            if(int(word1[character_i]) < int(word2[character_i])){
                return 1;
            }
            // return 0 when meet first smaller character
            else if(int(word1[character_i]) > int(word2[character_i])){
                return 0;
            }
            // if this character still same, compare next character
        }

        // if still not return, mean 2 words have same front section, then need to compare word length
        // if words have same length, mean they are same word, return 3 to ignore duplicates
        if(word1.length() == word2.length()){
            return 3;
        }
        // if word1 is shorter, return 1
        else if(word1.length() < word2.length()){
            return 1;
        }
        // if word1 is longer, return 0 and go to next word
        else{
            return 0;
        }
    }
    // if meet greater character
    else if(int(word1[2]) < int(word2[2])){
        return 1;
    }
    else{
        return 0;
    }
}
