# U.S. States Clustering (R)

## 📌 Description
Clustering of U.S. states (including Washington D.C. and Puerto Rico) based on:
- mean household income
- mean household electricity usage

Both **k-means** and **hierarchical clustering** methods are used to analyze structure in the data.

## 🧰 Tools
- R
- ggplot2
- maps

## 🔍 What was done
- Extracted census data from a relational database
- Applied k-means clustering (k = 10)
- Studied clustering instability and initialization effects
- Selected a reasonable value of k
- Re-clustered data using log₁₀-transformed features
- Detected and removed an outlier
- Visualized clusters on a U.S. map
- Repeated experiments with hierarchical clustering using different linkage methods

## 📊 Key Insight
Log transformation and outlier removal significantly affect cluster structure, leading to more interpretable and stable groupings of U.S. states.

