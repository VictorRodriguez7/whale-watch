# Stage 1: Build the C++ binary
FROM ubuntu:22.04 AS builder

# Install build tools and dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    curl \
    zip \
    unzip \
    tar \
    pkg-config \
    bison \
    flex \
    autoconf \
    automake \
    libtool \
    && rm -rf /var/lib/apt/lists/*

# Install vcpkg
RUN git clone https://github.com/microsoft/vcpkg.git /vcpkg && \
    /vcpkg/bootstrap-vcpkg.sh

# Install libraries using vcpkg
RUN /vcpkg/vcpkg install cpr nlohmann-json libpq libpqxx boost-multiprecision

# Set environment for CMake to use vcpkg
ENV CMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build the C++ application
RUN rm -rf build && mkdir -p build && cd build && \
    cmake .. -DCMAKE_TOOLCHAIN_FILE=/vcpkg/scripts/buildsystems/vcpkg.cmake && \
    make -j$(nproc)

# Stage 2: Runtime image
FROM ubuntu:22.04

# Install runtime dependencies (PostgreSQL client for libpq)
RUN apt-get update && apt-get install -y \
    libpq-dev \
    curl \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /app

# Copy binary from builder
COPY --from=builder /app/build/whale-watch .

# Expose no port (background scanner)
CMD ["./whale-watch"]
