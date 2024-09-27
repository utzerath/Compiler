.data
y: .word 0
c: .word 0
b: .word 0
a: .word 0
x: .word 0
.text
.globl main
main:
	li $t0, 1
	sw $t0, x
	li $t0, 12
	sw $t0, a
	li $t0, 15
	li $t0, 23
	lw $t1, t0
	add $t2, $t0, $t1
	lw $t0, t1
	sw $t0, b
	lw $t0, a
	li $t1, 1
	add $t2, $t0, $t1
	lw $t0, a
	lw $t1, t2
	add $t2, $t0, $t1
	lw $t0, t3
	sw $t0, x
	lw $t0, a
	li $t1, 5
	add $t2, $t0, $t1
	lw $t0, c
	lw $t1, t4
	add $t2, $t0, $t1
	lw $t0, b
	lw $t1, t5
	add $t2, $t0, $t1
	lw $t0, a
	lw $t1, t6
	add $t2, $t0, $t1
	lw $t0, x
	lw $t1, t7
	add $t2, $t0, $t1
	lw $t0, t8
	sw $t0, y
	li $v0, 10
	syscall
