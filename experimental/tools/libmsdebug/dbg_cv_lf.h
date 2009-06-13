#ifndef _DBG_CV_LF_HXX
#define _DBG_CV_LF_HXX

#pragma pack (push, 1)

typedef struct CV_Line_t {
  unsigned long offset;
  unsigned long linenumStart;
  unsigned long deltaLineEnd;
  unsigned long fStatement;
} CV_Line_t;
typedef struct CV_Column_t {
  unsigned short offColumnStart;
  unsigned short offColumnEnd;
} CV_Column_t;

typedef struct CV_modifier_t {
  unsigned short MOD_const : 1;
  unsigned short MOD_volatile : 1;
  unsigned short MOD_unaligned : 1;
  unsigned short MOD_unused : 3;
} CV_modifier_t;
static void dump_CV_modifier_t (CV_modifier_t *m, sDbgMemFile *t);

typedef enum eCV_access {
  CV_private = 1,
  CV_protected = 2,
  CV_public = 3,
} eCV_access;

typedef enum eCV_methodprop {
  CV_MTvanilla = 0,
  CV_MTvirtual = 1,
  CV_MTstatic = 2,
  CV_MTfriend = 3,
  CV_MTintro = 4,
  CV_MTpurevirt = 5,
  CV_MTpureintro = 6,
} eCV_methodprop;

typedef struct CV_fldattr_t {
  unsigned short access : 2; /* eCV_access */
  unsigned short mprop : 3;  /* eCV_methodprop */
  unsigned short pseudo : 1;
  unsigned short noinherit : 1;
  unsigned short noconstruct : 1;
  unsigned short compgenx : 1;
  unsigned short sealed : 1;
  unsigned short unused : 6;
} CV_fldattr_t;
static void dump_CV_fldattr_t (CV_fldattr_t *m, sDbgMemFile *t);

typedef struct CV_funcattr_t {
  unsigned char cxxreturnudt : 1;
  unsigned char ctor : 1;
  unsigned char ctorvbase : 1;
  unsigned char unused : 5;
} CV_funcattr_t;
static void dump_CV_funcattr_t (CV_funcattr_t *m, sDbgMemFile *t);

typedef struct CV_prop_t {
  unsigned short packed : 1;
  unsigned short ctor : 1;
  unsigned short ovlops : 1;
  unsigned short isnested : 1;
  unsigned short cnested : 1;
  unsigned short opassign : 1;
  unsigned short opcast : 1;
  unsigned short fwdref : 1;
  unsigned short scoped : 1;
  unsigned short hasuniquename : 1;
  unsigned short sealed : 1;
  unsigned short hfa : 2;
  unsigned short intrinsic : 1;
  unsigned short reserved : 2;
} CV_prop_t;
static void dump_CV_prop_t (CV_prop_t *m, sDbgMemFile *t);

typedef struct lmFunc {
  unsigned short leaf;
  unsigned long rvtype;
  unsigned long classtype;
  unsigned long thistype;
  unsigned char calltype;
  CV_funcattr_t funcattr;
  unsigned short parmcount;
  unsigned long arglist;
  long thisadjust;
} lmFunc;

typedef struct lfArgList {
  unsigned short leaf;
  unsigned long count;
  unsigned long arg[0];
} lfArgList;

typedef struct lfIndex {
  unsigned short leaf;
  unsigned short pad;
  unsigned long index;
} lfIndex;

typedef struct lfVFuncTab {
  unsigned short leaf;
  unsigned short pad;
  unsigned long type;
} lfVFuncTab;

typedef struct lfProc { /* LF_PROCEDURE */
  /* unsigned short leaf; */
  unsigned long rvtype;
  unsigned char calltype;
  CV_funcattr_t funcattr;
  unsigned short parmcount;
  unsigned long arglist;
} lfProc;
static void dump_lfPointer (lfProc *m, sDbgMemFile *t);

typedef struct lfModifier { /* LF_MODIFIER */
  /*unsigned short leaf; */
  unsigned long type;
  CV_modifier_t attr;
} lfModifier;
static void dump_lfModifier (lfModifier *m, sDbgMemFile *t);

typedef struct lfArray { /* LF_ARRAY */
  unsigned short leaf;
  unsigned long elemtype;
  unsigned long idxtype;
  unsigned char data[0];
} lfArray;

typedef struct lfBitfield {
  /* unsigned short leaf; */
  unsigned long type;
  unsigned char length;
  unsigned char position;
} lfBitfield;

typedef struct lfPointerBody {
  unsigned short leaf;
  unsigned long utype;
} lfPointerBody;

typedef struct lfPointer { /* LF_POINTER */
  /*unsigned short leaf;*/
  unsigned long utype;
  struct lfPointerAttr {
    unsigned long ptrtype : 5;
    unsigned long ptrmode : 3;
    unsigned long isflat32 : 1;
    unsigned long isvolatile : 1;
    unsigned long isconst : 1;
    unsigned long isunaligned : 1;
    unsigned long isrestrict : 1;
    unsigned long size : 6;
    unsigned long unused : 13;
  } attr;
  union {
    struct {
      uint32_t pmclass;
      uint16_t pmenum;
    } pm;
    uint16_t bseg;
    unsigned char Sym[];
    struct {
      uint32_t index;
      unsigned char name [];
    } btype;
  } pbase;
} lfPointer;
static void dump_lfProc (lfPointer *m, sDbgMemFile *t);

typedef struct lfChar {
  unsigned short leaf;
  unsigned char val;
} lfChar;

typedef struct lfShort {
  unsigned short leaf;
  short val;
} lfShort;

typedef struct lfUShort {
  unsigned short leaf;
  unsigned short val;
} lfUShort;

typedef struct lfLong {
  unsigned short leaf;
  long val;
} lfLong;

typedef struct lfULong {
  unsigned short leaf;
  unsigned long val;
} lfULong;

typedef struct lfQuad {
  unsigned short leaf;
  unsigned char val[8];
} lfQuad;

typedef struct lfUQuad {
  unsigned short leaf;
  unsigned char val[8];
} lfUQuad;

typedef struct lfClass {
  /*unsigned short leaf;*/
  unsigned short count;
  CV_prop_t property;
  unsigned long field;
  unsigned long derived;
  unsigned long vshape;
  unsigned short size;
  unsigned char data[0];
} lfClass;
static void dump_lfClass (lfClass *m, sDbgMemFile *t);

typedef struct lfStruct { /* LF_STRUCTURE */
  /*unsigned short leaf; */
  unsigned short count;
  CV_prop_t property;
  unsigned long field;
  unsigned long derived;
  unsigned long vshape;
  unsigned short size;
  unsigned char data[0];
} lfStruct;
static void dump_lfStruct (lfStruct *m, sDbgMemFile *t);

typedef struct lfUnion {
  unsigned short leaf;
  unsigned short count;
  CV_prop_t property;
  unsigned long field;
  unsigned char data[0];
} lfUnion;
typedef struct lfEnum { /* LF_ENUMERATE */
  /* unsigned short leaf; */
  unsigned short count;
  CV_prop_t property;
  unsigned long utype;
  unsigned long field;
  unsigned char name[1];
} lfEnum;
static void dump_lfEnum (lfEnum *m, sDbgMemFile *t);

typedef struct lfBClass {
  unsigned short leaf;
  CV_fldattr_t attr;
  unsigned long index;
  unsigned char offset[0];
} lfBClass;
typedef struct lfVBClass {
  unsigned short leaf;
  CV_fldattr_t attr;
  unsigned long index;
  unsigned long vbptr;
  unsigned char vboff[0];
} lfVBClass;

typedef struct lfMember {
  unsigned short leaf;
  CV_fldattr_t attr;
  unsigned long index;
  unsigned char offset[0];
} lfMember;
typedef struct lfSTMember {
  unsigned short leaf;
  CV_fldattr_t attr;
  unsigned long index;
  unsigned char name[1];
} lfSTMember;

typedef struct lfMethod {
  unsigned short leaf;
  unsigned short count;
  unsigned long mList;
  unsigned char name[1];
} lfMethod;

typedef struct lfNestType {
  unsigned short leaf;
  unsigned short pad;
  unsigned long index;
  unsigned char name[1];
} lfNestType;

typedef struct lfOneMethod { /* LF_ONEMETHOD */
  /* unsigned short leaf; */
  CV_fldattr_t attr;
  unsigned long index;
  unsigned long vbaseoff[0];
} lfOneMethod;
static void dump_lfOneMethod (lfOneMethod *m, sDbgMemFile *x);

typedef struct lfFieldList { /* LF_FIELDLIST */
  /* unsigned short leaf; */
  char data[1];
} lfFieldList;
static void dump_lfFieldList (lfFieldList *m, size_t size, sDbgMemFile *t);

typedef struct lfEnumFieldList {
  unsigned short leaf;
  unsigned short pad;
  unsigned short value;
  char name[1];
} lfEnumFieldList;

typedef struct lfMemberFL {
  unsigned short leaf;
  unsigned short pad;
  unsigned long index;
  unsigned short offset;
  char name[1];
} lfMemberFL;

typedef union lfEasy {
  unsigned short leaf;
  lfEnumFieldList enumFL;
  lfMemberFL memberFL; /* LF_MEMBER lfMember */
} lfEasy;

typedef struct _GUID {
  unsigned long data1;
  unsigned short data2;
  unsigned short data3;
  unsigned char data4[8];
} _GUID;

typedef struct lfTypeServer2 { /* LF_TYPESERVER2 */
  unsigned short leaf;
  struct _GUID sig70;
  unsigned long age;
  unsigned char name[0];
} lfTypeServer2;

typedef struct lfPreComp { /* LF_PRECOMP */
  unsigned short leaf;
  unsigned long start;
  unsigned long count;
  unsigned long signature;
  unsigned char name[0];
} lfPreComp;

#define LF_MODIFIER_16t 0x1
#define LF_MODIFIER 0x1001
#define LF_POINTER_16t	0x2
#define LF_ARRAY_16t	0x3
#define LF_CLASS_16t	0x4
#define LF_STRUCTURE_16t 0x5
#define LF_UNION_16t	0x6
#define LF_ENUM_16t	0x7
#define LF_PROCEDURE_16t 0x8
#define LF_MFUNCTION_16t 0x9
#define LF_VTSHAPE 0xa
#define LF_COBOL0_16t 0xb
#define LF_COBOL1     0xc
#define LF_BARRAY_16t 0xd
#define LF_LABEL 0xe
#define LF_NULL 0xf
#define LF_NOTTRAN 0x10
#define LF_DIMARRAY_16t 0x11
#define LF_VFTPATH_16t 0x12
#define LF_PRECOMP_16t 0x13
#define LF_ENDPRECOMP 0x14
#define LF_OEM_16t 0x15
#define LF_TYPESERVER_ST 0x16

typedef enum eDbgCVLeaf {
  LF_SKIP_16t = 0x200,
  LF_ARGLIST_16t = 0x201,
  LF_DEFARG_16t = 0x202,
  LF_LIST = 0x203,
  LF_FIELDLIST_16t = 0x204,
  LF_DERIVED_16t = 0x205,
  LF_BITFIELD_16t = 0x206,
  LF_METHODLIST_16t = 0x207,
  LF_DIMCONU_16t = 0x208,
  LF_DIMCONLU_16t = 0x209,
  LF_DIMVARU_16t = 0x20A,
  LF_DIMVARLU_16t = 0x20B,
  LF_REFSYM = 0x20C,

  LF_BCLASS_16t = 0x400,
  LF_VBCLASS_16t = 0x401,
  LF_IVBCLASS_16t = 0x402,
  LF_ENUMERATE_ST = 0x403,
  LF_FRIENDFCN_16t = 0x404,
  LF_INDEX_16t = 0x405,
  LF_MEMBER_16t = 0x406,
  LF_STMEMBER_16t = 0x407,
  LF_METHOD_16t = 0x408,
  LF_NESTTYPE_16t = 0x409,
  LF_VFUNCTAB_16t = 0x40A,
  LF_FRIENDCLS_16t = 0x40B,
  LF_ONEMETHOD_16t = 0x40C,
  LF_VFUNCOFF_16t = 0x40D,

  LF_TI16_MAX = 0x1000,
  LF_POINTER = 0x1002,
  LF_ARRAY_ST = 0x1003,
  LF_CLASS_ST = 0x1004,
  LF_STRUCTURE_ST = 0x1005,
  LF_UNION_ST = 0x1006,
  LF_ENUM_ST = 0x1007,
  LF_PROCEDURE = 0x1008,
  LF_MFUNCTION = 0x1009,
  LF_COBOL0 = 0x100A,
  LF_BARRAY = 0x100B,
  LF_DIMARRAY_ST = 0x100C,
  LF_VFTPATH = 0x100D,
  LF_PRECOMP_ST = 0x100E,
  LF_OEM = 0x100F,
  LF_ALIAS_ST = 0x1010,
  LF_OEM2 = 0x1011,

  LF_SKIP = 0x1200,
  LF_ARGLIST = 0x1201,
  LF_DEFARG_ST = 0x1202,
  LF_FIELDLIST = 0x1203,
  LF_DERIVED = 0x1204,
  LF_BITFIELD = 0x1205,
  LF_METHODLIST = 0x1206,
  LF_DIMCONU = 0x1207,
  LF_DIMCONLU = 0x1208,
  LF_DIMVARU = 0x1209,
  LF_DIMVARLU = 0x120A,

  LF_BCLASS = 0x1400,
  LF_VBCLASS = 0x1401,
  LF_IVBCLASS = 0x1402,
  LF_FRIENDFCN_ST = 0x1403,
  LF_INDEX = 0x1404,
  LF_MEMBER_ST = 0x1405,
  LF_STMEMBER_ST = 0x1406,
  LF_METHOD_ST = 0x1407,
  LF_NESTTYPE_ST = 0x1408,
  LF_VFUNCTAB = 0x1409,
  LF_FRIENDCLS = 0x140A,
  LF_ONEMETHOD_ST = 0x140B,
  LF_VFUNCOFF = 0x140C,
  LF_NESTTYPEEX_ST = 0x140D,
  LF_MEMBERMODIFY_ST = 0x140E,
  LF_MANAGED_ST = 0x140F,

  LF_ST_MAX = 0x1500,
  LF_TYPESERVER = 0x1501,
  LF_ENUMERATE = 0x1502,
  LF_ARRAY = 0x1503,
  LF_CLASS = 0x1504,
  LF_STRUCTURE = 0x1505,
  LF_UNION = 0x1506,
  LF_ENUM = 0x1507,
  LF_DIMARRAY = 0x1508,
  LF_PRECOMP = 0x1509,
  LF_ALIAS = 0x150A,
  LF_DEFARG = 0x150B,
  LF_FRIENDFCN = 0x150C,
  LF_MEMBER = 0x150D,
  LF_STMEMBER = 0x150E,
  LF_METHOD = 0x150F,
  LF_NESTTYPE = 0x1510,
  LF_ONEMETHOD = 0x1511,
  LF_NESTTYPEEX = 0x1512,
  LF_MEMBERMODIFY = 0x1513,
  LF_MANAGED = 0x1514,
  LF_TYPESERVER2 = 0x1515,

  LF_NUMERIC = 0x8000,
  LF_CHAR = 0x8000,
  LF_SHORT = 0x8001,
  LF_USHORT = 0x8002,
  LF_LONG = 0x8003,
  LF_ULONG = 0x8004,
  LF_REAL32 = 0x8005,
  LF_REAL64 = 0x8006,
  LF_REAL80 = 0x8007,
  LF_REAL128 = 0x8008,
  LF_QUADWORD = 0x8009,
  LF_UQUADWORD = 0x800A,
  LF_REAL48 = 0x800B,
  LF_COMPLEX32 = 0x800C,
  LF_COMPLEX64 = 0x800D,
  LF_COMPLEX80 = 0x800E,
  LF_COMPLEX128 = 0x800F,
  LF_VARSTRING = 0x8010,
  /* If BigInt support.  */
  LF_OCTWORD = 0x8017,
  LF_UOCTWORD = 0x8018,
  LF_DECIMAL = 0x8019,
  LF_DATE = 0x801A,
  LF_UTF8STRING = 0x801B,

  LF_PAD0 = 0xF0,
  LF_PAD1 = 0xF1,
  LF_PAD2 = 0xF2,
  LF_PAD3 = 0xF3,
  LF_PAD4 = 0xF4,
  LF_PAD5 = 0xF5,
  LF_PAD6 = 0xF6,
  LF_PAD7 = 0xF7,
  LF_PAD8 = 0xF8,
  LF_PAD9 = 0xF9,
  LF_PAD10 = 0xFA,
  LF_PAD11 = 0xFB,
  LF_PAD12 = 0xFC,
  LF_PAD13 = 0xFD,
  LF_PAD14 = 0xFE,
  LF_PAD15 = 0xFF,
  eDbgCVLeaf_max
} eDbgCVLeaf;

/* Misc */
typedef struct Scpx8_t {
  float S_re;
  float S_im;
} Scpx8_t;

typedef struct PchHd_s {
  unsigned char signature[12];
  unsigned long version;
  unsigned long sizecheck;
  char buildTimeStamp[20];
} PchHd_s;
typedef struct PchHD_s_pchILsizes {
  long ilsLSym;
  long ilsGSym;
  long ilsExp;
  long ilsInit;
  long ilsDB;
  long I_sbrfp;
} PchHD_s_pchILsizes;
typedef struct PchILsizes {
  long ilsLSym;
  long  ilsGSym;
  long ilsExp;
  long  ilsInit;
  long  ilsDB;
  long  I_sbrfp;
  PchHD_s_pchILsizes ILSizes;
  long ILPos;
  long DirsSize;
  long DirsPos;
  long HeapDumpPos;
  long p_PchSegCount;
  int PchMaxSegCount;
} PchILsizes;

#pragma pack(pop)

#endif
