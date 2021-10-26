.extern a
.section drugasekcija
.word nesto
.skip 10
nesto:
push r1
ldr r5, $a
labeladva:
ldr r1, %a
jmp nesto
jmp 0x10
.end