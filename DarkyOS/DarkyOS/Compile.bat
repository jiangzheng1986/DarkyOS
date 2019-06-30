del haribote.img

md listings

..\z_tools\makefont.exe src/Ascii.txt Ascii.bin

..\z_tools\bin2obj.exe Ascii.bin Ascii.obj _ascii

..\z_tools\nask.exe src/asm/Boot.nas Boot.bin listings/Boot.lst

..\z_tools\nask.exe src/asm/KernelLoader.nas KernelLoader.bin listings/KernelLoader.lst

..\z_tools\nask.exe src/asm/Hardware.nas Hardware.obj listings/Hardware.lst

..\z_tools\cc1.exe -I..\z_tools\darkyos\ -Os -Wall -quiet -o Kernel.gas src/kernel/Kernel.c

..\z_tools\gas2nask.exe -a Kernel.gas Kernel.nas

..\z_tools\nask.exe Kernel.nas Kernel.obj listings/Kernel.lst

..\z_tools\obj2bim.exe @../z_tools/darkyos/darkyos.rul out:Kernel.bim stack:3136k map:Kernel.map Kernel.obj Hardware.obj Ascii.obj

..\z_tools\bim2hrb.exe Kernel.bim Kernel.hrb 0

copy /B KernelLoader.bin+Kernel.hrb DarkyOS.sys

..\z_tools\edimg.exe   imgin:..\z_tools\fdimg0at.tek wbinimg src:Boot.bin len:512 from:0 to:0 copy from:DarkyOS.sys to:@: copy from:Test.txt to:@: copy from:src/dog.tex to:@: imgout:DarkyOS.img

copy DarkyOS.img ..\z_tools\qemu\fdimage0.bin

del *.bin
del *.gas
del *.obj
del Kernel.map
del Kernel.bim
del Kernel.hrb
del DarkyOS.sys

del Kernel.nas