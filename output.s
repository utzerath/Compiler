.data
.text
.globl main
main:
	li $t0, 1
	sw $t0, t0
	li $t0, 12
	sw $t0, t1
	li $t0, 23
	sw $t0, t2
	li $t0, 10
	sw $t0, t3
	li $t0, 5
	sw $t0, t4
	li $t0, 15
	sw $t0, t5
	lw $t0, 23
	lw $t1, t6
	add $t2, $t0, $t1
	sw $t2, t7
	li $t0, 1
	sw $t0, t8
	lw $t0, a
	lw $t1, 1
	add $t2, $t0, $t1
	sw $t2, t9
	lw $t0, a
	lw $t1, t10
	add $t2, $t0, $t1
	sw $t2, t11
	li $t0, 5
	sw $t0, t12
	lw $t0, a
	lw $t1, 5
	add $t2, $t0, $t1
	sw $t2, t13
	lw $t0, c
	lw $t1, t14
	add $t2, $t0, $t1
	sw $t2, t15
	lw $t0, b
	lw $t1, t16
	add $t2, $t0, $t1
	sw $t2, t17
	lw $t0, a
	lw $t1, t18
	add $t2, $t0, $t1
	sw $t2, t19
	lw $t0, x
	lw $t1, t-1
	add $t2, $t0, $t1
	sw $t2, t-1
	li $v0, 10
	syscall
