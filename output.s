.data
var_k: .word 0
var_y: .word 0
var_x: .word 0
var_10.500000: .word 0
var_b: .word 0
var_18.750000: .word 0
var_p: .word 0
.text
.globl main
main:
	lw $t0, var_18.750000
	move $t1, $t0
	sw $t1, var_p
	lw $t2, var_t0
	move $t3, $t2
	sw $t3, var_b
	lw $t3, var_t2
	move $t4, $t3
	sw $t4, var_x
	lw $t4, var_b
	li $t5, 19
	add $t6, $t4, $t5
	li $t6, 12
	lw $t7, var_t5
	add $t8, $t6, $t7
	lw $t7, var_x
	lw $t8, var_t6
	add $t9, $t7, $t8
	lw $t8, var_t7
	move $t9, $t8
	sw $t9, var_y
	lw $t9, var_t8
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	move $t0, $t9
	sw $t0, var_k
	li $v0, 1
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	lw $t1, var_k
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	move $a0, $t7
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	lw $t1, var_y
	move $a0, $t1
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	move $a0, $t4
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
