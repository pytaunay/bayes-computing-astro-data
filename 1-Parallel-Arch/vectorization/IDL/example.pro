FUNCTION log_likelihood_naive, OBS, NOBS, ISIG, MU, DET_SIG 
	SUM = 0
	FOR i = 0,NOBS-1 DO BEGIN
		LV = OBS[*,i]-MU
		RV = TRANSPOSE(LV)
		SUM = SUM + LV ## ISIG ## RV
	ENDFOR

	; Size of a random variable
	N = SIZE(MU,/DIMENSIONS)

	SUM = SUM + NOBS*N*ALOG(2*!PI) + NOBS*ALOG(ABS(DET_SIG))
	SUM = -0.5*SUM

	RETURN, SUM
END

FUNCTION log_likelihood_vec, OBS, NOBS, ISIG, MU, DET_SIG 
	SUM = 0

	; Size of a random variable
	N = SIZE(MU,/DIMENSIONS)

	; Create a tempprary container for subtraction of X-mu
	TMP = MAKE_ARRAY(1,NOBS, VALUE=1)
	TMP = TMP ## MU
	LV = OBS-TMP

	; First part of the product
	TMP = LV ## ISIG

	; Right hand side
	RV = REFORM(LV,NOBS*N,1)
	RV = TRANSPOSE(RV)

	; Reform TMP into LV as a row-array
	LV = REFORM(TMP,NOBS*N,1)

	; Single sum through dot product
	SUM = LV ## RV

	SUM = SUM + NOBS*N*ALOG(2*!PI) + NOBS*ALOG(ABS(DET_SIG))
	SUM = -0.5*SUM

	RETURN, SUM
END


PRO idl_vectorization 
	;---------------- Read the data ----------------
	; MU
	mu_struct = READ_ASCII('/path/to/mu.csv',DELIMITER=',')
	MU = mu_struct.FIELD01

	; Sigma
	isig_struct = READ_ASCII('/path/to/inv_sigma.csv',DELIMITER=',')
	ISIG = isig_struct.FIELD01

	; Sigma
	dsig_struct = READ_ASCII('/path/to/det_sigma.csv',DELIMITER=',')
	DET_SIG= dsig_struct.FIELD1

	;----------------- Observations ------------------
	; Change the total number of observations to see how the code scales 
	NOBS = 100
	obs_struct = READ_ASCII('/path/to/random_vectors.csv',DELIMITER=',', NUM_RECORDS=NOBS)
	OBS = obs_struct.FIELD01

	;---------------- Perform calculations -----------------------
	; Number of samples for timing routines. Increase the number to get more accurate timing results
	NSAMPLE = 100
	NAIVE_AVE = 0
	VEC_AVE = 0
	FOR i = 0, NSAMPLE-1 DO BEGIN
		TIC = SYSTIME(/SECONDS)
		NAIVE_SUM = log_likelihood_naive(OBS,NOBS,ISIG,MU,DET_SIG)
		TOC = SYSTIME(/SECONDS)
		
		NAIVE_AVE = NAIVE_AVE + (TOC-TIC)

		TIC = SYSTIME(/SECONDS)
		VEC_SUM = log_likelihood_vec(OBS,NOBS,ISIG,MU,DET_SIG)
		TOC = SYSTIME(/SECONDS)

		VEC_AVE = VEC_AVE + (TOC-TIC)
	ENDFOR

	PRINT, "AVERAGE TIME (NAIVE):", NAIVE_AVE/NSAMPLE 
	PRINT, "AVERAGE TIME (VEC):", VEC_AVE/NSAMPLE 
	PRINT, "Results", "NAIVE","VEC"
	PRINT,  " ", NAIVE_SUM, VEC_SUM
END	
