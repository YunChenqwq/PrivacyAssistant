org 0x7c00 
start: 
 mov ax,cs
 mov ds,ax 
 mov ss,ax
 mov es,ax
 mov sp,0x100 ;开辟0x100字节的堆栈空间
 main:
	mov bp,msg1		
	mov cx,msg1_Len 
	mov ax,0x1301  
	mov bx,0x1a   ;蓝底绿字护眼
	mov dl,0		;段号
    int 0x10 ;调用BIOS的显示服务。
  mov ax,0xb800 ;显示缓冲区
  add ax,0xA0   ;定位到第二行
  mov ds,ax		
  xor cx,cx     
  xor bx,bx
  GetChar:
   xor ax,ax
   int 0x16		;键盘中断
   cmp AL,0x8  ;退格键 if al==0x8
   je back
   CMP al,0x0d ;回车键
   je entry
   mov ah,2
   mov [bx],al
   mov [bx+1],ah
   add bx,2
   inc cx
   jmp GetChar
   back:
   sub bx,2
   dec cx
   xor ax,ax
   mov [bx],ax
   jmp  GetChar
   entry:
   ;逐个字符比较
   mov ax,cs
   mov es,ax
   xor bx,bx ;bx = 0
   mov si,Key  ;si 指向真正的密码   bx指向输入的密码
   mov cl,[cs:KeySize1]
   mov ch,0 ;cx
   cmp_key:
    mov al,[ds:bx] 
	mov ah,[es:si]
	cmp al,ah
	jne key_err ;不相同,退出
	add bx,2 
	inc si
	loop cmp_key
	
	xor ax,ax ;初始化
	mov ax,0x7e00
	mov es,ax
	xor bx,bx
	mov ah,0x2
	mov dl,0x80
	mov al,1  ;数量
	mov dh,0  ;磁头
	mov ch,0  ;柱面  ；
	mov cl,3  ;扇区  ；
	int 0x13
	;写回去
	xor bx,bx
	mov dl,0x80
	mov ah,0x3
	mov al,1  ;数量
	mov dh,0  ;磁头
	mov ch,0  ;柱面
	mov cl,1  ;1扇区
	int 0x13
	jmp _REST  ;重启计算机
	key_err:
	mov bx,0xb800
	add bx,msg1_Len
	mov al,'X'
	mov [bx],al
   mov cx,[cs:KeySize1]
   xor ax,ax
   kk:  ;对输入的清0
    mov [bx],ax
	add bx,2
   loop kk
  jmp start
  ;重启计算机
_REST:
mov ax,0xffff
push ax
mov ax,0
push ax
retf
data:
msg1:	db "This Disk already has Diskprotection LOCK by PrivacyAssistan,Please enter the password to unlock",0AH,0DH
msg2:	db "Github:https://github.com/YunChenqwq/PrivacyAssistant",0AH,0DH
msg3:	db "Writer Name:Wushangze Xiya",0AH,0DH
msg4:	db "(*＾-＾*)",0AH,0DH
msg5:	db "If you forget, you can unlock it through the unlock .exe",0AH,0DH
msg1_Len equ  $-msg1
msg2_Len equ  $-msg2
msg3_Len equ  $-msg3
msg4_Len equ  $-msg4
msg5_Len equ  $-msg5
KeySize1:db 10
Key:db 'AAAAAAAAAA' ;也不能有中文
times 510-($-$$)  db 0xF
dw 0xAA55