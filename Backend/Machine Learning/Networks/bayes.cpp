#include <GTable.h>
#include <GList.h>
#include <GString.h>
#include "bayes.h"
#include <string>
#include <map>
#include <sstream>

namespace glades {

NaiveBayes::NaiveBayes() : n_gram_size(2) {
    // Constructor implementation moved here
}

shmea::GTable NaiveBayes::import(const shmea::GList& tokenizedWords) {
    shmea::GTable dummy(',');
    return dummy;
}

shmea::GTable NaiveBayes::import(const shmea::GTable& newInputTable) {
    std::vector<std::string> words;
    for (int r = 0; r < newInputTable.numberOfRows(); ++r) {
        shmea::GString line = newInputTable.getCell(r, 0);
        std::istringstream iss(line.c_str());
        std::string word;
        while (iss >> word) {
            preprocess(word);
            if (!word.empty()) words.push_back(word);
        }
    }
    for (int i = 0; i < words.size() - n_gram_size; ++i) {
        std::string context;
        for (int j = 0; j < n_gram_size; ++j) {
            context += words[i + j];
            if (j < n_gram_size - 1) context += "_";
        }
        ngram_probs[context][words[i + n_gram_size]] += 1.0;
    }
    for (std::map<std::string, std::map<std::string, double> >::iterator ctx = ngram_probs.begin(); ctx != ngram_probs.end(); ++ctx) {
        double total = 0.0;
        for (std::map<std::string, double>::iterator pair = ctx->second.begin(); pair != ctx->second.end(); ++pair) {
            total += pair->second;
        }
        for (std::map<std::string, double>::iterator pair = ctx->second.begin(); pair != ctx->second.end(); ++pair) {
            pair->second /= total;
        }
    }
    return newInputTable;
}

void NaiveBayes::train(const shmea::GTable& data) {
    import(data);
}

int NaiveBayes::predict(const shmea::GList& attributes) {
    std::string result = predict_string(attributes);
    return result.empty() ? -1 : 0;
}

std::string NaiveBayes::predict_string(const shmea::GList& attributes) {
    std::string key;
    for (int i = attributes.size() - n_gram_size; i < attributes.size(); ++i) {
        std::string word = attributes[i].c_str();
        preprocess(word);
        key += word;
        if (i < attributes.size() - 1) key += "_";
    }
    if (ngram_probs.find(key) != ngram_probs.end()) {
        std::map<std::string, double>& probs = ngram_probs[key];
        std::string best_word;
        double max_prob = -1.0;
        for (std::map<std::string, double>::iterator it = probs.begin(); it != probs.end(); ++it) {
            if (it->second > max_prob) {
                max_prob = it->second;
                best_word = it->first;
            }
        }
        return best_word;
    }
    return "";
}

void NaiveBayes::print() const {
    // Placeholder implementation
}

void NaiveBayes::reset() {
    classes.clear();
    attributesPerClass.clear();
    OHEMaps.clear();
    ngram_probs.clear();
}

std::string NaiveBayes::getClassName(int) const {
    return ""; // Placeholder
}

void NaiveBayes::preprocess(std::string& word) {
    for (int i = 0; i < word.length(); ++i) {
        word[i] = tolower(word[i]);
    }
    std::string clean;
    for (int i = 0; i < word.length(); ++i) {
        if (isalnum(word[i])) clean += word[i];
    }
    word = clean;
}

} // namespace glades
