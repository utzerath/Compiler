.data
var_2.500000: .word 0
var_k: .word 0
var_z: .word 0
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
	sw $t0, var_z
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	li $t1, 12
	lw $t1, 0($sp)
	addi $sp, $sp, 4
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	add $t1, $t1, $t1
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	lw $t2, var_t9
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	move $t2, $t2
	sw $t2, var_p
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	lw $t3, var_t10
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	move $t3, $t3
	sw $t3, var_k
	addi $sp, $sp, -4
	sw $t4, 0($sp)
	lw $t4, var_z
	addi $sp, $sp, -4
	sw $t4, 0($sp)
	lw $t4, var_t12
	addi $sp, $sp, -4
	sw $t4, 0($sp)
	add $t4, $t4, $t4
	addi $sp, $sp, -4
	sw $t5, 0($sp)
	lw $t5, var_t13
	addi $sp, $sp, -4
	sw $t5, 0($sp)
	move $t5, $t5
	sw $t5, var_z
	li $v0, 1
	addi $sp, $sp, -4
	sw $t6, 0($sp)
	lw $t6, var_k
	move $a0, $t6
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
	sw $t6, 0($sp)
	lw $t6, var_y
	move $a0, $t6
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	addi $sp, $sp, -4
	sw $t6, 0($sp)
	lw $t6, var_b
	move $a0, $t6
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 1
	addi $sp, $sp, -4
	sw $t6, 0($sp)
	lw $t6, var_z
	move $a0, $t6
	syscall
	li $v0, 11
	li $a0, 10
	syscall
	li $v0, 10
	syscall
