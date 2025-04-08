# Base image
FROM ubuntu:22.04

# -------------- Time Zone -------------
ENV TZ=Europe/Rome
RUN ln -snf /usr/share/zoneinfo/$TZ /etc/localtime && echo $TZ > /etc/timezone

# -------------- Base C++ Development -------------
RUN apt update \
    && apt install -y \
    git \
    curl \
    zip \
    gcc \
    g++ \
    cmake \
    build-essential \
    tzdata \
    pkg-config \
    liblog4cxx-dev \
    libgtest-dev \
    && rm -rf /var/lib/apt/lists/*

# -------------- VCPKG ---------------------------
RUN git clone https://github.com/microsoft/vcpkg.git /opt/vcpkg && \
    cd /opt/vcpkg && \
    /opt/vcpkg/bootstrap-vcpkg.sh

# Set environment variables for vcpkg
ENV VCPKG_ROOT=/opt/vcpkg
ENV VCPKG_TOOLCHAIN=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake
ENV VCPKG_FEATURE_FLAGS=versions
ENV PATH=$PATH:/opt/vcpkg

RUN /opt/vcpkg/vcpkg install nlohmann-json log4cxx gtest
RUN ls /opt/vcpkg/scripts/buildsystems/vcpkg.cmake

# -------------- Copy Project Files ---------------
WORKDIR /workspace
COPY . /workspace

# -------------- Check if files are copied ---------------
RUN ls /workspace

# -------------- Configure Project --------------------
RUN cmake -S /workspace -B /workspace/build -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake --debug-output --trace

# -------------- Check CMake Logs --------------------
RUN cat /workspace/build/CMakeFiles/CMakeError.log
RUN cat /workspace/build/CMakeFiles/CMakeOutput.log

# -------------- Build Project --------------------
RUN cmake --build /workspace/build

# -------------- Run Executable -------------------
CMD ["/workspace/build/MapperReducer"]
