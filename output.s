.data
var_p: .space 40
var_workplz: .space 40
var_arr: .space 40
var_d: .space 40
var_t: .space 40
.text
.globl main
main:
	li $t0, 10
	lw $t1, var_t
	mul $t2, $t0, $t1
	lw $t0, var_d
	add $t1, (null), $t0
	lw $t0, var_t4
	move $t1, $t0
	sw $t1, var_p
	li $v0, 1
	lw $t1, var_p
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
