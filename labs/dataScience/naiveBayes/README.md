# Naïve Bayes Classifier in R

## 📌 Description
Classification of individuals’ income and sex using categorical survey data.  
The assignment explores constructing Naïve Bayes models, evaluating predictive performance, and analyzing the effect of training data selection on model accuracy.

## 🧰 Tools
- R  
- `e1071` package (`naiveBayes` function)  

## 🔍 What was done
- Imported and split `nbtrain.csv` into training (first 9010 records) and testing data (last 1000 records)  
- **Income Prediction:**  
  - Built Naïve Bayes model: `income ~ age + sex + educ`  
  - Calculated model’s a priori and conditional probabilities  
  - Scored model on testing data and created confusion matrix  
  - Calculated overall and class-specific misclassification rates (10-50K, 50-80K, GT 80K)  
- **Sex Prediction:**  
  - Built Naïve Bayes model: `sex ~ age + educ + income`  
  - Calculated overall, female, and male misclassification rates  
  - Subsampled training data (3500 records per sex) and rebuilt model  
  - Evaluated model performance on testing data  
  - Repeated random sampling to observe variability in predictions  

## 📊 Key Insight
- Naïve Bayes effectively captures categorical relationships, but predictive power varies across income classes due to class imbalance  
- Random selection of training records influences model probabilities and performance, highlighting the importance of representative sampling  
- Models for predicting sex show lower misclassification for the majority class, illustrating the impact of class distribution on accuracy

