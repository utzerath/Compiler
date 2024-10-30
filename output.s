.data
var_z: .space 40
var_y: .space 40
var_arr: .space 40
var_49.218750: .space 40
var_2.500000: .space 40
var_0.085366: .space 40
var_k: .space 40
var_2.625000: .space 40
var_x: .space 40
var_b: .space 40
.text
.globl main
main:
	li $t0, 11
	sw $t0, var_b
	lw $t1, var_2.625000
	move $t2, $t1
	sw $t2, var_x
	lw $t3, var_0.085366
	move $t4, $t3
	sw $t4, var_k
	li $t5, 3
	mul $t6, $t5, pg~ÌU
	lw $t5, var_49.218750
	add $t6, $t5, pg~ÌU
	li $t5, 0
	sll $t5, $t5, 2
	la $a0, var_arr
	add $a0, $a0, $t5
	li $t5, 5
	sw $t5, 0($a0)
	li $t5, 1
	sll $t5, $t5, 2
	la $a0, var_arr
	add $a0, $a0, $t5
	li $t5, 10
	sw $t5, 0($a0)
	li $t5, 2
	sll $t5, $t5, 2
	la $a0, var_arr
	add $a0, $a0, $t5
	li $t5, 15
	sw $t5, 0($a0)
	li $t5, 3
	sll $t5, $t5, 2
	la $a0, var_arr
	add $a0, $a0, $t5
	li $t5, 30
	sw $t5, 0($a0)
	li $t5, 30
	sw $t5, var_y
	li $t6, 25
	sw $t6, var_z
	li $v0, 1
	lw $t7, var_k
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t7, var_x
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	lw $t7, var_y
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
	li $v0, 1
	lw $t7, var_z
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	li $t7, 0
	sll $t7, $t7, 2
	la $a0, var_arr
	add $a0, $a0, $t7
	lw $t7, 0($a0)
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	li $t7, 1
	sll $t7, $t7, 2
	la $a0, var_arr
	add $a0, $a0, $t7
	lw $t7, 0($a0)
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	li $t7, 2
	sll $t7, $t7, 2
	la $a0, var_arr
	add $a0, $a0, $t7
	lw $t7, 0($a0)
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	li $t7, 3
	sll $t7, $t7, 2
	la $a0, var_arr
	add $a0, $a0, $t7
	lw $t7, 0($a0)
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
