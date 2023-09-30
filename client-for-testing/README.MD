Ubuntu: sudo apt update
sudo apt install libncurses-dev

Fedora:
sudo dnf install ncurses-devel

Yum:
sudo yum install ncurses-devel


client execution:
gcc -o client client.c -lncurses