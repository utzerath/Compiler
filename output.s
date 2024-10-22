.data
var_y: .space 40
var_arr: .space 40
var_‡®`: .space 40
var_: .space 40
var_2.500000: .space 40
var_k: .space 40
var_z: .space 40
var_êü`: .space 40
var_x: .space 40
var_2.625000: .space 40
var_b: .space 40
var_18.750000: .space 40
var_p: .space 40
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
	move $t5, $t4
	sw $t5, var_x
	lw $t5, var_x
	li $t6, 42
	add $t7, $t5, $t6
	sw $t7, var_êü`
	lw $t5, var_x
	lw $t6, var_p
	mul $t8, $t5, $t6
	lw $t5, var_t8
	move $t6, $t5
	sw $t6, var_z
	li $t6, 12
	lw $t8, var_p
	add $t9, $t6, $t8
	lw $t6, var_t9
	move $t8, $t6
	sw $t8, var_p
	lw $t8, var_x
	lw $t9, var_p
	addi $sp, $sp, -4
	sw $t0, 0($sp)
	div $t0, $t8, $t9
	lw $t8, var_t10
	move $t9, $t8
	sw $t9, var_k
	lw $t9, var_2.500000
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	move $t1, $t9
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	li $t1, 3
	addi $sp, $sp, -4
	sw $t1, 0($sp)
	mul $t1, $t1, (null)
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	lw $t2, var_z
	addi $sp, $sp, -4
	sw $t2, 0($sp)
	add $t2, $t2, (null)
	sw $t2, var_
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	lw $t3, var_p
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	lw $t3, var_x
	addi $sp, $sp, -4
	sw $t3, 0($sp)
	mul $t3, $t3, $t3
	addi $sp, $sp, -4
	sw $t4, 0($sp)
	lw $t4, var_x
	addi $sp, $sp, -4
	sw $t4, 0($sp)
	div $t4, (null), $t4
	sw $t4, var_‡®`
	addi $sp, $sp, -4
	sw $t5, 0($sp)
	li $t5, 0
	sll $t5, $t5, 2
	la $a0, var_arr
	add $a0, $a0, $t5
	addi $sp, $sp, -4
	sw $t6, 0($sp)
	li $t6, 5
	sw $t6, 0($a0)
	addi $sp, $sp, -4
	sw $t7, 0($sp)
	li $t7, 1
	sll $t7, $t7, 2
	la $a0, var_arr
	add $a0, $a0, $t7
	addi $sp, $sp, -4
	sw $t8, 0($sp)
	li $t8, 10
	sw $t8, 0($a0)
	addi $sp, $sp, -4
	sw $t9, 0($sp)
	li $t9, 2
	sll $t9, $t9, 2
	la $a0, var_arr
	add $a0, $a0, $t9
