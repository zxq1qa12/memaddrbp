x64dbg插件,因为x64dbg对内存读写以及执行断点是对内存页进行，无法一步到位断点到具体某个地址，该插件可以像windbg一样对某个内存地址下断点，这样调试会方便很多。如果要编译的话没有pluginsdk文件，
需要把pluginsdk文件放在memaddrbp目录才可以编译。在bin目录有编译好的插件。具体用法如下:在命令中填写memaddrbp 0x020000 10，第一个参数是具体的内存地址，第二个参数是地址范围，比如0x020005也能
断下来，也可以不填或者填0。然后需要点击在内存窗口中的断点里面的硬件，访问、硬件，写入、硬件、执行。因为内存断点似乎有时候断不下来，所以只实现了硬件内存断点的功能。该插件还有些不完善，
某些汇编指令可能断不下来，可能下次更新会完善。
