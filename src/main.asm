org 0x7c00
bits 16

%define ENDL 0x0D, 0x0A

start:
  jmp main

; print string to screen
; params:
  ; ds:si points to a null-terminated string

puts:
  push si
  push ax

.loop:
  lodsb     ; load byte at si into al, increment si
  or al, al ; check if null terminator
  jz .done

  mov ah, 0x0e ; call bios interrupt
  mov bh, 0
  int 0x10

  jmp .loop

.done:
  pop ax
  pop si
  ret


main:
  ; setup data segments
  mov ax, 0
  mov ds, ax
  mov es, ax
    
  ; init stack
  mov ss, ax
  mov ax, 0x7c00 ; set to memory locatiion before bootloader since it grows down
  mov sp, ax

  ; print message
  lea si, [msg]
  call puts

msg: dw 'Hello World!', ENDL, 0

.halt:
  jmp .halt

times 510-($-$$) db 0
dw 0aa55h
