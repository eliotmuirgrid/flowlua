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

###  Build the program with make -j16 for faster pararel build.

```bash
make -j16
```

### Run it

```bash
./flowlua.com
```

This starts the FlowLua interpreter.

You'll see a Lua prompt similar to:

```text
>
```

or

```text
Lua 5.x
>
```

---

### 9. Test it

At the prompt, type:

```lua
print("Hello")
```

You should see:

```text
Hello
```

which confirms the interpreter is working.


