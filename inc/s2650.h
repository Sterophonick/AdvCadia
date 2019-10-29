extern u8 psl;
extern u8 psu;
extern u8 r[7];
extern u16 ras[8];

extern u8 memory[8192];
extern u8 opbytes[256];

#define KILOBYTE           1024
#define MEMSIZE            (8 * KILOBYTE)

#define PSU_S            128
#define PSU_F             64
#define PSU_II            32

#define AUTO               auto   /* automatic variables */
#define TRANSIENT          auto   /* automatic variables */
#define MODULE             static /* external static (file-scope) */
#define PERSIST            static /* internal static (function-scope) */
#define DISCARD            (void) /* discarded return values */
#define elif               else if
#define acase              break; case
#define adefault           break; default
#define EXPORT
#define IMPORT             extern
#define UNUSED


#define PSU_SP             7 // %00000111
#define PSU_WRITABLE    0x67 // %01100111
#define PSU_READONLY    0x98 // %10011000
#define PSL_CC 192 // $11000000
#define PSL_IDC 32
#define PSL_RS  16
#define PSL_WC   8
#define PSL_OVF  4
#define PSL_COM  2
#define PSL_C    1
#define PMSK    0x1fff          /* mask page offset */
#define PLEN    0x2000          /* page length */
#define PAGE    0x6000          /* mask page */
#define AMSK    0x7fff          /* mask address range */

extern void s2650_execute();
extern void(*op_table[])(void);

extern s8 rel;
extern u16 abst;
extern u16 ind;
extern u8 imm;
extern u16  iar;