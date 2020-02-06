FROM ubuntu:rolling

RUN apt-get update && \
    apt-get install -y software-properties-common && \
    add-apt-repository ppa:ubuntu-toolchain-r/test

RUN apt-get update && \
    apt-get install -y curl g++-9 gcc-9 git wget curl \
        clang-tidy-9 clang-tools-9 clang-format-9 clang \
        python3 python3-yaml python3-pip clang-9 \
        libncurses5-dev libncurses5 libclang-9-dev \
        lcov ninja-build nodejs npm doxygen && \
    update-alternatives --install /usr/bin/gcc     gcc     /usr/bin/gcc-9     10 && \
    update-alternatives --install /usr/bin/gcov    gcov    /usr/bin/gcov-9    10 && \
    update-alternatives --install /usr/bin/g++     g++     /usr/bin/g++-9     10 && \
    update-alternatives --install /usr/bin/clang   clang   /usr/bin/clang-9   10 && \
    update-alternatives --install /usr/bin/clang++ clang++ /usr/bin/clang++-9 10 && \
    update-alternatives --install /usr/bin/clang-format clang-format /usr/bin/clang-format-9 10 && \
    update-alternatives --install /usr/bin/clang-tidy clang-tidy /usr/bin/clang-tidy-9 10

WORKDIR /pkg
RUN curl -L https://github.com/Kitware/CMake/releases/download/v3.16.3/cmake-3.16.3-Linux-x86_64.tar.gz | tar zx
ENV PATH $PATH:/pkg/cmake-3.16.3-Linux-x86_64/bin

RUN curl -sL https://deb.nodesource.com/setup_12.x | bash -
RUN apt-get install -y nodejs

COPY install_loop_fusion.sh /opt/install_loop_fusion.sh
COPY install_compiler_explorer.sh /opt/install_compiler_explorer.sh
COPY install_ce_libraries.sh /opt/install_ce_libraries.sh
COPY c++.local.properties /opt/c++.local.properties
COPY start_ce.sh /opt/start_ce.sh

RUN /opt/install_loop_fusion.sh
RUN /opt/install_compiler_explorer.sh
RUN /opt/install_ce_libraries.sh

CMD ["/opt/start_ce.sh"]
