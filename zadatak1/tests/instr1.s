.global labela1
.section prva
xchg r3, r0
labela1: cmp r5, r2
xor r1, r3
push r5
pop r2
cmp r2, r5
.section druga
shr r5, r6
str r5, $labela1
str r6, 0x10
.end