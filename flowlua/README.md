# Eureka!

This is lua.com, a sample Lua interpreter compiled as a cross
plaform binary.

It can be used as a batch interpreter and also interactively.
There are man pages for it in both nroff and html in ../../doc.

Usage: ./lua.com [options] [script [args]].  Available options are:
  -        execute stdin as a file
  -e stat  execute string `stat'
  -i       enter interactive mode after executing `script'
  -l name  load and run library `name'
  -v       show version information
  --       stop handling options

This interpreter is suitable for using Lua as a standalone language; it loads
all standard libraries. For a minimal interpreter, see ../../etc/min.c.

Now I can add debugging and start porting flowshell over to it.

Looking for my first customer to start porting their interfaces to.
