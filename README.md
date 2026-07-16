# Flowlua

These instructions are setting up a project called **flowlua**, which is built using the **Cosmopolitan Libc** toolchain.

### 1. Create a directory for Cosmopolitan

```bash
mkdir cosmo
```

### 2. Download Cosmopolitan C/C++ compiler

```bash
curl -L -O https://cosmo.zip/pub/cosmocc/cosmocc.zip
```

This downloads the cosmocc.zip file with the cosmo compiler in it.

### 3. Extract it quietly into the cosmo directory

```bash
unzip -q cosmocc.zip -d cosmo
```

### 4. Remove the ZIP file afterwards

```bash
rm cosmocc.zip
```

### 5. Clone the FlowLua source into ~/flowlua

```bash
git clone https://github.com/eliotmuirgrid/flowlua
```

### 6. Enter the source directory

```bash
cd flowlua/flowlua
```

The repository  contains another `flowlua` directory inside it. After changing directories, you're in the folder containing the `Makefile` and source code.


### 7. Build the program with make -j16 for faster pararel build.

```bash
make -j16
```

The Makefile is configured to use the compiler you unpacked into `$HOME/cosmo/`. If everything is set up correctly, this produces an executable named: flowlua.com/

The `.com` extension comes from Cosmopolitan Libc. It's not a DOS program—it is a "Actually Portable Executable" (APE) that can run on multiple operating systems.

### 8. Run it

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


