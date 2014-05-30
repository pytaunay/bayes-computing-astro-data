require(microbenchmark)

log_likelihood_naive <- function(obs,nobs,isig,mu,det_sig)
{
	sum = 0
	for (i in seq(1,nobs))
	{
		LV <- obs[i,] - mu
		RV <- t(LV)
		sum <- sum + LV %*% isig %*% RV 
	}	

	# Size of random variable
	N <- dim(mu)[2]

	sum <- sum + nobs*N*log(2*pi) + nobs*log(abs(det_sig))
	sum <- sum*(-0.5)

	return(sum)
}	

log_likelihood_vectorized <- function(obs,nobs,isig,mu,det_sig)
{
	sum = 0

	# Size of random variable
	N <- dim(mu)[2]
	
	# Create a temporary container for subtraction X-mu
	tmp <- matrix(1,nrow=nobs,ncol=1)
	tmp <- tmp %*% mu
	LV <- X-tmp 

	# First part of the product (X-mu)T*SIG
	tmp <- LV %*% isig

	# RHS
	RV <- matrix(LV,nrow=1,byrow=TRUE)
	RV <- t(RV)

	# Make that an array
	LV <- matrix(tmp,nrow=1,byrow=TRUE)

	# Single sum through dot product
	sum <- LV %*% RV

	sum <- sum + nobs*N*log(2*pi) + nobs*log(abs(det_sig))
	sum <- sum*(-0.5)

	return(sum)
}	



# Read the MU vector
csvfile = "/path/to/mu.csv"
mu = read.csv(csvfile,header=FALSE)
mu <- as.matrix(mu)

# Read the matrix inverse
csvfile = "/path/to/inv_sigma.csv"
isig = read.csv(csvfile,header=FALSE)
isig <- as.matrix(isig)

# Read the observations
NOBS = 100
csvfile = "/path/to/random_vectors.csv"
obs = read.csv(csvfile,header=FALSE)
obs <- as.matrix(obs)
X <- obs[1:NOBS,]

# Read the determinant
csvfile = "/path/to/det_sigma.csv"
det_sig = read.csv(csvfile,header=FALSE)

# Naive implementation
naive_tot <- log_likelihood_naive(X,NOBS,isig,mu,det_sig)
print("Naive timing")

res <- microbenchmark(log_likelihood_naive(X,NOBS,isig,mu,det_sig),times=100,unit='s')
print(res)

print("Result naive")
print(naive_tot)

# Vectorized
vec_tot <- log_likelihood_vectorized(X,NOBS,isig,mu,det_sig)
print("Vectorized timing")

res <- microbenchmark(log_likelihood_vectorized(X,NOBS,isig,mu,det_sig),times=100,unit='s')
print(res)

print("Result vectorized")
print(vec_tot)
