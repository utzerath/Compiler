.data
y: .word 0
b: .word 0
a: .word 0
x: .word 0
c: .word 0
.text
.globl main
main:
	li $t0, 2
	sw $t0, c
	li $t0, 1
	sw $t0, x
	li $t0, 12
	sw $t0, a
	li $t0, 15
	li $t0, 38
	li $t0, 38
	sw $t0, b
	li $t0, 13
	li $t0, 25
	li $t0, 25
	sw $t0, x
	li $t0, 17
	li $t0, 19
	li $t0, 57
	li $t0, 69
	li $t0, 70
	li $t0, 70
	sw $t0, y
	li $v0, 10
	syscall
