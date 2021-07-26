# Get the base Ubuntu image from Docker Hub
FROM ubuntu:latest AS base

# Update apps on the base image
RUN apt-get -y update && apt-get install -y

ENV TZ=Europe/London
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# Install common build dependencies
ENV APT_PACKAGES="at curl unzip tar libxt-dev gperf libxaw7-dev cifs-utils \
  build-essential g++ gfortran zip libx11-dev libxkbcommon-x11-dev libxi-dev \
  libgl1-mesa-dev libglu1-mesa-dev mesa-common-dev libxinerama-dev libxxf86vm-dev \
  libxcursor-dev yasm libnuma1 libnuma-dev python-six python3-six python-yaml \
  flex libbison-dev autoconf libudev-dev libncurses5-dev libtool libxrandr-dev \
  xutils-dev dh-autoreconf autoconf-archive libgles2-mesa-dev ruby-full \
  pkg-config meson"

# Additionally required by qt5-base
ENV APT_PACKAGES="$APT_PACKAGES libxext-dev libxfixes-dev libxrender-dev \
  libxcb1-dev libx11-xcb-dev libxcb-glx0-dev libxcb-util0-dev"
# Additionally required by qt5-base for qt5-x11extras
ENV APT_PACKAGES="$APT_PACKAGES libxkbcommon-dev libxcb-keysyms1-dev \
  libxcb-image0-dev libxcb-shm0-dev libxcb-icccm4-dev libxcb-sync0-dev \
  libxcb-xfixes0-dev libxcb-shape0-dev libxcb-randr0-dev \
  libxcb-render-util0-dev libxcb-xinerama0-dev libxcb-xkb-dev libxcb-xinput-dev"

# ffmpeg
ENV APT_PACKAGES="$APT_PACKAGES nasm"

ENV APT_PACKAGES="$APT_PACKAGES qt5-qmake flatpak flatpak-builder"

# Install the Clang compiler
RUN apt-get -y install clang cmake make git $APT_PACKAGES

# Install package manager
FROM base AS vcpkg
RUN mkdir /usr/src/vcpkg
RUN mkdir /usr/src/lodge
RUN chown -R 1000:1000 /usr/src/lodge
RUN chown -R 1000:1000 /usr/src/vcpkg
RUN	git clone https://github.com/Microsoft/vcpkg.git /usr/src/vcpkg

RUN	/usr/src/vcpkg/bootstrap-vcpkg.sh

RUN /usr/src/vcpkg/vcpkg install qt5-base
RUN /usr/src/vcpkg/vcpkg install qt5-declarative
RUN /usr/src/vcpkg/vcpkg install boost
RUN /usr/src/vcpkg/vcpkg install spdlog ffmpeg catch

FROM vcpkg AS gui
RUN flatpak remote-add --if-not-exists flathub https://flathub.org/repo/flathub.flatpakrepo && flatpak remote-add --if-not-exists kdeapps --from https://distribute.kde.org/kdeapps.flatpakrepo
RUN flatpak install -y flathub org.kde.Platform//5.15 org.kde.Sdk//5.15

# Build application
FROM gui AS builder
ENV VCPKG=/usr/src/vcpkg
WORKDIR /usr/src/lodge



