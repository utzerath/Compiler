.data
var_arr2: .space 40
var_optopus1: .space 40
var_p: .space 40
.text
.globl main
main:
	li $t0, 53
	sw $t0, var_p
	li $t1, 9
	sw $t1, var_optopus1
	li $t2, 10
	sw $t2, var_arr2
	li $v0, 1
	lw $t3, var_optopus1
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t3, var_arr2
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t3, var_p
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
