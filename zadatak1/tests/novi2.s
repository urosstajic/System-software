.global b
.extern ww
.section druga
add r5, r6
call *[r6 + ww]
mul r4, r2
.section prva
test r1, r2
jne %ww
.section treca
#malo komentarcica
b: .skip 4
a: div r1, r2
.word 85, 0x20, a
.end