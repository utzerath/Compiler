.data
var_m: .space 40
var_true: .space 40
var_a: .space 40
.text
.globl main
main:
	lw $t0, var_true
	move $t1, $t0
	sw $t1, var_a
	li $v0, 1
	lw $t2, var_a
	move $a0, $t2
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $t2, 10
	sw $t2, var_m
	li $v0, 1
	lw $t3, var_m
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
