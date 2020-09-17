.text
FUNCTION_func:
subu $sp,$sp,88
sw $fp,0($sp)
sw $t0,4($sp)
sw $t1,8($sp)
sw $t2,12($sp)
sw $t3,16($sp)
sw $t4,20($sp)
sw $t5,24($sp)
sw $t6,28($sp)
sw $t7,32($sp)
sw $t8,36($sp)
sw $t9,40($sp)
sw $s0,44($sp)
sw $s1,48($sp)
sw $s2,52($sp)
sw $s3,56($sp)
sw $s4,60($sp)
sw $s5,64($sp)
sw $s6,68($sp)
sw $s7,72($sp)
sw $ra,76($sp)
addu $fp,$sp,88
lw $t2, 0($fp)
lw $t3, 4($fp)
add $t1,$t2,$t3
sw $t1, -8($fp)
lw $t4, -8($fp)
sw $t4, -4($fp)
lw $t5, -4($fp)
move $v0,$t5
b FUNCTION_func_END
FUNCTION_func_END:
lw $fp,0($sp)
lw $t0,4($sp)
lw $t1,8($sp)
lw $t2,12($sp)
lw $t3,16($sp)
lw $t4,20($sp)
lw $t5,24($sp)
lw $t6,28($sp)
lw $t7,32($sp)
lw $t8,36($sp)
lw $t9,40($sp)
lw $s0,44($sp)
lw $s1,48($sp)
lw $s2,52($sp)
lw $s3,56($sp)
lw $s4,60($sp)
lw $s5,64($sp)
lw $s6,68($sp)
lw $s7,72($sp)
lw $ra,76($sp)
addu $sp,88
jr $ra
.text
main:
FUNCTION_main:
subu $sp,$sp,104
sw $fp,8($sp)
sw $t0,12($sp)
sw $t1,16($sp)
sw $t2,20($sp)
sw $t3,24($sp)
sw $t4,28($sp)
sw $t5,32($sp)
sw $t6,36($sp)
sw $t7,40($sp)
sw $t8,44($sp)
sw $t9,48($sp)
sw $s0,52($sp)
sw $s1,56($sp)
sw $s2,60($sp)
sw $s3,64($sp)
sw $s4,68($sp)
sw $s5,72($sp)
sw $s6,76($sp)
sw $s7,80($sp)
sw $ra,84($sp)
addu $fp,$sp,104
li $t6, 2
sw $t6, -4($fp)
li $t7, 3
sw $t7, -8($fp)
lw $t9, -4($fp)
move $a0, $t9
sw $t9, 0($sp)
lw $s0, -8($fp)
move $a1, $s0
sw $s0, 4($sp)
jal FUNCTION_func
move $t8,$v0
sw $t8, -16($fp)
lw $s1, -16($fp)
sw $s1, -12($fp)
lw $s3, -12($fp)
move $a0,$s3
li $v0,0x01
syscall
li $v0,0
b FUNCTION_main_END
FUNCTION_main_END:
lw $fp,8($sp)
lw $t0,12($sp)
lw $t1,16($sp)
lw $t2,20($sp)
lw $t3,24($sp)
lw $t4,28($sp)
lw $t5,32($sp)
lw $t6,36($sp)
lw $t7,40($sp)
lw $t8,44($sp)
lw $t9,48($sp)
lw $s0,52($sp)
lw $s1,56($sp)
lw $s2,60($sp)
lw $s3,64($sp)
lw $s4,68($sp)
lw $s5,72($sp)
lw $s6,76($sp)
lw $s7,80($sp)
lw $ra,84($sp)
addu $sp,104
jr $ra
