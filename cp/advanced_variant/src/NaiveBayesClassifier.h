#ifndef DA_CP_BASE_NAIVEBAYESCLASSIFIER_H
#define DA_CP_BASE_NAIVEBAYESCLASSIFIER_H

#include <iostream>
#include <vector>
#include <unordered_map>

using TText = std::vector<std::string>;

struct data {
    TText tag;
    TText doc;
};

class NaiveBayesClassifier {
public:
    void fit(std::vector<data> &dataset) {
        for (const auto &data : dataset) {
            TText tags = data.tag;
            TText doc = data.doc;
            for (auto &tag: tags) {
                for (auto &word : doc) {
                    add(tag, word);
                }
            }
            dataset_size += doc.size();
        }
    }

    std::unordered_map<std::string, double> predict_proba(TText &doc) {
        std::unordered_map<std::string, double> probabilities;
        for (auto &data : tag_summary) {
            std::string cur_tag = data.first;
            probabilities[cur_tag] = probability(cur_tag, doc);
        }

        return probabilities;
    }

    void save_stats(std::ofstream &out) {
        out << dataset_size << "\n";
        for (auto &tag_data : tag_summary) {
            std::string tag = tag_data.first;
            auto data = tag_data.second;
            out << tag << " " << data.size() << " ";
            for (auto &word_count : data) {
                out << word_count.first << " " << word_count.second << " ";
            }
            out << "\n";
        }
    }

    void load_stats(std::ifstream &in) {
        in >> dataset_size;
        std::string tag;
        while (in >> tag) {
            int amount_of_words;
            in >> amount_of_words;
            for (int i = 0; i < amount_of_words; ++i) {
                std::string word;
                int count;
                in >> word >> count;
                add(tag, word, count);
            }
        }
    }

    int get_number_of_tags() {
        return tag_sizes.size();
    }

private:
    std::unordered_map<std::string, int> tag_sizes; // amounts of words with each tag
    std::unordered_map<std::string, std::unordered_map<std::string, int>> tag_summary; // amounts of each word with each tag
    std::unordered_map<std::string, int> words_summary; // amounts of each word
    int dataset_size = 0; // total amount of words in dataset

    double probability(std::string &tag, TText &doc, double alpha = 1) { // P(tag | doc)
        // the Bayes rule

        /*                  P(word_1 | tag) * P(word_2 | tag) * ... * P(word_n | tag) * P(tag)
         * P(tag | doc) =  ---------------------------------------------------------------------
         *                               P(word_1) * P(word_2) * ... * P(word_n)
         */

        // doc = word_1 + word_2 + ... + word_n

        // the denominator doesn't affect the maximum value of probabilities, so we won't consider it

        // to avoid underflow we'll use:
        // P(tag | doc) = log(P(word_1 | tag)) + log(P(word_2 | tag)) + ... + log(P(word_n | tag)) + log(P(tag))

        double prob = 0;
        for (auto word : doc) {
            prob += log(probability(word, tag, alpha));
        }

        prob += log(probability(tag, alpha));

        return prob;
    }

    double probability(std::string &tag, double alpha = 1) { // P(tag)
        // applying Laplace smoothing
        return (1. * tag_sizes[tag] + alpha) / (dataset_size + alpha * dataset_size);
    }

    double probability(std::string &word, std::string &tag, double alpha = 1) { // P(word | tag)
        // applying Laplace smoothing
        return (1. * tag_summary[tag][word] + alpha) / (words_summary[word] + alpha * tag_summary.size());
    }

    void add(std::string &tag, std::string &word) {
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

        if (tag_sizes[tag]) {
            tag_sizes[tag] = 1;
        } else {
            ++tag_sizes[tag];
        }
    }

    void add(std::string &tag, std::string &word, int count) {
        tag_summary[tag][word] = count;

        if (words_summary[word]) {
            words_summary[word] += count;
        } else {
            words_summary[word] = count;
        }

        if (tag_sizes[tag]) {
            tag_sizes[tag] += count;
        } else {
            tag_sizes[tag] = count;
        }
    }
};


#endif //DA_CP_BASE_NAIVEBAYESCLASSIFIER_H
