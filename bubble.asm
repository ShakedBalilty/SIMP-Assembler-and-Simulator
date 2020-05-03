		add		$sp,	$zero,	$imm,	2048		# set $sp = 2048
		add		$a0,	$zero,	$imm,	16			# n=16
		add		$a1,	$zero,	$imm,	1024		# beginning of array
		add		$t0,	$zero,	$zero				# $t0=c=0
		add		$t1,	$zero,	$zero				# $t1=d=0	
L1:
		sub		$s0,	$a0,	$imm,	1			# $s0=n-1
		ble		$imm,	$s0,	$t0,	EL1			# if(c>=n-1), jump to EL1
		sub		$s0,	$s0,	$t0					# $s0=n-c-1
L2:
		ble		$imm,	$s0,	$t1,	EL2			# if(d>=n-c-1), jump to EL2
		add		$t3,	$a1,	$t1					# &array[d]
		add		$t3,	$t3,	$imm,	1			# &array+d+1
		lw		$s1,	$t3,	$zero				# $s1=array[d+1]
		sub		$t3,	$t3,	$imm,	1			# &array[d]
		lw		$s2,	$t3,	$zero				# $s2=array[d]
		ble		$imm,	$s2,	$s1,	EIF			# if(array[d+1]>=array[d]), jump to EIF
		add		$t2,	$s2,	$zero				# swap=$t2=array[d]
		sw		$s1,	$t3,	$zero				# array[d]=array[d+1]	
		add		$t3,	$t3,	$imm,	1			# &array[d+1]
		sw		$t2,	$t3,	$zero				# &array[d+1]=swap
EIF:	
		add		$t1,	$t1,	$imm,	1			# d++
		jal		$imm,	$zero,	$zero,	L2			# jump to L2	
EL2:
		add		$t0,	$t0,	$imm,	1			# c++	
		add		$t1,	$zero,	$zero				# d=0
		jal		$imm,	$zero,	$zero,	L1			# jump to L1
EL1:
		add		$s0,	$zero,	$zero				# $s0=0		
		add		$s1,	$zero,	$zero				# $s1=0
		add		$s2,	$zero,	$zero				# $s2=0
		halt	$zero,	$zero,	$zero				# end

		.word	1024	23
		.word	1025	2
		.word	1026	6
		.word	1027	98
		.word	1028	55
		.word	1029	5
		.word	1030	8
		.word	1031	9
		.word	1032	10
		.word	1033	11
		.word	1034	12
		.word	1035	14
		.word	1036	13
		.word	1037	90
		.word	1038	89
		.word	1039	100