.global aa, bb, cc, dd
.section treca
call 0x55
aa: 
bb:
cc: add r1, r2
dd: .word 24
jne %aa
.section prva
jne %dd
.end