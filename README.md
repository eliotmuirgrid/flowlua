# Flow

(formerly called FlowLua)

Just download the binary [flow.com][https://github.com/eliotmuirgrid/flow/blob/master/flow.com] from here.

Okay so it won't really work but until it does something useful like enable people to clean up their email spam no one will do this
anyway :-)

## For boring technical folk who like working - here you go.

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

### Clone the Flow source into ~/flow and enter the source directory

```bash
git clone https://github.com/eliotmuirgrid/flow
cd flow
```

###  Build the program with make -j16 for faster parallel build.

```bash
make -j16
```

### Run it 

```bash
./flow.com
```
It should do something interesting.  I'm working on it every day so it's changing a lot.
