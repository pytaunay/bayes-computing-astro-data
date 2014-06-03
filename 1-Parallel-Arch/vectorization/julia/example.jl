function log_likelihood_naive(obs::Array{Float64,2},nobs::Int64,isig::Array{Float64,2},mu::Array{Float64,1},det_sig::Float64)
	@assert(size(obs,1)==length(mu)==size(isig,1)==size(isig,2))

	N = size(obs,1)
	RV = Array(Float64,1)

	tot = 0.0
	for i=1:nobs
		RV = obs[:,i] - mu
		tot += RV'*isig*RV
	end

	tot += nobs*N*log(2*pi) + nobs*log(abs(det_sig))
	tot *= (-0.5)

	return tot 
end	

function log_likelihood_vec(obs::Array{Float64,2},nobs::Int64,isig::Array{Float64,2},mu::Array{Float64,1},det_sig::Float64)
	@assert(size(obs,1)==length(mu)==size(isig,1)==size(isig,2))

	tot = 0.0
	N = size(mu,1)

	# X-mu
	RV = obs.-mu

	# X-mu * isig
	tmp = isig*RV

	tot = dot(vec(RV),vec(tmp))

	tot += nobs*N*log(2*pi) + nobs*log(abs(det_sig))
	tot *= (-0.5)

	return tot 
end


print("Reading data...")
# Load data
# MU
mu = readcsv("mu.csv")
mu = reshape(mu,size(mu,2))

# X
X = readcsv("random_vectors.csv")
X = transpose(X)

# inverse sigma
isig = readcsv("inv_sigma.csv")

# determinant
det_sig = readcsv("det_sigma.csv")
det_sig = det_sig[1,1]

print("success\n")

nobs = 100

nsample = 100

naive_time = 0.0
vec_time = 0.0
devec_time = 0.0
naive_sum = 0.0
vec_sum = 0.0
devec_sum = 0.0

# Julia has a just-in-time compiler:
# Have to call functions once to compile them before timing them
# This avoids timing the time it takes to actually compile the functions
naive_sum = log_likelihood_naive(X,nobs,isig,mu,det_sig)
vec_sum = log_likelihood_vec(X,nobs,isig,mu,det_sig)

for i=1:nsample
	tic()
	naive_sum = log_likelihood_naive(X,nobs,isig,mu,det_sig)
	naive_time += toq()

	tic()
	vec_sum = log_likelihood_vec(X,nobs,isig,mu,det_sig)
	vec_time += toq()
	
end

print(naive_sum)
print("\n")
print(vec_sum)

naive_time /= nsample
print("\nNaive time\n")
print(naive_time)

vec_time /= nsample
print("\n\nVectorized time\n")
print(vec_time)
print("\n")

