.extern b, dd
.global ww
.equ a, 8
.equ ww, 0x10
.section prva
ldr r4, a
andrea: ldr r4, %a
str r5, dd
str r5, %dd
.word 1, ww, 020
.section druga
jmp *a
str r5, $andrea
jne %a
.word andrea
.end