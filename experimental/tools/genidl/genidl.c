#define _CRT_SECURE_NO_WARNINGS
#include "genidl_cfg.h"
#include "genidl_readpe.h"
#include "genidl_typeinfo.h"
#include "genidl_typinfo.h"

/* Configure globals.  */
int show_dump_too = 0;
int is_verbose = 0;

int genidl_read_config (const char *fname);

/* Process files.  */
size_t file_args_cnt = 0;
char **file_args = NULL;
const char *basedumpname = "test_dump";

static void
show_usage (void)
{
  fprintf (stderr, "Usage: genidl [OPTION]... [FILE]...\n");
  fprintf (stderr, "Dumps IDL information from typelib data found in PE32/PE32+ executables and\n"
                   "TLB files.\n");
  fprintf (stderr, "\n");
  fprintf (stderr, "Options:\n"
    "  -b ARG, --basedumpname=ARG\n"
    "                           Specify ARG as prefix of generated idl files.\n"
    "  -d, --dump               Dump additional internal debugging information.\n"
    "  -v, --verbose            Show additional status prints.\n"
    "  -h, --help               Show this help.\n"
  );
  fprintf (stderr, "\nReport bugs to <mingw-w64-public@lists.sourceforge.net>\n");
  exit (1);
}

static int
scanArgs (int argc, char **argv)
{
  int seen_error = 0;
  if (!argc)
    return -1;
  file_args = (char **) malloc (sizeof (char *) * argc);
  while (argc > 0)
    {
      char *h = *argv;
      if (h[0]  == '-')
	{
	  h++;
	  switch (*h) {
      case '-': /* Long arguments section */
        h++;
        switch (*h) {
        case 'd':
            if(! strcmp (h, "dump"))
                {
                    show_dump_too = 1;
                    break;
                }
            else
                goto unknown_fail;
        case 'h':
            if(! strcmp (h, "help")) return -2;
            goto unknown_fail;
        case 'v':
	    if (! strcmp (h, "verbose"))
	       is_verbose++;
	    else
	      goto unknown_fail;
	    break;
        case 'b':
            if (! strncmp(h, "basedumpname=", 13))
                {
                    basedumpname = &(h[13]);
                    break;
                }
            else
                goto unknown_fail;
        default: goto unknown_fail;
        }
        break;
        /* Short arguments section */
	  case 'd':
	    if (h[1] == 0)
	      show_dump_too = 1;
	    else
	      goto unknown_fail;
	    break;
	  case 'h':
	    if (h[1] == 0)
	      return -2;
	       goto unknown_fail;
	  case 'b':
	    if (h[1] == 0)
	      {
		basedumpname = *(++argv);
		--argc;
		break;
	      }
	    goto unknown_fail;
	  case 'v':
	    if (h[1] != 0)
	      goto unknown_fail;
	    is_verbose++;
	    break;
	  default:
unknown_fail:
	    fprintf (stderr, "Option %s' is unknown.\n", *argv);
	    seen_error = 1;
	    break;
	  }
	}
      else
	{
	  file_args[file_args_cnt++] = *argv;
	}
      --argc;
      ++argv;
    }
  if (seen_error || file_args_cnt == 0)
    return -2;
  return 0;
}

int main(int argc,char **argv)
{
  FILE *fp,*gp;
  long p;
  int32_t be64;
  size_t len;
  size_t i;
  unsigned char *dta = NULL;
  if (scanArgs (--argc, ++argv) < 0)
    {
       show_usage ();
    }
  genidl_read_config ("./genidl.conf");
  for (i=0;i<file_args_cnt;i++)
    {
      char s[1024];
      int start, end;
      gp = fopen (file_args[i], "rb");
      if (!gp)
	{
	  fprintf (stderr, "Failed to open file ,%s'.\n", file_args[i]);
	  continue;
	}
      p = genidl_ispe (gp, &be64);

      if (is_verbose)
        fprintf (stderr, "Found PE at %ld (%s bits)\n", p, be64 ? "32" : "64");

      end = genidl_pe_typelib_resource_count (gp);

      if (is_verbose)
	fprintf (stderr, "Contains %d typelib resource(s)\n", end);

      for (start = 0; start < end; start++)
	{
      genidl_pe_typelib_resource_read (gp, start, &dta, &len);
      sprintf (s, "%s_%d_%d.idl", basedumpname, (int) i, start);
      fp = fopen (s,"wt");
      {
	sTI2TypLib *tl = TI2_typlib_init (dta, (size_t) len);
	if (tl)
	  {
	    TI2_typlib_idl (fp, tl);
	    TI2_typlib_dest (tl);
	  }
	if (show_dump_too)
	  dumpInfo (fp, dta, len);
      }
      fclose (fp);
	}
      fclose (gp);
    }
  return 0;
}
