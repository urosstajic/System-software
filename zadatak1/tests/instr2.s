.global a, b, c, d
.section treca
.word 5, 4, 02, 0x15, 0x90, 22
add r5, r6
sub r5, r6
a: add r5, r6
.skip 8
d:
.skip 2
b:
.skip 0x10
c:
sub r4, r5
jne %c
.end