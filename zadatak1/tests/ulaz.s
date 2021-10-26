.global simbolcic1, b
.extern flamingo
#malo da vidimo
.section prva
.word b, 5, 0x14
.section druga
a: add r1, r2
ldr r1, %b
push r6
simbolcic1: pop r5
b: ldr r5, 0x28
str r2, %simbolcic1
jmp *flamingo
.end