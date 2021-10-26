.global flamingo #malo komentara
.extern b, simbolcic2
.section treca
flamingo: add r1, r4
ldr r4, simbolcic2
jne %b
.section druga
jne *b
jne *[r3 + flamingo]
.end