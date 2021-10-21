#include "NaiveBayesClassifier.h"

document convert(std::string &text); // convert text to vector of words

int main() {
    int train_size, test_size;
    std::cin >> train_size >> test_size;
    std::vector<data> dataset(train_size);
    for (int i = 0; i < train_size; ++i) {
        int tag;
        std::string text;
        std::cin >> tag;
        std::cin.ignore();
        getline(std::cin, text);
        dataset[i] = {tag, convert(text)};
    }
    NaiveBayesClassifier NB;
    NB.fit(dataset);

    for (int i = 0; i < test_size; ++i) {
        std::string text;
        getline(std::cin, text);
        document doc = convert(text);
        std::cout << NB.predict(doc) << std::endl;
    }
}

document convert(std::string &text) { // convert text to vector of words
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

