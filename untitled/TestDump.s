.text
.global foo

foo:

    pushq %rbp
    movq %rsp, %rbp
    subq $16, %rsp


    movl $5, %edx
    bola:
    imull %edx, %esi
    jmp bola

    leave
    ret
