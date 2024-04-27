addi a0, zero, 8
jal ra, Fib
ecall
Fib:
addi t0, zero, 1
bge t0, a0, exit
add t0, t0, zero
addi t1, zero, 1
addi t2, zero, 2
L1:
add t3, t0, t1
add t0, t1, zero
add t1, t3, zero
addi t2, t2, 1
bge a0, t2, L1
add a0, zero, t1
exit:
jalr zero, 0(ra)