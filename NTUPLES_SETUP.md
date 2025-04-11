# N-tuples Feature Setup and Testing

This document guides you through setting up, building, and testing the 2-gram n-tuples feature added to `bayes.cpp` in the Glades-ML project. The feature introduces a `predict_string` method for text prediction, tested with a subset of the UCI Sentiment Labelled Sentences dataset.

## Prerequisites

- **CMake**: Version 3.5.1 or higher (tested with 3.28.3).
- **GTest**: Google Test framework (e.g., install via `sudo apt-get install libgtest-dev` on Ubuntu).
- **ShmeaDB**: Dependency providing `GTable`, `GList`, and `GString` (installation steps below).
- **Dataset**: `data/amazon_subset.txt` (included in this PR, derived from UCI Sentiment).

## Setup Instructions

### 1. Pull the Code
Clone or pull the n-tuples branch:
```bash
git pull origin ntuples
```

### 2. Install ShmeaDB Dependency
The code uses a hardcoded include path `/home/ubuntu/shmea/Backend/Database` for ShmeaDB headers. Update this to your local ShmeaDB installation:

- **Clone and Install ShmeaDB**:
  ```bash
  git clone https://github.com/MeeseeksLookAtMe/shmea ~/shmea
  cd ~/shmea
  mkdir build
  cd build
  cmake .. -DCMAKE_INSTALL_PREFIX=$HOME/.local
  make -j1
  sudo make install
  ```
  - Headers will be in `~/shmea/Backend/Database/` or `$HOME/.local/include/shmea/`.

- **Update Include Path**:
  - Edit `unit-tests/CMakeLists.txt`:
    ```bash
    nano unit-tests/CMakeLists.txt
    ```
  - Change line ~10 from:
    ```cmake
    include_directories(/home/ubuntu/shmea/Backend/Database)
    ```
    To your ShmeaDB path, e.g.:
    ```cmake
    include_directories($ENV{HOME}/shmea/Backend/Database)  # Adjust to your path
    ```
  - Save and exit (`Ctrl+O`, `Enter`, `Ctrl+X`).

### 3. Prepare the Dataset
The test uses `data/amazon_subset.txt`, a comma-separated CSV included in this PR, derived from the UCI Sentiment Labelled Sentences dataset.

- **Download the Dataset**:
  - URL: `https://archive.ics.uci.edu/ml/machine-learning-databases/00331/sentiment%20labelled%20sentences.zip`
  - Command:
    ```bash
    cd ~/glades-ml
    wget https://archive.ics.uci.edu/ml/machine-learning-databases/00331/sentiment%20labelled%20sentences.zip
    unzip sentiment\ labelled\ sentences.zip
    ```

- **Modify the Dataset with `awk`**:
  - The original `amazon_cells_labelled.txt` is tab-separated. Convert it to comma-separated CSV with quoted text:
    ```bash
    head -n 100 "sentiment labelled sentences/amazon_cells_labelled.txt" | awk -F'\t' '{print "\"" $1 "\", " $2}' > data/amazon_subset.txt
    ```
  - This takes the first 100 lines, quotes the text (field 1), and appends the label (field 2) with a comma.

- **Verify**:
  ```bash
  head -n 5 data/amazon_subset.txt
  ```
  - Expected output:
    ```
    "So there is no way for me to plug it in here in the US unless I go by a converter.", 0
    "Good case, Excellent value.", 1
    "Great for the jawbone.", 1
    "Tied to charger for conversations lasting more than 45 minutes.MAJOR PROBLEMS!!", 0
    "The mic is great.", 1
    ```
  - No further changes needed unless you want a custom dataset.

### 4. Build the Project
```bash
cd ~/glades-ml
mkdir build
cd build
cmake ..
make -j1
```
- **Note**: Ensure the ShmeaDB include path is correct in `unit-tests/CMakeLists.txt` before running `cmake`.

### 5. Test the Feature
- **Manual Test** (Recommended):
  ```bash
  ./tests --gtest_filter=BayesTest.NtuplePrediction
  ```
  - Expected output:
    ```
    [==========] Running 1 test from 1 test suite.
    [----------] Global test environment set-up.
    [----------] 1 test from BayesTest
    [ RUN      ] BayesTest.NtuplePrediction
    [CSV] +/home/ubuntu/glades-ml/data/amazon_subset.txt
    Rows in trainingTable: 99
    Prediction: quality
    [       OK ] BayesTest.NtuplePrediction (X ms)
    [----------] 1 test from BayesTest (X ms total)
    [==========] 1 test from 1 test suite ran. (X ms total)
    [  PASSED  ] 1 test.
    ```

- **CTest** (Optional, under investigation):
  ```bash
  ctest -R BayesTest.NtuplePrediction --verbose
  ```
  - Current status: May show "No tests were found!!!" due to a `gtest_discover_tests` issue. Manual test confirms functionality.

## Code Changes Overview
- **`Backend/Machine Learning/Networks/bayes.h`**:
  - Added `n_gram_size`, `ngram_probs`, `preprocess`, and `predict_string`.
- **`Backend/Machine Learning/Networks/bayes.cpp`**:
  - Implemented n-tuples logic in `import` and `predict_string`.
- **`unit-tests/Backend/Machine Learning/bayes-test.cpp`**:
  - Test for `predict_string` with UCI Sentiment data.
- **`unit-tests/CMakeLists.txt`**:
  - Hardcoded include: `/home/ubuntu/shmea/Backend/Database`—update to your ShmeaDB path.
  - Uses `gtest_discover_tests` for test registration.

## Notes
- **Hardcoded Path**: Replace `/home/ubuntu/shmea/Backend/Database` with your ShmeaDB include directory in `unit-tests/CMakeLists.txt`.
- **CTest Issue**: Manual test (`./tests --gtest_filter=BayesTest.NtuplePrediction`) works; `ctest` detection is pending further debugging but doesn’t affect functionality.
- **Dataset Source**: UCI Sentiment dataset from `https://archive.ics.uci.edu/ml/machine-learning-databases/00331/`.

For questions or help adjusting paths, feel free to reach out!
