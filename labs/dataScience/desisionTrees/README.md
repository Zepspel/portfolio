# Decision Trees in R

## 📌 Description
Classification of survey responses using decision trees.  
The assignment focuses on building, evaluating, and pruning classification trees, as well as analyzing model performance and feature importance.

## 🧰 Tools
- R  
- `rpart` package (decision tree)  
- `rpart.plot` package (visualization)  
- `ROCR` package (ROC and AUC analysis)  

## 🔍 What was done
- Imported and split `survey.csv` into training (rows 1–600) and testing data (rows 601–750)  
- Built classification tree: `MYDEPV ~ Price + Income + Age`  
  - Used three-fold cross-validation and information gain splitting index  
  - Examined which features were actually used (`printcp`)  
  - Plotted tree with `rpart.plot`  
- Scored model on training data and created confusion matrix  
  - Identified which class of `MYDEPV` was classified best  
  - Calculated resubstitution error rate and discussed its limitations as a performance metric  
- Evaluated model with ROC curve and calculated AUC using `ROCR`  
- Scored model on testing data to assess predictive accuracy  
- Built a new tree using Gini coefficient splitting index and compared it to the previous tree  
- Pruned the Gini tree based on the complexity parameter to reduce overfitting  
  - Examined features used in the pruned tree and explained why some variables were excluded  
- Created confusion matrix for the pruned model and compared performance before and after pruning  

## 📊 Key Insight
- Decision trees capture feature interactions effectively, but overfitting can occur on training data  
- Pruning reduces tree complexity, removes low-information variables, and can improve generalization  
- ROC curves and AUC provide useful metrics for assessing classifier performance beyond simple accuracy  
- Splitting index choice (information gain vs. Gini) can slightly alter tree structure and feature importance

