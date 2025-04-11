#include <gtest/gtest.h>
#include <GTable.h>
#include <GList.h>
#include <GString.h>
#include "Backend/Machine Learning/Networks/bayes.h"
#include <iostream>

TEST(BayesTest, NtuplePrediction) {
    glades::NaiveBayes bayes;
    shmea::GTable trainingTable("/home/ubuntu/glades-ml/data/amazon_subset.txt", ',', shmea::GTable::TYPE_FILE);
    if (trainingTable.numberOfRows() == 0) {
        std::cerr << "Failed to load trainingTable" << std::endl;
        FAIL() << "Training table is empty";
    }
    std::cout << "Rows in trainingTable: " << trainingTable.numberOfRows() << std::endl;
    bayes.import(trainingTable);
    shmea::GList context;
    context.addString(shmea::GString("very"));
    context.addString(shmea::GString("good"));
    std::string pred = bayes.predict_string(context);
    std::cout << "Prediction: " << pred << std::endl;
    EXPECT_TRUE(!pred.empty());
}
