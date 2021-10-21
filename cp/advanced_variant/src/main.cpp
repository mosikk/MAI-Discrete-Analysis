#include <cstring>
#include <fstream>
#include <unordered_set>
#include <cmath>

#include "NaiveBayesClassifier.h"

TText convert(std::string &text); // convert text to vector of words
void print_error();
std::unordered_map<std::string, double> softmax(std::unordered_map<std::string, double>& preds);

int main(int argc, char *argv[]) {
    std::string input_file_name;
    std::string stats_file_name;
    std::string output_file_name;

    if (argc < 6) {
        print_error();
        return 1;
    }

    if (!strcmp(argv[1], "learn")) {
        if (argc != 6) {
            print_error();
            return 1;
        }

        for (int i = 2; i < argc; ++i) {
            if (!strcmp(argv[i], "--input") && input_file_name.empty()) {
                input_file_name = argv[++i];
            } else if (!strcmp(argv[i], "--output") && stats_file_name.empty()) {
                stats_file_name = argv[++i];
            } else {
                print_error();
                return 1;
            }
        }

        if (input_file_name.empty() || stats_file_name.empty()) {
            print_error();
            return 1;
        }

        std::ifstream input_file(input_file_name);
        std::ofstream stats_file(stats_file_name);
        std::vector<data> dataset;

        int lines;
        while (input_file >> lines) {
            std::string tags;
            std::string text;

            input_file.ignore();
            getline(input_file, tags);
            for (int i = 0; i < lines + 1; ++i) {
                std::string cur_line;
                getline(input_file, cur_line);
                text += " " + cur_line;
            }
            dataset.push_back({convert(tags), convert(text)});
        }

        NaiveBayesClassifier NB;
        NB.fit(dataset);
        NB.save_stats(stats_file);
    } else if (!strcmp(argv[1], "classify")) {
        if (argc != 8) {
            print_error();
            return 1;
        }

        for (int i = 2; i < argc; ++i) {
            if (!strcmp(argv[i], "--input") && input_file_name.empty()) {
                input_file_name = argv[++i];
            } else if (!strcmp(argv[i], "--output") && output_file_name.empty()) {
                output_file_name = argv[++i];
            } else if (!strcmp(argv[i], "--stats") && stats_file_name.empty()) {
                stats_file_name = argv[++i];
            } else {
                print_error();
                return 1;
            }
        }

        if (input_file_name.empty() || stats_file_name.empty() || output_file_name.empty()) {
            print_error();
            return 1;
        }

        std::ifstream input_file(input_file_name);
        std::ifstream stats_file(stats_file_name);
        std::ofstream output_file(output_file_name);

        NaiveBayesClassifier NB;
        NB.load_stats(stats_file);

        int lines;
        while (input_file >> lines) {
            std::string text;

            input_file.ignore();
            for (int i = 0; i < lines + 1; ++i) {
                std::string cur_line;
                getline(input_file, cur_line);
                text += " " + cur_line;
            }
            auto doc = convert(text);
            auto probas = NB.predict_proba(doc);
            auto preds = softmax(probas);

            bool need_comma = false;
            double threshold = 1. / NB.get_number_of_tags();
            for (const auto& pred : preds) {
                if (pred.second > threshold) {
                    if (need_comma) {
                        output_file << ", ";
                    }
                    output_file << pred.first;
                    need_comma = true;
                }
            }
            output_file << "\n";
        }

    } else {
        print_error();
        return 1;
    }
}

TText convert(std::string &text) { // convert text to vector of words
    std::vector<std::string> words;
    int i = 0;
    std::string cur_word;
    while (i < text.size()) {
        if (isalpha(text[i])) {
            cur_word += tolower(text[i]);
        } else if (cur_word.length()) {
            words.push_back(cur_word);
            cur_word = "";
        }
        if (i + 1 == text.size() && cur_word.length()) {
            words.push_back(cur_word);
        }
        ++i;
    }
    return words;
}

void print_error() {
    std::cerr << "Incorrect syntax!" << std::endl <<
              "./classifier learn --input <input file> --output <output file>" << std::endl <<
              "./classifier classify --stats <stats file> --input <input file> --output <output file>" << std::endl;
}

std::unordered_map<std::string, double> softmax(std::unordered_map<std::string, double>& probs) {
    // src: https://slaystudy.com/implementation-of-softmax-activation-function-in-c-c/
    double max = -1e10;
    for (const auto& prob : probs) {
        if (prob.second > max) {
            max = prob.second;
        }
    }

    double sum = 0;
    for (auto& prob : probs) {
        sum += exp(prob.second - max);
    }

    double constant = max + log(sum);
    std::unordered_map<std::string, double> res;
    for (auto prob : probs) {
        res[prob.first] = exp(prob.second - constant);
    }
    return res;
}
