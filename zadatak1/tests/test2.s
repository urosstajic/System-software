.global a, b, c, d
.section trecasekcija
.word 5, 6, 0x15, 33, 0x90
add r4, r5
sub r4, r5
a:
add r4, r4
.skip 8
d:
.skip 5
b:
.skip 0x10
c:
sub r4, r5
jne %c
.end