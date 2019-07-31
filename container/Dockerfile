FROM archlinux/base:latest

RUN pacman -Syu --noconfirm && pacman -S --noconfirm \
   cmake \
   make \
   gcc \
   clang

COPY entrypoint.sh /entrypoint.sh

ENTRYPOINT [ "/entrypoint.sh" ]
