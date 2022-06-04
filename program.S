; .globl _start
_start:
;	mov %eax, %ebx
;	movb %al, %al
;	movb %ah, %al
;	movb %bl, %al
;	movb %bh, %al
;	movb %cl, %al
;	movb %ch, %al
;	movb %dl, %al
;	movb %dh, %al

;	movb %al, %ah
;	movb %ah, %ah
;	movb %bl, %ah
;	movb %bh, %ah
;	movb %cl, %ah
;	movb %ch, %ah
;	movb %dl, %ah
;	movb %dh, %ah

;	inc %ebx
;	mov %eax, %ecx
;	mov %esi, %edi
;	mov %ecx, %eax
;	iret

;	mov %eax, %ebx
;	mov %edx, %ecx
;	mov %al, %bh

;	iret

	mov eax, ebx
	mov edx, ecx
	mov al, bh

	iret
