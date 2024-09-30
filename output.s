.data
var_y: .word 0
var_b: .word 0
var_a: .word 0
var_x: .word 0
var_c: .word 0
.text
.globl main
main:
	li $t0, 2
	sw $t0, var_c
	li $t1, 1
	sw $t1, var_x
	li $t2, 12
	sw $t2, var_a
	li $t3, 15
	li $t3, 38
	li $t3, 38
	sw $t3, var_b
	li $t4, 13
	li $t4, 25
	li $t4, 25
	sw $t4, var_x
	li $t5, 17
	li $t5, 19
	li $t5, 57
	li $t5, 69
	li $t5, 70
	li $t5, 70
	sw $t5, var_y
	li $v0, 10
	syscall
