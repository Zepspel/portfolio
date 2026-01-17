# Time Series Analysis in R

## 📌 Description
Exploration of quarterly EPS data for Johnson & Johnson and simulated AR/MA series.  
The assignment focuses on visualization, stationarity, ARIMA modeling, and understanding autocorrelation patterns.

## 🧰 Tools
- R  
- `forecast` and `stats` packages (ARIMA, ACF, PACF)  

## 🔍 What was done
- Plotted quarterly EPS vs. time and described observed patterns (trend, seasonality)  
- Differenced the series (`xt - xt-1`) and log-transformed series to remove trend and stabilize variance  
- Plotted ACF and PACF of stationary series to identify candidate ARIMA models  
- Fitted and compared ARIMA models using AIC to select the best model  
- Explained why choice of log base is irrelevant and why ACF at lag 0 equals 1  
- Simulated 10,000 points for AR(p) and MA(p) processes (p = 1–4) using `arima.sim`  
  - Plotted simulated series, ACF, and PACF  
  - Observed how increasing p affects autocorrelation patterns  

## 📊 Key Insight
- Differencing and log transformation effectively remove trend and stabilize variance  
- ACF/PACF plots guide ARIMA model selection  
- Simulation of AR and MA series shows predictable changes in correlation structure with order p  
- Understanding autocorrelation and stationarity is crucial for reliable time series modeling

