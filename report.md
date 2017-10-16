http://blog.csdn.net/pacosonswjtu/article/details/48005813
http://blog.csdn.net/ruyanhai/article/details/7181842
http://blog.csdn.net/bysui/article/details/51453329
http://wiki.osdev.org/GDT_Tutorial
http://wiki.osdev.org/Protected_Mode
http://blog.csdn.net/bufanq/article/details/51530875
http://blog.csdn.net/vally1989/article/details/71796482
http://blog.csdn.net/qq_25426415/article/details/54583835
./configure MACH=pc ARCH=i386 --disable-werror --host=i386-pc-elf --target=i386-pc-elf
http://www.cnblogs.com/zslhg903/p/5781882.html
dd if=/dev/zero of=aim.img bs=1b count=100

sudo dd if=boot.bin of=/dev/loop0
sudo dd if=vmaim.elf of=/dev/loop0 seek=1 bs=512
