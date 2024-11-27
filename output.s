.data
var_L3: .space 40
var_0.000000: .space 40
var_L0: .space 40
var_15.500000: .space 40
var_b: .space 40
.text
.globl main
main:
	lw $t0, var_15.500000
	move $t1, $t0
	sw $t1, var_b
	li $v0, 1
	lw $t2, var_b
	move $a0, $t2
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t2, var_b
	move $a0, $t2
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
