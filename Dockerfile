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
    && apt-get clean \
    && rm -rf /var/lib/apt/lists/*

RUN if [ -f /usr/include/X11/Xlib.h ]; then echo "X11 installed successfully"; else echo "X11 installation failed"; exit 1; fi

# Install Google Test
RUN git clone https://github.com/google/googletest.git \
    && cd googletest \
    && cmake . \
    && make \
    && make install

# Copy current working directory into container
COPY . .

CMD ["tail", "-f", "/dev/null"]
