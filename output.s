.data
var_m: .space 40
var_L4: .space 40
var_L3: .space 40
var_c: .space 40
var_L1: .space 40
var_L2: .space 40
var_L0: .space 40
var_false: .space 40
var_b: .space 40
var_true: .space 40
var_a: .space 40
var_work: .space 40
var_param1: .space 40
var_x: .space 40
.text
.globl main
main:
	lw $t0, var_param1
	move $t1, $t0
	sw $t1, var_x
	li $t2, 10
	lw $t3, var_true
	move $t4, $t3
	sw $t4, var_a
	lw $t5, var_false
	move $t6, $t5
	sw $t6, var_b
	li $v0, 1
	lw $t7, var_a
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t7, var_b
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	lw $t7, var_t3
	move $t8, $t7
	sw $t8, var_c
	li $t8, 5
	move $t9, $t8
	sw $t9, var_param1
	lw $t9, var_t5
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	move $t0, $t9
	sw $t0, var_m
	li $v0, 1
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	lw $t1, var_m
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
