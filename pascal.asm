			add	$sp,	$zero,	$imm,	2048		# $sp=2048
			lw	$s0,	$zero,	$imm,	1023		# $s0=n=MEMORY[1023]
			add	$s0,	$s0,	$imm,	1			# n++
			sub $s1,	$imm,	$s0,	1023		# $s1 help pointer
			add $s2,	$zero,	$imm,	1024		# $s2 memory array pointer
			add $t0,	$zero,	$imm,	0			# $t0=i=0
			add $t1,	$zero,	$imm,	0			# $t1=j=0
RFOR:
			beq $imm,	$t0,	$s0,	ERFOR		# if (i>=n), jump to ERFOR
			add $t1,	$zero,	$imm,	0			# j=0	
LFOR:
			bgt $imm,	$t1,	$t0,	ENDFORL		#if (j>i), jump to ENDFORL
			beq $imm,	$t1,	$zero,	ELSE		#if(j!=0), jump to ELSE
			beq $imm,	$t0,	$zero,	ELSE		#&&(i!=0), jump to ELSE 
			beq $imm,	$t1,	$t0,	ELSE		#&&(j!=i), jump to ELSE
			sub $t2,	$t1,	$imm,	1			# $t2=j-1			
			lw	$t2,	$s2,	$t2					# $t2=memory[j-1] 
			lw	$t3,	$s2,	$t1					# $t3=memory[j] 
			add $t2,	$t2,	$t3					# $t2+$t3=memory[j-1]+memory[j] 
			sw	$t2,	$t1,	$s1					# store memory[j]+memory[j-1]
			jal $imm,	$zero,	$zero,	ENDIF		# jump to ENDIF	
ELSE:
			sw	$imm,	$t1,	$s1,	1			# aug[j]=1
ENDIF:
			add	$t1,	$t1,	$imm,	1			# j++
			jal	$imm,	$zero,	$zero,	LFOR		# jump to LFOR
ENDFORL:
			add $t1,	$zero,	$imm,	0			# $t1=j=0
WFOR:
			beq	$imm,	$s0,	$t1,	EWFOR	# if(j>=n), jump to EWFOR
			lw	$t3,	$s1,	$t1					# $t3)=aug[j]
			sw	$t3,	$t1,	$s2					# mem[j]=aug[j]=$t3
			add $t1,	$t1,	$imm,	1			# j++
			jal	$imm,	$zero,	$zero, WFOR  # jump to WFOR
EWFOR:
			add $t0,	$t0,	$imm,	1			# i++
			jal	$imm,	$zero,	$zero,	RFOR		# jump to EWFOR
ERFOR:
			halt $zero,	$zero,	$zero				# end
			.word 1023 4							# n=4