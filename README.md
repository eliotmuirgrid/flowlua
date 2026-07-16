# Flowlua

These instructions are setting up a project called **flowlua**, which is built using the **Cosmopolitan Libc** toolchain.

### Create a directory for Cosmopolitan and download it

```bash
mkdir cosmo
curl -L -O https://cosmo.zip/pub/cosmocc/cosmocc.zip
```

### Extract it quietly into the cosmo directory and remove zip afterwards

```bash
unzip -q cosmocc.zip -d cosmo
rm cosmocc.zip
```

### Clone the FlowLua source into ~/flowlua and enter the source directory

```bash
git clone https://github.com/eliotmuirgrid/flowlua
cd flowlua/flowlua
```

###  Build the program with make -j16 for faster parallel build.

```bash
make -j16
```

### Run it and try printing "Hello"

```bash
./flowlua.com
```
It should open up and see

```
Lua 5.0.3  Copyright (C) 1994-2006 Tecgraf, PUC-Rio
> print "Hello"
Hello
> 

```
Control+C to exit 
