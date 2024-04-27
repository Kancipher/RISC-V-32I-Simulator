addi s0, x0, 2000
lw a0, 0(s0)
lw a1, 36(s0)
jal ra, Sum
ecall
sum:
addi sp, sp, -8
sw ra, 0(sp)
sw a0, 4(sp)
blt a1, a0, exit
addi a0, a0, 1
jal ra, sum
lw t0, 4(sp)
lw ra, 0(sp)
addi sp, sp, 8
add a0, a0, t0
jalr x0, 0(ra)
exit: 
addi a0, x0, 0
addi sp, sp, 8
jalr zero, 0(ra)
