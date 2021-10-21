#ifndef DA_CP_BASE_NAIVEBAYESCLASSIFIER_H
#define DA_CP_BASE_NAIVEBAYESCLASSIFIER_H

#include <iostream>
#include <vector>
#include <unordered_map>

using document = std::vector<std::string>;

struct data {
    int tag;
    document doc;
};

class NaiveBayesClassifier {
public:
    NaiveBayesClassifier(int classes = 2) {
        tag_sizes.resize(classes);
        dataset_size = 0;
    }

    void fit(std::vector<data> &dataset) {
        for (const auto& data : dataset) {
            int tag = data.tag;
            document doc = data.doc;
            for (const auto& word : doc) {
                if (tag_summary[tag][word]) {
                    ++tag_summary[tag][word];
                } else {
                    tag_summary[tag][word] = 1;
                }

                if (words_summary[word]) {
                    ++words_summary[word];
                } else {
                    words_summary[word] = 1;
                }

                ++tag_sizes[tag];
                ++dataset_size;
            }
        }
    }

    int predict(document &doc) {
        long double prob0 = probability(0, doc);
        long double prob1 = probability(1, doc);

        // std::cout << "Prob0 = " << prob0 << std::endl;
        // std::cout << "Prob1 = " << prob1 << std::endl;

        return prob0 > prob1 ? 0 : 1;
    }

    void print_summary() {
        for (const auto& tag_data : tag_summary) {
            int tag = tag_data.first;
            auto data = tag_data.second;
            std::cout << tag << ":\n";
            for (const auto& word_count : data) {
                std::cout << "\t" << word_count.first << " - " << word_count.second << "\n";
            }
        }
    }

private:
    std::vector<int> tag_sizes; // amounts of words with each tag
    std::unordered_map<int, std::unordered_map<std::string, int>> tag_summary; // amounts of each word with each tag
    std::unordered_map<std::string, int> words_summary; // amounts of each word
    int dataset_size; // total amount of words in dataset

    long double probability(int tag, document& doc, double eps = 1e-8) { // P(tag | doc)
        // the Bayes rule

        /*                  P(word_1 | tag) * P(word_2 | tag) * ... * P(word_n | tag) * P(tag)
         * P(tag | doc) =  ---------------------------------------------------------------------
         *                               P(word_1) * P(word_2) * ... * P(word_n)
         */

        // doc = word_1 + word_2 + ... + word_n

        // the denominator doesn't affect the maximum value of probabilities, so we won't consider it

        long double prob = 1;
        for (auto word : doc) {
            prob *= probability(word, tag, eps);
        }

        prob *= probability(tag);
        return prob;
    }

    long double probability(int tag) { // P(tag)
        return 1. * tag_sizes[tag] / dataset_size;
    }

    long double probability(std::string &word, int tag, double eps = 1e-8) { // P(word | tag)
        if (!tag_summary[tag][word]) {
            /* there were no such word with such tag in training dataset.
             * the answer should be 0, but we'll return epsilon value
             * so that the final probability was not equal to 0
             */
            return eps;
        }
        return 1. * tag_summary[tag][word] / words_summary[word];
    }
};


#endif //DA_CP_BASE_NAIVEBAYESCLASSIFIER_H

