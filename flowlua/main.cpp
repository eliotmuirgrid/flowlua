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
  "  --zip-memory-test execute <entry.lua> from <archive.zip> in memory\n"
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


static int read_file_into_memory (const char *zipname,
                                  unsigned char **out_zipbuf,
                                  size_t *out_zipsize) {
  FILE *f = NULL;
  unsigned char *zipbuf = NULL;
  long zipsize_long;
  size_t zipsize;
  char message[512];

  *out_zipbuf = NULL;
  *out_zipsize = 0;

  f = fopen(zipname, "rb");
  if (f == NULL) {
    snprintf(message, sizeof(message), "cannot open zip %s", zipname);
    l_message(progname, message);
    return 1;
  }
  if (fseek(f, 0, SEEK_END) != 0) {
    snprintf(message, sizeof(message), "cannot seek zip %s", zipname);
    l_message(progname, message);
    fclose(f);
    return 1;
  }
  zipsize_long = ftell(f);
  if (zipsize_long <= 0) {
    snprintf(message, sizeof(message), "empty or unreadable zip %s", zipname);
    l_message(progname, message);
    fclose(f);
    return 1;
  }
  if (fseek(f, 0, SEEK_SET) != 0) {
    snprintf(message, sizeof(message), "cannot rewind zip %s", zipname);
    l_message(progname, message);
    fclose(f);
    return 1;
  }

  zipsize = (size_t)zipsize_long;
  zipbuf = (unsigned char *)malloc(zipsize);
  if (zipbuf == NULL) {
    l_message(progname, "out of memory reading zip");
    fclose(f);
    return 1;
  }
  if (fread(zipbuf, 1, zipsize, f) != zipsize) {
    snprintf(message, sizeof(message), "cannot read zip %s", zipname);
    l_message(progname, message);
    free(zipbuf);
    fclose(f);
    return 1;
  }

  fclose(f);
  *out_zipbuf = zipbuf;
  *out_zipsize = zipsize;
  return 0;
}


static int find_zip_eocd (const unsigned char *zipbuf,
                          size_t zipsize,
                          const unsigned char **out_eocd) {
  size_t eocd_start;
  size_t eocd_min;
  size_t at;

  *out_eocd = NULL;
  if (zipsize < kZipCdirHdrMinSize) return 1;

  /* EOCD must live in the last 64KiB + header. */
  eocd_start = zipsize - kZipCdirHdrMinSize;
  eocd_min = (zipsize > (size_t)(65535 + kZipCdirHdrMinSize))
                 ? (zipsize - (size_t)(65535 + kZipCdirHdrMinSize))
                 : 0;
  for (at = eocd_start + 1; at > eocd_min; --at) {
    const unsigned char *p = zipbuf + (at - 1);
    if (ZIP_CDIR_MAGIC(p) == kZipCdirHdrMagic &&
        p + ZIP_CDIR_HDRSIZE(p) == zipbuf + zipsize) {
      *out_eocd = p;
      return 0;
    }
  }
  return 1;
}


static int find_zip_entry (const unsigned char *zipbuf,
                           size_t zipsize,
                           const unsigned char *eocd,
                           const char *entry_name,
                           const unsigned char **out_cfile) {
  const unsigned char *zp;
  const unsigned char *zend;
  uint32_t cdir_off;
  uint16_t cdir_records;
  int i;

  *out_cfile = NULL;
  cdir_off = ZIP_CDIR_OFFSET(eocd);
  cdir_records = ZIP_CDIR_RECORDS(eocd);
  zp = zipbuf + cdir_off;
  zend = zipbuf + zipsize;
  if (zp < zipbuf || zp > zend) {
    l_message(progname, "invalid zip central directory offset");
    return 1;  /* invalid central-directory state */
  }

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
      *out_cfile = zp;
      return 0;  /* found */
    }
    zp += hdrsz;
  }

  return 2;  /* entry not found */
}


static int extract_zip_entry (const unsigned char *zipbuf,
                              size_t zipsize,
                              const unsigned char *cfile,
                              unsigned char **out_luabuf,
                              size_t *out_luasize) {
  int method = ZIP_CFILE_COMPRESSIONMETHOD(cfile);
  uint32_t lfileoff = ZIP_CFILE_OFFSET(cfile);
  uint32_t compsize = ZIP_CFILE_COMPRESSEDSIZE(cfile);
  uint32_t uncsize = ZIP_CFILE_UNCOMPRESSEDSIZE(cfile);
  const unsigned char *lfile;
  const unsigned char *ldata;
  const unsigned char *zend = zipbuf + zipsize;
  unsigned char *luabuf = NULL;
  size_t outsize;
  char message[128];

  *out_luabuf = NULL;
  *out_luasize = 0;

  if ((size_t)lfileoff > zipsize) {
    l_message(progname, "local file offset outside zip");
    return 1;
  }

  lfile = zipbuf + (size_t)lfileoff;
  if (lfile + kZipLfileHdrMinSize > zend || ZIP_LFILE_MAGIC(lfile) != kZipLfileHdrMagic) {
    l_message(progname, "invalid local file header");
    return 1;
  }

  ldata = ZIP_LFILE_CONTENT(lfile);
  if (ldata < zipbuf || ldata > zend || (size_t)(zend - ldata) < (size_t)compsize) {
    l_message(progname, "zip entry data range is invalid");
    return 1;
  }

  outsize = (size_t)uncsize;
  luabuf = (unsigned char *)malloc(outsize ? outsize : 1);
  if (luabuf == NULL) {
    l_message(progname, "out of memory for lua entry");
    return 1;
  }

  if (method == kZipCompressionNone) {
    if (compsize != uncsize) {
      l_message(progname, "stored zip entry size mismatch");
      free(luabuf);
      return 1;
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
      snprintf(message, sizeof(message), "inflateInit2 failed: %d", zrc);
      l_message(progname, message);
      free(luabuf);
      return 1;
    }
    zrc = inflate(&zs, Z_FINISH);
    inflateEnd(&zs);
    if (zrc != Z_STREAM_END || zs.total_out != (uLong)outsize) {
      snprintf(message, sizeof(message), "inflate failed: %d", zrc);
      l_message(progname, message);
      free(luabuf);
      return 1;
    }
  }
  else {
    snprintf(message, sizeof(message), "unsupported zip method %d", method);
    l_message(progname, message);
    free(luabuf);
    return 1;
  }

  *out_luabuf = luabuf;
  *out_luasize = outsize;
  return 0;
}


static int find_zip_entry_or_report (const unsigned char *zipbuf,
                                     size_t zipsize,
                                     const unsigned char *eocd,
                                     const char *entry_name,
                                     const unsigned char **out_cfile) {
  int entry_result;
  if (out_cfile == NULL) return 1;
  *out_cfile = NULL;

  entry_result = find_zip_entry(zipbuf, zipsize, eocd, entry_name, out_cfile);
  if (entry_result == 0) return 0;
  if (entry_result == 1) return 1;

  {
    char message[512];
    snprintf(message, sizeof(message), "entry %s not found in zip", entry_name);
    l_message(progname, message);
  }
  return 1;
}


static int load_zip_entry_into_memory (const char *zipname,
                                       const char *entry_name,
                                       unsigned char **out_luabuf,
                                       size_t *out_luasize) {
  unsigned char *zipbuf = NULL;
  unsigned char *luabuf = NULL;
  const unsigned char *eocd;
  const unsigned char *cfile;
  size_t zipsize = 0;
  size_t luasize = 0;

  *out_luabuf = NULL;
  *out_luasize = 0;

  if (read_file_into_memory(zipname, &zipbuf, &zipsize) != 0) {
    return 1;
  }
  if (find_zip_eocd(zipbuf, zipsize, &eocd) != 0) {
    l_message(progname, "invalid zip: cannot find EOCD");
    free(zipbuf);
    return 1;
  }
  if (find_zip_entry_or_report(zipbuf, zipsize, eocd, entry_name, &cfile) != 0) {
    free(zipbuf);
    return 1;
  }
  if (extract_zip_entry(zipbuf, zipsize, cfile, &luabuf, &luasize) != 0) {
    free(zipbuf);
    return 1;
  }

  free(zipbuf);
  *out_luabuf = luabuf;
  *out_luasize = luasize;
  return 0;
}


static int execute_lua_buffer (const char *entry_name,
                               const unsigned char *luabuf,
                               size_t luasize) {
  char *chunk_name = NULL;
  size_t chunk_name_len = strlen(entry_name) + 7;  /* "=@zip:" + entry + NUL */
  int status;

  chunk_name = (char *)malloc(chunk_name_len);
  if (chunk_name == NULL) {
    l_message(progname, "out of memory for lua chunk name");
    return 1;
  }
  snprintf(chunk_name, chunk_name_len, "=@zip:%s", entry_name);
  status = docall(luaL_loadbuffer(L, (const char *)luabuf, luasize, chunk_name));
  free(chunk_name);
  return status;
}


static int zip_memory_test_input (const char *zipname, const char *entry_name) {
  unsigned char *luabuf = NULL;
  size_t luasize = 0;
  int status;

  if (load_zip_entry_into_memory(zipname, entry_name, &luabuf, &luasize) != 0) {
    return 1;
  }

  status = execute_lua_buffer(entry_name, luabuf, luasize);
  free(luabuf);
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
    int stop_handling_options = 0;
    for (i = 1; argv[i] != NULL && !stop_handling_options; i++) {
      if (argv[i][0] != '-') break;  /* not an option? */
      switch (argv[i][1]) {  /* option */
        case '-': {  /* `--' */
          if (argv[i][2] != '\0') {
            if (strcmp(argv[i], "--memory-test") == 0) {
              return memory_test_input();
            }
            if (strcmp(argv[i], "--zip-memory-test") == 0) {
              const char *zipname = argv[++i];
              const char *entry_name = argv[++i];
              if (zipname == NULL || entry_name == NULL) {
                l_message(progname,
                          "usage: flowlua.com --zip-memory-test <archive.zip> <entry.lua>");
                return 1;
              }
              return zip_memory_test_input(zipname, entry_name);
            }
            BAS_TRC("Eliot hack for --trace");
            i++;
            break;
          }
          stop_handling_options = 1;  /* loop increment skips this argument and stops option handling */
          break;
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
    }
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