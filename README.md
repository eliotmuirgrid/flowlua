# Flowlua

These instructions are setting up a project called **flowlua**, which is built using the **Cosmopolitan Libc** toolchain.

Here's what each step does:

### 1. Create a directory for Cosmopolitan

```bash
mkdir cosmo
```

Creates a directory named `cosmo` that will hold the Cosmopolitan compiler and tools.

---

### 2. Download Cosmopolitan C/C++ compiler

```bash
curl -L -O https://cosmo.zip/pub/cosmocc/cosmocc.zip
```

* `curl` downloads a file from the internet.
* `-L` follows HTTP redirects.
* `-O` saves the file using its original name (`cosmocc.zip`).

After this, you'll have:

```
cosmocc.zip
```

---

### 3. Extract it

```bash
unzip -q cosmocc.zip -d cosmo
```

* `-q` = quiet mode (less output).
* `-d cosmo` extracts everything into the `cosmo` directory.

Result:

```
cosmo/
    bin/
    include/
    lib/
    ...
```

---

### 4. Remove the ZIP file

```bash
rm cosmocc.zip
```

The archive is no longer needed after extraction.

---

### 5. Clone the FlowLua source

```bash
git clone https://github.com/eliotmuirgrid/flowlua
```

Downloads the project's source code into a new directory:

```
flowlua/
```

---

### 6. Enter the source directory

```bash
cd flowlua/flowlua
```

The repository apparently contains another `flowlua` directory inside it. After changing directories, you're in the folder containing the `Makefile` and source code.

For example:

```
flowlua/
    README.md
    flowlua/
        makefile
        *.c
        *.lua
        ...
```

---

### 7. Build the program

```bash
make
```

`make` reads the `makefile` and compiles the project.

The Makefile is configured to use the compiler you unpacked into `$HOME/cosmo/`. If everything is set up correctly, this produces an executable named:

```
flowlua.com
```

The `.com` extension comes from Cosmopolitan Libc. It's not a DOS program—it is a "Actually Portable Executable" (APE) that can run on multiple operating systems.

---

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


