.global labela1
.section prvasekcija
add r3, r0
labela1:
xchg r5, r2
xor r1, r3
push r5
pop r4
cmp r4, r5
.section drugasekcija
shr r5, r6
str r5, $labela1
str r6, 0x10
.end