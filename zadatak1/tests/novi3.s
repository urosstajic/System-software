.global labela2, labela3
.extern labela1, uvezen
.equ labela2, 16
.section prvasekcija
#neki komentar
.word 12, labela3, 37, labela2, 0x10
.skip 5
bb: 
mul r3, r5
aa: 
cmp r2, r3 #neki kom
jne aa
labela3: 
jeq bb
jne %uvezen
.section cetvrta
jmp %labela3
halt
.section trecasekcija
jeq %labela2
.end