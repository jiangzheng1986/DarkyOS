#ifndef HARDWARE_H
#define HARDWARE_H

#define ADR_BOOTINFO			0x00000ff0
#define ADR_DISKIMG             0x00100000
#define ADR_IDT					0x0026f800
#define LIMIT_IDT				0x000007ff
#define ADR_GDT					0x00270000
#define LIMIT_GDT				0x0000ffff
#define ADR_BOTPAK				0x00280000
#define LIMIT_BOTPAK			0x0007ffff
#define AR_DATA32_RW			0x4092
#define AR_CODE32_ER			0x409a
#define AR_INTGATE32			0x008e
#define AR_TSS32				0x0089

#define PIC0_ICW1				0x0020
#define PIC0_OCW2				0x0020
#define PIC0_IMR				0x0021
#define PIC0_ICW2				0x0021
#define PIC0_ICW3				0x0021
#define PIC0_ICW4				0x0021
#define PIC1_ICW1				0x00a0
#define PIC1_OCW2				0x00a0
#define PIC1_IMR				0x00a1
#define PIC1_ICW2				0x00a1
#define PIC1_ICW3				0x00a1
#define PIC1_ICW4				0x00a1

#define PORT_KEYDAT				0x0060
#define PORT_KEYSTA				0x0064
#define PORT_KEYCMD				0x0064
#define KEYSTA_SEND_NOTREADY	0x02
#define KEYCMD_WRITE_MODE		0x60
#define KBC_MODE				0x47
#define KEYCMD_SENDTO_MOUSE		0xd4
#define MOUSECMD_ENABLE			0xf4

#define PIT_CTRL				0x0043
#define PIT_CNT0				0x0040

void io_nop(void);
void io_hlt(void);
void io_cli(void);
void io_sti(void);
void io_stihlt(void);
int io_in8(int port);
int io_in16(int port);
int io_in32(int port);
void io_out8(int port, int data);
void io_out16(int port, int data);
void io_out32(int port, int data);
int io_load_eflags(void);
void io_store_eflags(int eflags);
void load_gdtr(int limit, int addr);
void load_idtr(int limit, int addr);
int load_cr0();
void store_cr0(int cr0);
void load_tr(int tr);
void taskswitch4(void);
void taskswitch3(void);
void farjmp(int eip, int cs);

#endif
