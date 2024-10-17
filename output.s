.data
var_2.500000: .word 0
var_k: .word 0
var_z: .word 0
var_y: .word 0
var_x: .word 0
var_2.625000: .word 0
var_b: .word 0
var_18.750000: .word 0
var_p: .word 0
.text
.globl main
main:
	lw $t0, var_18.750000
	move $t1, $t0
	sw $t1, var_p
	li $t2, 11
	sw $t2, var_b
	lw $t3, var_2.625000
	move $t4, $t3
	lw $t4, var_t2
	move $t5, $t4
	sw $t5, var_x
	lw $t5, var_x
	li $t6, 42
	add $t7, $t5, $t6
	lw $t7, var_t7
	move $t8, $t7
	sw $t8, var_y
	lw $t8, var_t8
	move $t9, $t8
	sw $t9, var_z
	li $t9, 12
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	add $t0, $t9, $t1
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	lw $t1, var_t9
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	move $t1, $t1
	sw $t1, var_p
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	lw $t2, var_t10
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	move $t2, $t2
	sw $t2, var_k
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	lw $t3, var_2.500000
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	move $t3, $t3
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
	addi $sp, $sp, -4
	sw $t6, 0($sp)
	lw $t6, var_x
	move $a0, $t6
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
