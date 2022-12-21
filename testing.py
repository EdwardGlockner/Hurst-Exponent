from numpy import cumsum, log, polyfit, sqrt, std, subtract
from numpy.random import randn

def hurst(ts):

    lags = []
    for i in range(2,100):
        lags.append(i)

    tau = []
    for i in range(0, len(lags)):
        temp = []
        temp2 = []
        sub = []
        for j in range(lags[i], len(ts)):
            temp.append(ts[j])
        for k in range(0, len(ts)-lags[i]):
            temp2.append(ts[k]-temp[k])
        tau.append(sqrt(std(temp2)))


    # Use a linear fit to estimate the Hurst Exponent
    poly = polyfit(log(lags), log(tau), 1)



    # Return the Hurst exponent from the polyfit output
    return poly[0]*2.0

# Create a Gometric Brownian Motion, Mean-Reverting and Trending Series
gbm = log(cumsum(randn(100000))+1000)
mr = log(randn(100000)+1000)
tr = log(cumsum(randn(100000)+1)+1000)








print(hurst(gbm))





