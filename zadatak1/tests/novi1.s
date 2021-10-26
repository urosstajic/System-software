.extern uvezen
.global labela1, nekob
#kommmmmmm
.section prvasekcija
.word 6, nekob, 0x20
.section drugasekcija
nekoa: 
add r2, r3
str r1, %nekob #komentar
pop r1
labela1: 
push r4
nekob: 
ldr r2, 0x11
ldr r5, %labela1
jmp *uvezen
.end