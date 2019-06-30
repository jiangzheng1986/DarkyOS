
[FORMAT "WCOFF"]
[INSTRSET "i486p"]
[BITS 32]
[FILE "naskfunc.nas"]

		GLOBAL	_io_nop, _io_hlt, _io_cli, _io_sti, _io_stihlt
		GLOBAL	_io_in8, _io_in16, _io_in32
		GLOBAL	_io_out8, _io_out16, _io_out32
		GLOBAL	_io_load_eflags, _io_store_eflags
		GLOBAL	_load_gdtr, _load_idtr
		GLOBAL	_asm_inthandler20, _asm_inthandler21, _asm_inthandler27, _asm_inthandler2c
		GLOBAL	_load_cr0, _store_cr0
		GLOBAL	_load_tr, _taskswitch4, _taskswitch3, _farjmp
		EXTERN	_inthandler20, _inthandler21, _inthandler27, _inthandler2c		
		
[SECTION .text]

; void io_nop(void);
_io_nop:
		NOP
		RET

; void io_hlt(void);
_io_hlt:
		HLT
		RET
		
;void io_cli(void);
_io_cli:
		CLI
		RET
		
;void io_sti(void);
_io_sti:
		STI
		RET
		
;void io_stihlt(void);
_io_stihlt:
		STI
		HLT
		RET
		
;int io_in8(int port);
_io_in8:
		MOV		EDX, [ESP+4]		;port
		MOV		EAX, 0
		IN		AL, DX
		RET
		
;int io_in16(int port);
_io_in16:
		MOV		EDX, [ESP+4]		;port
		MOV		EAX, 0
		IN		AX, DX
		RET
		
;int io_in32(int port);
_io_in32:
		MOV		EDX, [ESP+4]		;port
		MOV		EAX, 0
		IN		EAX, DX
		RET

;void io_out8(int port, int data);
_io_out8:
		MOV		EDX, [ESP+4]		;port
		MOV		EAX, [ESP+8]		;data
		OUT		DX, AL
		RET
		
;void io_out16(int port, int data);
_io_out16:
		MOV		EDX, [ESP+4]		;port
		MOV		EAX, [ESP+8]		;data
		OUT		DX, AX
		RET
		
;void io_out32(int port, int data);
_io_out32:
		MOV		EDX, [ESP+4]		;port
		MOV		EAX, [ESP+8]		;data
		OUT		DX, EAX
		RET

;int io_load_eflags(void);
_io_load_eflags:
		PUSHFD
		POP		EAX					;返回值
		RET

;void io_store_eflags(int eflags);
_io_store_eflags:
		MOV		EAX, [ESP+4]		;eflags
		PUSH	EAX
		POPFD
		RET

;void load_gdtr(int limit, int addr);
_load_gdtr:
		MOV		AX, [ESP+4]			;limit
		MOV		[ESP+6],AX
		LGDT	[ESP+6]
		RET
		
;void load_idtr(int limit, int addr);
_load_idtr:
		MOV		AX, [ESP+4]			;limit
		MOV		[ESP+6],AX
		LIDT	[ESP+6]
		RET
		
;int load_cr0();
_load_cr0:
		MOV		EAX, CR0
		RET
		
;void store_cr0(int cr0);
_store_cr0:
		MOV		EAX, [ESP+4]
		MOV		CR0, EAX
		RET
		
;void asm_inthandler20();
_asm_inthandler20:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX, ESP
		PUSH	EAX
		MOV		AX, SS
		MOV		DS, AX
		MOV		ES, AX
		CALL	_inthandler20
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
		
;void asm_inthandler21();
_asm_inthandler21:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX, ESP
		PUSH	EAX
		MOV		AX, SS
		MOV		DS, AX
		MOV		ES, AX
		CALL	_inthandler21
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
		
;void asm_inthandler27();
_asm_inthandler27:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX, ESP
		PUSH	EAX
		MOV		AX, SS
		MOV		DS, AX
		MOV		ES, AX
		CALL	_inthandler27
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
		
;void asm_inthandler2c();
_asm_inthandler2c:
		PUSH	ES
		PUSH	DS
		PUSHAD
		MOV		EAX, ESP
		PUSH	EAX
		MOV		AX, SS
		MOV		DS, AX
		MOV		ES, AX
		CALL	_inthandler2c
		POP		EAX
		POPAD
		POP		DS
		POP		ES
		IRETD
		
;void load_tr(int tr);
_load_tr:
		LTR		[ESP + 4]
		RET

;void taskswitch4(void);
_taskswitch4:
		JMP		4*8:0
		RET
		
;void taskswitch3(void);
_taskswitch3:
		JMP		3*8:0
		RET
		
;void farjmp(int eip, int cs);
_farjmp:
		JMP		FAR [ESP+4]		;eip cs
		RET