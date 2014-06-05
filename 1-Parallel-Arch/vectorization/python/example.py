#!/usr/bin/python

import time
import math
import random
import numpy as np


def read_file(csvfile,footer=0):
	data = np.array
	data = np.genfromtxt(csvfile,delimiter=",",dtype=float,skip_footer=footer)

	return data

def log_likelihood_naive(obs,nobs,isig,mu,det_sig):
	sum = 0
	# Sum all members
	for i in range(0,nobs):
		LV = obs[i]-mu;
		RV = np.transpose(LV);
		sum += np.dot(np.dot(LV,isig),RV)

	# Size of random variable
	N = mu.shape[0]

	# Add the rest to sum
	sum += nobs*N*math.log(2*np.pi) + nobs*math.log(abs(det_sig))
	sum *= -0.5

	return sum


def log_likelihood_vec(obs,nobs,isig,mu,det_sig):
	sum = 0

	# Size of random variable
	N = mu.shape[0]

	# Left hand side: (X-mu)T*SIG
	LV = obs[:]-mu
	tmp = np.dot(LV,isig)

	# Right hand side: (X-mu)
	RV = np.reshape(LV,[nobs*N,1])

	# Reshape left hand side
	LV = np.reshape(tmp,[1,nobs*N])

	# Perform single sum through dot product
	sum = np.dot(LV,RV)


	# Add the rest to sum
	sum += nobs*N*math.log(2*np.pi) + nobs*math.log(abs(det_sig))
	sum *= -0.5

	return sum



# Read the MU vector
csvfile = "/path/to/mu.csv"
mu = np.array
mu = read_file(csvfile)

# Read the matrix inverse
csvfile = "/path/to/inv_sigma.csv"
isig  = np.array
isig = read_file(csvfile)

# Read the observations
NOBS = 100
NOBSMAX = 1000000
footer = NOBSMAX-NOBS
csvfile = "/path/to/random_vectors.csv"
obs = np.array
obs = read_file(csvfile,footer)

# Read the determinant
csvfile = "/path/to/det_sigma.csv"
det_sig = read_file(csvfile)

# Calculate log-likelihood
NSAMPLE = 100
ave_naive = 0
for i in range(0,NSAMPLE):
	tic = time.time()
	naive_res = log_likelihood_naive(obs,NOBS,isig,mu,det_sig)
	toc = time.time()
	ave_naive += toc-tic
	
print "Naive time (s):" 
print ave_naive/NSAMPLE


ave_vec = 0
for i in range(0,NSAMPLE):
	tic = time.time()
	vec_res = log_likelihood_vec(obs,NOBS,isig,mu,det_sig)
	toc = time.time()
	ave_vec += toc-tic

print "Vectorized time (s):" 
print ave_vec/NSAMPLE

print "Results"
print naive_res
print vec_res

