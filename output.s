.data
var_y: .word 0
var_x: .word 0
var_b: .word 0
.text
.globl main
main:
	li $t0, 38
	sw $t0, var_b
	li $t1, 25
	sw $t1, var_x
	li $t2, 94
	sw $t2, var_y
	li $v0, 1
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	move $a0, $t2
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	move $a0, $t0
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
