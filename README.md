# Flowlua

These instructions are setting up a project called [flowlua](https://interfaceware.com/aazip/cosmopolitan/flowlua), which is built using the **Cosmopolitan Libc** toolchain.

### Create a directory for Cosmopolitan and download it

```bash
cd $HOME
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

### Run it 

```bash
./flowlua.com
```
It should say something friendly like Hello. 

```
Hello
```

Congratulations - one small step to a world when people
can be safe and control their own data.
