
[INSTRSET "i486p"]

VBEMODE	EQU		0x105
;VBEMODE	EQU		0x107

BOTPAK	EQU		0x00280000		
DSKCAC	EQU		0x00100000
DSKCAC0	EQU		0x00008000

CYLS	EQU		0x0ff0				;柱面数
LEDS	EQU		0x0ff1
VMODE	EQU		0x0ff2				;视频模式
SCRNX	EQU		0x0ff4				;屏幕分辨率X
SCRNY	EQU		0x0ff6				;屏幕分辨率Y
VRAM	EQU		0x0ff8				;显存地址

		ORG		0xc200
		
;显示模式
	
		;JMP		scrn320

		MOV		AX, 0x9000
		MOV		ES, AX
		MOV		DI, 0
		MOV		AX, 0x4f00
		INT		0x10				;确认VBE是否存在
		CMP		AX, 0x004f
		JNE		scrn320
		
		MOV		AX, [ES:DI+4]		;检查VBE版本
		CMP		AX, 0x0200
		JB		scrn320
		
		MOV		CX, VBEMODE
		MOV		AX, 0x4f01
		INT		0x10				;取得画面模式信息
		CMP		AX, 0x004f
		JNE		scrn320
		
		CMP		BYTE [ES:DI+0x19], 8
		JNE		scrn320
		CMP		BYTE [ES:DI+0x1b], 4
		JNE		scrn320
		MOV		AX, [ES:DI+0x00]
		AND		AX, 0x0080
		JZ		scrn320
		
		MOV		BX, VBEMODE + 0x4000
		MOV		AX, 0x4f02
		INT		0x10
		MOV		BYTE [VMODE], 8
		MOV		AX, [ES:DI + 0x12]
		MOV		WORD [SCRNX], AX
		MOV		AX, [ES:DI + 0x14]
		MOV		WORD [SCRNY], AX
		MOV		EAX, [ES:DI + 0x28]
		MOV		DWORD [VRAM], EAX
		JMP		scrn_over

scrn320:
		MOV		AL, 0x13			;VGA 320*200*8bit模式
		MOV		AH, 0x00
		INT		0x10
		MOV		BYTE [VMODE], 8
		MOV		WORD [SCRNX], 320
		MOV		WORD [SCRNY], 200
		MOV		DWORD [VRAM], 0x000a0000
		
scrn_over:
		
;获取LED状态

		MOV		AH, 0x02
		INT		0x16				;键盘BIOS
		MOV		[LEDS], AL
		
;初始化中断控制器PIC

		MOV		AL, 0xff
		OUT		0x21, AL			;主PIC IMR终端屏蔽寄存器设置
		NOP
		OUT		0xa1, AL			;从PIC IMR终端屏蔽寄存器设置
		
		CLI
		
;A20地址线设置

		CALL	waitkbdout
		MOV		AL, 0xd1
		OUT		0x64, AL
		CALL	waitkbdout
		MOV		AL, 0xdf
		OUT		0x60, AL
		CALL	waitkbdout
		
;进入保护模式
		
[INSTRSET "i486p"]

		LGDT	[GDTR0]
		MOV		EAX, CR0
		AND		EAX, 0x7fffffff		;关闭分页
		OR		EAX, 0x00000001		;开启保护模式
		MOV		CR0, EAX
		JMP		pipelineflush
pipelineflush:
		MOV		AX, 1*8
		MOV		DS, AX
		MOV		ES, AX
		MOV		FS, AX
		MOV		GS, AX
		MOV		SS, AX
		
;bootpack传送

		MOV		ESI, bootpack
		MOV		EDI, BOTPAK
		MOV		ECX, 512 * 1024 / 4			;512K
		CALL	memcpy
		
;磁盘数据最终传送到0x00100000的位置去

;首先重启动扇区开始

		MOV		ESI, 0x7c00
		MOV		EDI, DSKCAC
		MOV		ECX, 512 / 4
		CALL	memcpy
		
;剩余部分

		MOV		ESI, DSKCAC0+512
		MOV		EDI, DSKCAC+512
		MOV		ECX, 0
		MOV		CL, BYTE[CYLS]
		IMUL	ECX, 512 * 18 * 2 / 4		;柱面数转字节数
		SUB		ECX, 512 / 4				;减去IPL大小
		CALL	memcpy
		
;启动bootpack

		MOV		EBX, BOTPAK
		MOV		ECX, [EBX+16]
		ADD		ECX, 3
		SHR		ECX, 2
		JZ		skip
		MOV		ESI, [EBX+20]
		ADD		ESI, EBX
		MOV		EDI, [EBX+12]
		CALL	memcpy						;额外需要的数据传送
skip:
		MOV		ESP, [EBX+12]
		JMP		DWORD 2*8:0x0000001b
		
		
waitkbdout:
		IN		AL, 0x64
		AND		AL, 0x02
		JNZ		waitkbdout
		RET
		
memcpy:
		MOV		EAX, [ESI]
		ADD		ESI, 4
		MOV		[EDI], EAX
		ADD		EDI, 4
		SUB		ECX, 1
		JNZ		memcpy
		RET
		
		ALIGNB	16
		
GDT0:
		RESB	8									;不使用,置0
		DW		0xffff, 0x0000, 0x9200, 0x00cf		;可读可写
		DW		0xffff, 0x0000, 0x9a28, 0x0047		;可执行
		
		DW		0
GDTR0:
		DW		8*3-1
		DD		GDT0
		
		ALIGNB	16
bootpack:

		