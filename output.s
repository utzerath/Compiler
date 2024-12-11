.data
var_25.000000: .space 40
var_c: .space 40
var_a: .space 40
.text
.globl main
main:
	li $t0, 10
	sw $t0, var_a
	li $v0, 1
	lw $t1, var_a
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	lw $t1, var_25.000000
	move $t2, $t1
	sw $t2, var_c
	li $v0, 1
	lw $t3, var_c
	move $a0, $t3
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
