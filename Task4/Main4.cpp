#include <iostream>
#include <sstream>
#include <fstream>
#include <deque>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/mman.h>
#include <wait.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <algorithm>

bool *Graceful_Exit;
int *sort_done;
int *words_in_3, *words_in_4, *words_in_5, *words_in_6, *words_in_7, *words_in_8, *words_in_9, *words_in_10, *words_in_11
    , *words_in_12, *words_in_13, *words_in_14, *words_in_15;
std::string FIFOs[13] = {"FIFO_3", "FIFO_4", "FIFO_5", "FIFO_6", "FIFO_7", "FIFO_8", "FIFO_9", "FIFO_10", "FIFO_11"
                            , "FIFO_12", "FIFO_13", "FIFO_14", "FIFO_15"};
std::deque<std::string> TaskFilter(std::string input_file);
void write_file(std::deque<std::string> deque, std::string output_file);
std::deque<std::string> my_sort(std::deque<std::string> filtered);
int compare_2_words(std::string word1, std::string word2);
void map3(std::deque<std::string> deque);
std::deque<std::string> reduce3();

int main(int argc, char **argv) {
    
    // create global variable
    Graceful_Exit = (bool *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_3 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_4 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_5 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_6 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_7 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_8 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_9 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_10 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_11 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_12 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_13 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_14 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    words_in_15 = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
    sort_done = (int *)mmap(NULL, 4, PROT_READ|PROT_WRITE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);

    if(Graceful_Exit == MAP_FAILED || words_in_3 == MAP_FAILED || words_in_4 == MAP_FAILED || words_in_5 == MAP_FAILED
         || words_in_6 == MAP_FAILED || words_in_7 == MAP_FAILED || words_in_8 == MAP_FAILED || words_in_9 == MAP_FAILED
         || words_in_10 == MAP_FAILED || words_in_11 == MAP_FAILED || words_in_12 == MAP_FAILED || words_in_13 == MAP_FAILED
         || words_in_14 == MAP_FAILED || words_in_15 == MAP_FAILED || sort_done == MAP_FAILED){
        std::cerr << "mmap fail!" << std::endl;
        exit(0);
    }
    *Graceful_Exit = false;
    *sort_done = 0;

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

        std::deque<std::string> filtered_data;
        std::deque<std::string> output;

        filtered_data = TaskFilter(input_file_name);

        pid_t map_reduce = fork();
        if(map_reduce < 0){
            std::cerr << "Create fork fail!" << std::endl;
            exit(0);
        }
        // map() is child
        if(map_reduce == 0){
            map3(filtered_data);
            exit(0);
        }
        // reduce() is parent
        else{
            // do meaningless loop until all forks done sort
            while(*sort_done < 13){
            }
            output = reduce3();
        }

        wait(NULL);
        if(*Graceful_Exit == false){
            std::cout << "Merge files done! Outputting final file..." << std::endl;
        }
        write_file(output, output_file_name);
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

std::deque<std::string> my_sort(std::deque<std::string> filtered){
    std::deque<std::string> filtered_sorted;
    int progress_count = 0;
    // sort by third character
    while(filtered.size() != 0 && *Graceful_Exit == false){
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

    // update deque

    if(*Graceful_Exit == false){
        std::cout << "Words with length " << filtered_sorted.at(0).length() << " sort done! Creating FIFO file..." << std::endl;
    }

    return filtered_sorted;
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

void map3(std::deque<std::string> deque){
    if(*Graceful_Exit == false){
        std::cout << "Data filtered done! Separating the words list..." << std::endl;
    }
    std::deque<std::string> temp, words_3, words_4, words_5, words_6, words_7, words_8, words_9, words_10, words_11, words_12, 
                        words_13, words_14, words_15;
    std::deque<std::string> *word_list[14] = {&temp, &words_3, &words_4, &words_5, &words_6, &words_7, &words_8, &words_9, &words_10
                                                , &words_11 , &words_12, &words_13, &words_14, &words_15};
    int list_sort_by_size[13];
    int priority[13];

    // separate
    while(deque.size() != 0 && *Graceful_Exit == false){
        std::string word = deque.at(0);

        // i+2 is the length of word that list include
        int index = (int)word.length() - 2;
        word_list[index]->push_back(word);

        // delete word that processed
        deque.pop_front();
    }
    
    // add all size to list
    for(int i = 1; i < 14; i++){
        list_sort_by_size[i-1] = word_list[i]->size();
    }

    // sort list by size
    std::sort(list_sort_by_size, list_sort_by_size+13);

    // set priority
    for(int i = 0; i < 13; i++){
        for(int index = 1; index < 14; index++){
            if(list_sort_by_size[i] == (int)word_list[index]->size()){
                priority[index-1] = i;
                break;
            }
        }
    }

    // create forks to separate and sort
    for(int i = 1; i < 14; i++){
        pid_t sort_fork = fork();
        if(sort_fork < 0){
            std::cerr << "Create fork fail!" << std::endl;
            exit(0);
        }
        // child do separate and sort then exit
        if(sort_fork == 0){
            // set priority
            // build command
            char command_1[] = "renice ";
            char command_2[10];
            char command_3[] = " -p ";
            char command_4[10];

            // -20 is highest priority
            int command_priority = priority[i-1] - 20;
            snprintf(command_2, sizeof(command_2), "%d", command_priority);

            int command_pid = getpid();
            snprintf(command_4, sizeof(command_4), "%d", command_pid);

            strcat(command_1, command_2);
            strcat(command_1, command_3);
            strcat(command_1, command_4);
            system(command_1);


            // sort
            *word_list[i] = my_sort(*word_list[i]);

            // update word count
            if(i == 1){
                *words_in_3 = word_list[i]->size();
            }
            else if(i == 2){
                *words_in_4 = word_list[i]->size();
            }
            else if(i == 3){
                *words_in_5 = word_list[i]->size();
            }
            else if(i == 4){
                *words_in_6 = word_list[i]->size();
            }
            else if(i == 5){
                *words_in_7 = word_list[i]->size();
            }
            else if(i == 6){
                *words_in_8 = word_list[i]->size();
            }
            else if(i == 7){
                *words_in_9 = word_list[i]->size();
            }
            else if(i == 8){
                *words_in_10 = word_list[i]->size();
            }
            else if(i == 9){
                *words_in_11 = word_list[i]->size();
            }
            else if(i == 10){
                *words_in_12 = word_list[i]->size();
            }
            else if(i == 11){
                *words_in_13 = word_list[i]->size();
            }
            else if(i == 12){
                *words_in_14 = word_list[i]->size();
            }
            else if(i == 13){
                *words_in_15 = word_list[i]->size();
            }

            *sort_done = *sort_done + 1;

            // check FIFO file exist,, if not create FIFO file
            char fifo_name[6];
            std::strcpy(fifo_name, FIFOs[i-1].c_str());
            int fifo = access(fifo_name, F_OK);
            if(fifo != 0){
                fifo = mkfifo(fifo_name, 0777);
                if(fifo < 0){
                    std::cerr << "Create FIFO file fail!" << std::endl;
                    exit(0);
                }
            }

            // open FIFO file
            fifo = open(fifo_name, O_WRONLY);
            if(fifo < 0){
                std::cerr << "Open FIFO file fail!" << std::endl;
                exit(0);
            }

            while(word_list[i]->size() != 0 && *Graceful_Exit == false){
                std::string word = word_list[i]->at(0);
                char word_char[word.length()];
                std::strcpy(word_char, word.c_str());

                write(fifo, word_char, sizeof(word_char)+1);
                

                word_list[i]->pop_front();
            }

            close(fifo);
            
            exit(0);
        }
        // parent do nothing
    }
}

std::deque<std::string> reduce3(){
    std::string word_in_3, word_in_4, word_in_5, word_in_6, word_in_7, word_in_8, word_in_9, word_in_10, word_in_11
                        , word_in_12, word_in_13, word_in_14, word_in_15;
    std::string word_in_temp[13] = {word_in_3, word_in_4, word_in_5, word_in_6, word_in_7, word_in_8, word_in_9, word_in_10
                        , word_in_11, word_in_12, word_in_13, word_in_14, word_in_15};
    int *word_in_list_count[13] = {words_in_3, words_in_4, words_in_5, words_in_6, words_in_7, words_in_8, words_in_9, words_in_10
                                , words_in_11, words_in_12, words_in_13, words_in_14, words_in_15};
    std::deque<std::string> final_output;
    int word_remain = 0;
    int fifo3 = 0, fifo4 = 0, fifo5 = 0, fifo6 = 0, fifo7 = 0, fifo8 = 0, fifo9 = 0, fifo10 = 0, fifo11 = 0, fifo12 = 0
        , fifo13 = 0, fifo14 = 0, fifo15 = 0;
    int fifos[] = {fifo3, fifo4, fifo5, fifo6, fifo7, fifo8, fifo9, fifo10, fifo11, fifo12, fifo13, fifo14, fifo15};

    // open FIFO files and read first word
    for(int i = 0; i < 13 && *word_in_list_count[i] > 0; i++){
        // check FIFO file exist,, if not create FIFO file
        char fifo_name[FIFOs[i].length()];
        std::strcpy(fifo_name, FIFOs[i].c_str());
        fifos[i] = access(fifo_name, F_OK);
        if(fifos[i] != 0){
            fifos[i] = mkfifo(fifo_name, 0777);
            if(fifos[i] < 0){
                std::cerr << "Create FIFO file fail!" << std::endl;
                exit(0);
            }
        }

        // open FIFO file
        fifos[i] = open(fifo_name, O_RDONLY);
        if(fifos[i] < 0){
            std::cerr << "Open FIFO file fail!" << std::endl;
            exit(0);
        }

        // read first word
        char word_temp[i+3];
        read(fifos[i], word_temp, i+4);
        word_in_temp[i] = word_temp;
        

        word_remain = word_remain + *word_in_list_count[i];
        *word_in_list_count[i] = *word_in_list_count[i] - 1;
    }

    int total_words = word_remain;
    
    // merge files
    while(word_remain != 0 && *Graceful_Exit == false){
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
        final_output.push_back(word_in_temp[smallest_word_list_number]);

        // refill word in temp, set "" to empty word list
        if(*word_in_list_count[smallest_word_list_number] != 0){
            char fifo_name[FIFOs[smallest_word_list_number].length()];
            std::strcpy(fifo_name, FIFOs[smallest_word_list_number].c_str());

            // read word
            char word_temp[smallest_word_list_number+3];
            read(fifos[smallest_word_list_number], word_temp, smallest_word_list_number+4);
            word_in_temp[smallest_word_list_number] = word_temp;

            *word_in_list_count[smallest_word_list_number] = *word_in_list_count[smallest_word_list_number] - 1;
        }
        else{
            word_in_temp[smallest_word_list_number] = "";
            // close FIFO file
            close(fifos[smallest_word_list_number]);
        }

        // update word remain
        word_remain--;
    }

    if(*Graceful_Exit == false){
        std::cout << "\r";
        std::cout << (total_words - word_remain) << "/" << total_words << std::endl;
    }
    
    return final_output;
}

