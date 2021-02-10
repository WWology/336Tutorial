ARG VARIANT="buster"
FROM mcr.microsoft.com/vscode/devcontainers/cpp:0-${VARIANT}

RUN apt-get update && export DEBIAN_FRONTEND=noninteractive \
    && apt-get -yq install --no-install-recommends cppcheck \
    libxext-dev libx11-dev x11proto-gl-dev \
    libgl1-mesa-dev libegl1-mesa-dev pkg-config \
    xorg-dev libglu1-mesa-dev

# INSTALL DEPENDENCIES
RUN apt-get update -yq \
    && apt-get install -yq --no-install-recommends wget \
    && apt-get clean -yq

# INSTALL PVS-Studio
RUN wget -q -O - https://files.viva64.com/etc/pubkey.txt | apt-key add - \
    && wget -O /etc/apt/sources.list.d/viva64.list \
    https://files.viva64.com/etc/viva64.list \
    && apt-get update -yq \
    && apt-get install -yq pvs-studio strace \
    && pvs-studio --version \
    && apt-get clean -yq

# INSTALL CONAN
RUN apt-get -yq install python3-pip && pip3 install conan