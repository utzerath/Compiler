.data
var_x: .space 40
.text
.globl main
main:
	li $t0, 10
	sw $t0, var_x
	li $v0, 1
	lw $t1, var_x
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
