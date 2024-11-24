.data
var_m: .space 40
var_L4: .space 40
var_L3: .space 40
var_L1: .space 40
var_L2: .space 40
var_L0: .space 40
var_false: .space 40
var_b: .space 40
var_true: .space 40
var_a: .space 40
.text
.globl main
main:
	lw $t0, var_true
	move $t1, $t0
	sw $t1, var_a
	lw $t2, var_false
	move $t3, $t2
	sw $t3, var_b
	li $v0, 1
	lw $t4, var_a
	move $a0, $t4
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t4, var_b
	move $a0, $t4
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $t4, 10
	sw $t4, var_m
	li $v0, 1
	lw $t5, var_m
	move $a0, $t5
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
