#ifndef _GNAIVEBAYES
#define _GNAIVEBAYES

#include "Backend/Database/GTable.h"
#include "../GMath/OHE.h"
#include <stdio.h>
#include <vector>
#include <map>

namespace glades {

class NaiveBayes
{
private:
    // <class id, class probability> <C, P(C)>
    std::map<int, double> classes;

    // <class id, <attribute id, probability> > <C, <x, P(x|C)> >
    std::map<int, std::map<int, double> > attributesPerClass;

    // N-gram probabilities for text prediction
    std::map<std::string, std::map<std::string, double> > ngram_probs;
    int n_gram_size;

    std::vector<OHE> OHEMaps;

    void preprocess(std::string& word);

public:
    NaiveBayes();
    shmea::GTable import(const shmea::GList&);
    shmea::GTable import(const shmea::GTable&);
    void train(const shmea::GTable&);
    int predict(const shmea::GList&);
    std::string predict_string(const shmea::GList&);
    void print() const;
    void reset();
    std::string getClassName(int) const;
};

};

#endif
