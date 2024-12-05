.data
var_L2: .space 40
var_L3: .space 40
var_25.000000: .space 40
var_c: .space 40
var_L0: .space 40
var_true: .space 40
var_L1: .space 40
var_15.500000: .space 40
var_b: .space 40
var_a: .space 40
.text
.globl main
main:
	li $t0, 10
	sw $t0, var_a
	lw $t1, var_15.500000
	move $t2, $t1
	sw $t2, var_b
	li $v0, 1
	lw $t3, var_a
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t3, var_b
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	lw $t3, var_25.000000
	move $t4, $t3
	sw $t4, var_c
	li $v0, 1
	lw $t5, var_c
	move $a0, $t5
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t5, var_b
	move $a0, $t5
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
