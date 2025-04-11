# N-tuples Feature Setup and Testing

This document guides you through setting up, building, and testing the 2-gram n-tuples feature added to `bayes.cpp` in the Glades-ML project. The feature introduces a `predict_string` method for text prediction, tested with a subset of the UCI Sentiment dataset.

## Prerequisites

- **CMake**: Version 3.5.1 or higher (tested with 3.28.3).
- **GTest**: Google Test framework (e.g., install via `sudo apt-get install libgtest-dev` on Ubuntu).
- **ShmeaDB**: Dependency providing `GTable`, `GList`, and `GString` (installation steps below).
- **Dataset**: `data/amazon_subset.txt` (included in this PR, 100 sentences from UCI Sentiment).

## Setup Instructions

### 1. Pull the Code
Clone or pull the n-tuples branch:
```bash
git pull origin ntuples
