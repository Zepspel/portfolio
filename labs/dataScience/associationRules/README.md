# Association Rules Mining in R

## 📌 Description
Market basket analysis on simulated transaction data using the **Apriori** algorithm.  
The dataset contains 10,000 transactions with correlated items, generated using the Agrawal–Srikant method.

## 🧰 Tools
- R
- arules
- arulesViz

## 🔍 What was done
- Imported and explored transaction data
- Identified frequent items and transaction sizes
- Mined association rules with varying support and confidence thresholds
- Analyzed the impact of confidence on rule count
- Visualized rules using:
  - support vs. confidence vs. lift
  - support vs. lift
  - matrix-based and graph-based plots
- Filtered coincidental and low-value rules
- Extracted and analyzed high-lift and high-confidence rules
- Validated rule stability using train/test split

## 📊 Key Insight
High lift and confidence reveal meaningful item relationships, while low-lift rules often represent coincidental co-occurrences rather than true associations.

