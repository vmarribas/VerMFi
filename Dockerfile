FROM ubuntu:24.04
RUN apt-get update && apt-get -y upgrade
RUN apt-get install -y yosys ghdl yosys-plugin-ghdl git build-essential
RUN mkdir VerMFi
COPY ./ VerMFi/
WORKDIR VerMFi
RUN make all

# Needed for GHDL to find the standard library
ENV GHDL_PREFIX=/usr/lib/ghdl/mcode/vhdl

CMD bash
