/*
** $Id: lua.c,v 1.122 2003/04/03 13:34:42 roberto Exp $
** Lua stand-alone interpreter
** See Copyright Notice in lua.h
*/


#include "libc/c.inc"

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libc/zip.h"
#include "third_party/zlib/zlib.h"

#define lua_c

#include "MOD/MODlua.h"

#include "BAS/BASarg.h"
#include "BAS/BASstring.h"
#include "BAS/BASstream.h"
#include "BAS/BAStrace.h"
BAS_TRACE_INIT;

#include "MOD/MODpathSet.h"

/*
** generic extra include file
*/
#ifdef LUA_USERCONFIG
#include LUA_USERCONFIG
#endif


/*
** definition of `isatty'
*/
#ifdef _POSIX_C_SOURCE
#include <unistd.h>
#define stdin_is_tty()	isatty(0)
#else
#define stdin_is_tty()	1  /* assume stdin is a tty */
#endif



#ifndef PROMPT
#define PROMPT		"> "
#endif


#ifndef PROMPT2
#define PROMPT2		">> "
#endif

#ifndef PROGNAME
#define PROGNAME	"lua"
#endif

#ifndef lua_userinit
#define lua_userinit(L)		openstdlibs(L)
#endif


#ifndef LUA_EXTRALIBS
#define LUA_EXTRALIBS	/* empty */
#endif


static lua_State *L = NULL;

static const char *progname = PROGNAME;



static const luaL_reg lualibs[] = {
  {"base", luaopen_base},
  {"table", luaopen_table},
  {"io", luaopen_io},
  {"string", luaopen_string},
  {"math", luaopen_math},
  {"debug", luaopen_debug},
  {"loadlib", luaopen_loadlib},
  /* add your libraries here */
  LUA_EXTRALIBS
  {NULL, NULL}
};



static void lstop (lua_State *l, lua_Debug *ar) {
  (void)ar;  /* unused arg. */
  lua_sethook(l, NULL, 0, 0);
  luaL_error(l, "interrupted!");
}


static void laction (int i) {
  signal(i, SIG_DFL); /* if another SIGINT happens before lstop,
                              terminate process (default action) */
  lua_sethook(L, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}


static void print_usage (void) {
  fprintf(stderr,
  "usage: %s [options] [script [args]].\n"
  "Available options are:\n"
  "  -        execute stdin as a file\n"
  "  --memory-test execute a hard-coded Lua chunk from memory\n"
  "  --zip-memory-test zip execute hello.lua from <zip> in memory\n"
  "  -e stat  execute string `stat'\n"
  "  -i       enter interactive mode after executing `script'\n"
  "  -l name  load and run library `name'\n"
  "  -v       show version information\n"
  "  --       stop handling options\n" ,
  progname);
}


static void l_message (const char *pname, const char *msg) {
  if (pname) fprintf(stderr, "%s: ", pname);
  fprintf(stderr, "%s\n", msg);
}


static int report (int status) {
  const char *msg;
  if (status) {
    msg = lua_tostring(L, -1);
    if (msg == NULL) msg = "(error with no message)";
    l_message(progname, msg);
    lua_pop(L, 1);
  }
  return status;
}


static int lcall (int narg, int clear) {
  int status;
  int base = lua_gettop(L) - narg;  /* function index */
  lua_pushliteral(L, "_TRACEBACK");
  lua_rawget(L, LUA_GLOBALSINDEX);  /* get traceback function */
  lua_insert(L, base);  /* put it under chunk and args */
  signal(SIGINT, laction);
  status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
  signal(SIGINT, SIG_DFL);
  lua_remove(L, base);  /* remove traceback function */
  return status;
}


static void print_version (void) {
  l_message(NULL, LUA_VERSION "  " LUA_COPYRIGHT);
}


static void getargs (char *argv[], int n) {
  int i;
  lua_newtable(L);
  for (i=0; argv[i]; i++) {
    lua_pushnumber(L, i - n);
    lua_pushstring(L, argv[i]);
    lua_rawset(L, -3);
  }
  /* arg.n = maximum index in table `arg' */
  lua_pushliteral(L, "n");
  lua_pushnumber(L, i-n-1);
  lua_rawset(L, -3);
}


static int docall (int status) {
  if (status == 0) status = lcall(0, 1);
  return report(status);
}


static int file_input (const char *name) {
  return docall(luaL_loadfile(L, name));
}


static int dostring (const char *s, const char *name) {
  return docall(luaL_loadbuffer(L, s, strlen(s), name));
}


static int memory_test_input (void) {
  static const char *chunk = "print(\"Hello from memory\")";
  return docall(luaL_loadbuffer(L, chunk, strlen(chunk), "=@memory-test"));
}


static int zip_memory_test_input (const char *zipname) {
  static const char *entry_name = "hello.lua";
  FILE *f = NULL;
  unsigned char *zipbuf = NULL;
  unsigned char *luabuf = NULL;
  const unsigned char *eocd;
  const unsigned char *zp;
  const unsigned char *zend;
  const unsigned char *cfile;
  int status = 1;
  long zipsize_long;
  size_t zipsize;
  size_t eocd_start;
  size_t eocd_min;
  size_t at;
  uint32_t cdir_off;
  uint16_t cdir_records;
  int i;

  f = fopen(zipname, "rb");
  if (f == NULL) {
    l_message(progname, lua_pushfstring(L, "cannot open zip %s", zipname));
    goto cleanup;
  }
  if (fseek(f, 0, SEEK_END) != 0) {
    l_message(progname, lua_pushfstring(L, "cannot seek zip %s", zipname));
    goto cleanup;
  }
  zipsize_long = ftell(f);
  if (zipsize_long <= 0) {
    l_message(progname, lua_pushfstring(L, "empty or unreadable zip %s", zipname));
    goto cleanup;
  }
  if (fseek(f, 0, SEEK_SET) != 0) {
    l_message(progname, lua_pushfstring(L, "cannot rewind zip %s", zipname));
    goto cleanup;
  }

  zipsize = (size_t)zipsize_long;
  zipbuf = (unsigned char *)malloc(zipsize);
  if (zipbuf == NULL) {
    l_message(progname, "out of memory reading zip");
    goto cleanup;
  }
  if (fread(zipbuf, 1, zipsize, f) != zipsize) {
    l_message(progname, lua_pushfstring(L, "cannot read zip %s", zipname));
    goto cleanup;
  }

  /* EOCD must live in the last 64KiB + header. */
  eocd = NULL;
  eocd_start = zipsize - kZipCdirHdrMinSize;
  eocd_min = (zipsize > (size_t)(65535 + kZipCdirHdrMinSize))
                 ? (zipsize - (size_t)(65535 + kZipCdirHdrMinSize))
                 : 0;
  for (at = eocd_start + 1; at > eocd_min; --at) {
    const unsigned char *p = zipbuf + (at - 1);
    if (ZIP_CDIR_MAGIC(p) == kZipCdirHdrMagic &&
        p + ZIP_CDIR_HDRSIZE(p) == zipbuf + zipsize) {
      eocd = p;
      break;
    }
  }
  if (eocd == NULL) {
    l_message(progname, "invalid zip: cannot find EOCD");
    goto cleanup;
  }

  cdir_off = ZIP_CDIR_OFFSET(eocd);
  cdir_records = ZIP_CDIR_RECORDS(eocd);
  zp = zipbuf + cdir_off;
  zend = zipbuf + zipsize;
  if (zp < zipbuf || zp > zend) {
    l_message(progname, "invalid zip central directory offset");
    goto cleanup;
  }

  cfile = NULL;
  for (i = 0; i < (int)cdir_records; ++i) {
    size_t namesz;
    size_t hdrsz;
    if (zp + kZipCfileHdrMinSize > zend) break;
    if (ZIP_CFILE_MAGIC(zp) != kZipCfileHdrMagic) break;
    namesz = ZIP_CFILE_NAMESIZE(zp);
    hdrsz = ZIP_CFILE_HDRSIZE(zp);
    if (zp + hdrsz > zend) break;
    if (namesz == strlen(entry_name) &&
        memcmp(ZIP_CFILE_NAME(zp), entry_name, namesz) == 0) {
      cfile = zp;
      break;
    }
    zp += hdrsz;
  }

  if (cfile == NULL) {
    l_message(progname, lua_pushfstring(L, "entry %s not found in zip", entry_name));
    goto cleanup;
  }

  {
    int method = ZIP_CFILE_COMPRESSIONMETHOD(cfile);
    uint32_t lfileoff = ZIP_CFILE_OFFSET(cfile);
    uint32_t compsize = ZIP_CFILE_COMPRESSEDSIZE(cfile);
    uint32_t uncsize = ZIP_CFILE_UNCOMPRESSEDSIZE(cfile);
    const unsigned char *lfile;
    const unsigned char *ldata;
    size_t outsize;

    if ((size_t)lfileoff > zipsize) {
      l_message(progname, "local file offset outside zip");
      goto cleanup;
    }

    lfile = zipbuf + (size_t)lfileoff;
    if (lfile + kZipLfileHdrMinSize > zend || ZIP_LFILE_MAGIC(lfile) != kZipLfileHdrMagic) {
      l_message(progname, "invalid local file header");
      goto cleanup;
    }

    ldata = ZIP_LFILE_CONTENT(lfile);
    if (ldata < zipbuf || ldata > zend || (size_t)(zend - ldata) < (size_t)compsize) {
      l_message(progname, "zip entry data range is invalid");
      goto cleanup;
    }

    outsize = (size_t)uncsize;
    luabuf = (unsigned char *)malloc(outsize ? outsize : 1);
    if (luabuf == NULL) {
      l_message(progname, "out of memory for lua entry");
      goto cleanup;
    }

    if (method == kZipCompressionNone) {
      if (compsize != uncsize) {
        l_message(progname, "stored zip entry size mismatch");
        goto cleanup;
      }
      if (outsize > 0) memcpy(luabuf, ldata, outsize);
    }
    else if (method == kZipCompressionDeflate) {
      z_stream zs;
      int zrc;
      memset(&zs, 0, sizeof(zs));
      zs.next_in = ldata;
      zs.avail_in = compsize;
      zs.next_out = luabuf;
      zs.avail_out = (uInt)outsize;
      zrc = inflateInit2(&zs, -15);  /* raw deflate stream in ZIP entry */
      if (zrc != Z_OK) {
        l_message(progname, lua_pushfstring(L, "inflateInit2 failed: %d", zrc));
        goto cleanup;
      }
      zrc = inflate(&zs, Z_FINISH);
      inflateEnd(&zs);
      if (zrc != Z_STREAM_END || zs.total_out != (uLong)outsize) {
        l_message(progname, lua_pushfstring(L, "inflate failed: %d", zrc));
        goto cleanup;
      }
    }
    else {
      l_message(progname, lua_pushfstring(L, "unsupported zip method %d", method));
      goto cleanup;
    }

    status = docall(luaL_loadbuffer(L, (const char *)luabuf, outsize,
                                    "=@zip:hello.lua"));
  }

cleanup:
  if (f != NULL) fclose(f);
  if (zipbuf != NULL) free(zipbuf);
  if (luabuf != NULL) free(luabuf);
  return status;
}


static int load_file (const char *name) {
  lua_pushliteral(L, "require");
  lua_rawget(L, LUA_GLOBALSINDEX);
  if (!lua_isfunction(L, -1)) {  /* no `require' defined? */
    lua_pop(L, 1);
    return file_input(name);
  }
  else {
    lua_pushstring(L, name);
    return report(lcall(1, 1));
  }
}


/*
** this macro can be used by some `history' system to save lines
** read in manual input
*/
#ifndef lua_saveline
#define lua_saveline(L,line)	/* empty */
#endif


/*
** this macro defines a function to show the prompt and reads the
** next line for manual input
*/
#ifndef lua_readline
#define lua_readline(L,prompt)		readline(L,prompt)

/* maximum length of an input line */
#ifndef MAXINPUT
#define MAXINPUT	512
#endif


static int readline (lua_State *l, const char *prompt) {
  static char buffer[MAXINPUT];
  if (prompt) {
    fputs(prompt, stdout);
    fflush(stdout);
  }
  if (fgets(buffer, sizeof(buffer), stdin) == NULL)
    return 0;  /* read fails */
  else {
    lua_pushstring(l, buffer);
    return 1;
  }
}

#endif


static const char *get_prompt (int firstline) {
  const char *p = NULL;
  lua_pushstring(L, firstline ? "_PROMPT" : "_PROMPT2");
  lua_rawget(L, LUA_GLOBALSINDEX);
  p = lua_tostring(L, -1);
  if (p == NULL) p = (firstline ? PROMPT : PROMPT2);
  lua_pop(L, 1);  /* remove global */
  return p;
}


static int incomplete (int status) {
  if (status == LUA_ERRSYNTAX &&
         strstr(lua_tostring(L, -1), "near `<eof>'") != NULL) {
    lua_pop(L, 1);
    return 1;
  }
  else
    return 0;
}


static int load_string (void) {
  int status;
  lua_settop(L, 0);
  if (lua_readline(L, get_prompt(1)) == 0)  /* no input? */
    return -1;
  if (lua_tostring(L, -1)[0] == '=') {  /* line starts with `=' ? */
    lua_pushfstring(L, "return %s", lua_tostring(L, -1)+1);/* `=' -> `return' */
    lua_remove(L, -2);  /* remove original line */
  }
  for (;;) {  /* repeat until gets a complete line */
    status = luaL_loadbuffer(L, lua_tostring(L, 1), lua_strlen(L, 1), "=stdin");
    if (!incomplete(status)) break;  /* cannot try to add lines? */
    if (lua_readline(L, get_prompt(0)) == 0)  /* no more input? */
      return -1;
    lua_concat(L, lua_gettop(L));  /* join lines */
  }
  lua_saveline(L, lua_tostring(L, 1));
  lua_remove(L, 1);  /* remove line */
  return status;
}


static void manual_input (void) {
  int status;
  const char *oldprogname = progname;
  progname = NULL;
  while ((status = load_string()) != -1) {
    if (status == 0) status = lcall(0, 0);
    report(status);
    if (status == 0 && lua_gettop(L) > 0) {  /* any result to print? */
      lua_getglobal(L, "print");
      lua_insert(L, 1);
      if (lua_pcall(L, lua_gettop(L)-1, 0, 0) != 0)
        l_message(progname, lua_pushfstring(L, "error calling `print' (%s)",
                                               lua_tostring(L, -1)));
    }
  }
  lua_settop(L, 0);  /* clear stack */
  fputs("\n", stdout);
  progname = oldprogname;
}


static int handle_argv (char *argv[], int *interactive) {
  BAS_FUNCTION(handle_argv);
  if (argv[1] == NULL) {  /* no more arguments? */
    if (stdin_is_tty()) {
      print_version();
      manual_input();
    }
    else
      file_input(NULL);  /* executes stdin as a file */
  }
  else {  /* other arguments; loop over them */
    int i;
    for (i = 1; argv[i] != NULL; i++) {
      if (argv[i][0] != '-') break;  /* not an option? */
      switch (argv[i][1]) {  /* option */
        case '-': {  /* `--' */
          if (argv[i][2] != '\0') {
            if (strcmp(argv[i], "--memory-test") == 0) {
              return memory_test_input();
            }
            if (strcmp(argv[i], "--zip-memory-test") == 0) {
              const char *zipname = argv[++i];
              if (zipname == NULL) {
                print_usage();
                return 1;
              }
              return zip_memory_test_input(zipname);
            }
            BAS_TRC("Eliot hack for --trace");
	    i++; 
            break;
          }
          i++;  /* skip this argument */
          goto endloop;  /* stop handling arguments */
        }
        case '\0': {
          file_input(NULL);  /* executes stdin as a file */
          break;
        }
        case 'i': {
          *interactive = 1;
          break;
        }
        case 'v': {
          print_version();
          break;
        }
        case 'e': {
          const char *chunk = argv[i] + 2;
          if (*chunk == '\0') chunk = argv[++i];
          if (chunk == NULL) {
            print_usage();
            return 1;
          }
          if (dostring(chunk, "=<command line>") != 0)
            return 1;
          break;
        }
        case 'l': {
          const char *filename = argv[i] + 2;
          if (*filename == '\0') filename = argv[++i];
          if (filename == NULL) {
            print_usage();
            return 1;
          }
          if (load_file(filename))
            return 1;  /* stop if file fails */
          break;
        }
        case 'c': {
          l_message(progname, "option `-c' is deprecated");
          break;
        }
        case 's': {
          l_message(progname, "option `-s' is deprecated");
          break;
        }
        default: {
          print_usage();
          return 1;
        }
      }
    } endloop:
    if (argv[i] != NULL) {
      const char *filename = argv[i];
      getargs(argv, i);  /* collect arguments */
      lua_setglobal(L, "arg");
      return file_input(filename);  /* stop scanning arguments */
    }
  }
  return 0;
}


static void openstdlibs (lua_State *l) {
  const luaL_reg *lib = lualibs;
  for (; lib->func; lib++) {
    lib->func(l);  /* open library */
    lua_settop(l, 0);  /* discard any results */
  }
}


static int handle_luainit (void) {
  const char *init = getenv("LUA_INIT");
  if (init == NULL) return 0;  /* status OK */
  else if (init[0] == '@')
    return file_input(init+1);
  else
    return dostring(init, "=LUA_INIT");
}


struct Smain {
  int argc;
  char **argv;
  int status;
};


static int pmain (lua_State *l) {
  struct Smain *s = (struct Smain *)lua_touserdata(l, 1);
  int status;
  int interactive = 0;
  if (s->argv[0] && s->argv[0][0]) progname = s->argv[0];
  L = l;
  lua_userinit(l);  /* open libraries */
  status = handle_luainit();
  if (status == 0) {
    status = handle_argv(s->argv, &interactive);
    if (status == 0 && interactive) manual_input();
  }
  s->status = status;
  return 0;
}


int main (int argc, char *argv[]) {
  BASstring Match;
  if (BASargFlagPresent("trace", &Match, argc, argv)){
     BASout << "Tracing: " << Match << newline;
     BAStrace(Match.data());
  }
  int status;
  struct Smain s;
  MODpathSet(L);
  lua_State* L = lua_open();  /* create state */
  if (L == NULL) {
    l_message(argv[0], "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  s.argc = argc;
  s.argv = argv;
  status = lua_cpcall(L, &pmain, &s);
  report(status);
  lua_close(L);
  return (status || s.status) ? EXIT_FAILURE : EXIT_SUCCESS;
}

