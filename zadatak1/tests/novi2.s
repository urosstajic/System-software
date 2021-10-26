.global uvezen
.extern nekob, labela2
.section trecasekcija
uvezen: 
#komentar neki
sub r4, r3
ldr r0, labela2
jmp %nekob
#opet komentar
.section drugasekcija
jeq *nekob
jne *[r1 + uvezen]
ret
.end