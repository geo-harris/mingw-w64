#ifndef _DBG_INTERFACE_HXX
#define _DBG_INTERFACE_HXX

#include "dbg_memfile.h"

typedef enum eInterfaceType
{
  eInterface_error = -1,
  eInterface_unknown = 0,
  eInterface_pdb2 = 1,
  eInterface_pdb7 = 2,
  eInterface_max
} eInterfaceType;

typedef struct sSymbolInterface
{
  int kind;
  size_t elems;
  unsigned char **data;
} sSymbolInterface;

typedef struct sSymbolSearchInterface
{
  int kind;
  size_t size;
  unsigned char *data;
} sSymbolSearchInterface;

typedef struct sDbgInterface
{
  eInterfaceType file_type;
  const char *file_type_name;
  sDbgMemFile *memfile;
  size_t cb_size;
  int (*probe)(const sDbgMemFile *);
  int (*load)(struct sDbgInterface *pDCtx);
  int (*update)(struct sDbgInterface *pDCtx);
  int (*release)(struct sDbgInterface *pDCtx);
  sDbgMemFile *(*dump)(struct sDbgInterface *pDCtx);
  sSymbolInterface *(*search)(struct sDbgInterface *pDCtx, sSymbolSearchInterface *match);
  sDbgMemFile *(*dump_symbol)(struct sDbgInterface *pDCtx, sSymbolInterface *pSym);
} sDbgInterface;

/* Default implementations.  */
int unknown_probe (const sDbgMemFile *pDFile);
int unknown_load (sDbgInterface *pDCtx);
int unknown_update (sDbgInterface *pDCtx);
int unknown_release (sDbgInterface *pDCtx);
sDbgMemFile *unknown_dump (sDbgInterface *pDCtx);
sSymbolInterface *unknown_search(struct sDbgInterface *pDCtx, sSymbolSearchInterface *match);
sDbgMemFile *unknown_dump_symbol(struct sDbgInterface *pDCtx, sSymbolInterface *pSym);

eInterfaceType dbg_probe_file (const char *filename);
sDbgInterface *dbg_open_file (const char *filename);
int dbg_release_interface (sDbgInterface *pDCtx);

#endif
