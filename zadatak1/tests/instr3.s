.section .text
.global spoljasnji
.extern andrea
.word mika, spoljasnji
.extern a
.skip 10
mika:
spoljasnji: push r1
ldr r4, $a
vulence:
ldr r1, %mika
jmp andrea
jmp 0x10
.end