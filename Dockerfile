# Use ubuntu 20.04 stable release
FROM ubuntu:20.04

# Set working directory in container
WORKDIR /usr/src/app

# Set the environment variable to avoid interactive prompts
ENV DEBIAN_FRONTEND=noninteractive

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    g++ \
    make \
    cmake \
    git \
    libx11-dev \
    python3 \
    python3-pip \
    gdb \
    libc6-dbg \
    libcurl4-openssl-dev \
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

# Copy current working directory into container
COPY . .

CMD ["tail", "-f", "/dev/null"]
