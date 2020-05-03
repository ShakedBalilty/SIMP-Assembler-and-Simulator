		add $sp, $zero, $imm, 2048		# set $sp = 2048
		lw $a0, $zero, $imm, 1024		# get x from address 1024
		lw $a1, $zero, $imm, 1025		# get y from address 1025
		add $t0, $zero, $zero			# i=0
		sub $t1, $a0, $a1				# sum=x-y
LOOP:
		bgt $imm, $zero, $t1, AFLOOP	#LOOP condition
		add $t0, $t0, $imm, 1			# i++
		sub $t1, $t1, $a1				# sum=x-y
		jal $imm, $zero, $zero, LOOP	# jump back to LOOP
AFLOOP:
		add $s2, $zero, $imm, 1026		# set the address of i in memory
		add $t3, $zero, $imm, 1027		# set the address of result in memory
		add $t2, $t1, $a1				# sum+y	
		sw $t0, $zero, $s2				# set i in 1026 memory
		bne $imm, $t2, $zero, NONZERO	#if(sum+y!=0), jump to NONZERO
		sw $zero, $zero, $t3			# set remainder in 1027 memory 
NONZERO:	
		sw $t2, $zero, $t3				# set remainder in 1027 memory 
		halt $zero, $zero, $zero		# halt
		.word 1024 0x1244
		.word 1025 0x12