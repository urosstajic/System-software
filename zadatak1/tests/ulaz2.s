#kom1
#kom2
.extern simbolcic1, flamingo
.global simbolcic2, simbolcic3
.equ simbolcic2, 16
.section prva
.word simbolcic2, simbolcic3, 58, 12, 020
.skip 5
bb: add r1, r3
aa: sub r2, r3
jne aa
simbolcic3: jmp bb
jne %flamingo
#kom3
.section cetvrta
jne %simbolcic3
.section treca
jne %simbolcic2
.end