#define MKOPENFLAG(_flag_)  \
   ((_flag_) & (FMO_CREATE | FMO_IFEXIST | FMO_REPLACE))
#define MKOPENMODE(_flag_) \
   (((_flag_) & 0x7fff0000) >> 16)


#define fmIsNotCurDir(pszDir) \
((*((PULONG)(pszDir)) & 0x0000ffff) ^ 0x0000002e)

#define fmIsNotParDir(pszDir) \
((*((PULONG)(pszDir)) & 0x00ffffff) ^ 0x00002e2e)

#define fmIsCurDir(pszDir) \
((*((PULONG)(pszDir)) & 0x0000ffff) == 0x0000002e)

#define fmIsParDir(pszDir) \
((*((PULONG)(pszDir)) & 0x00ffffff) == 0x00002e2e)

// fileIO definitions ---------------------------------------------------------

#define FILEATTR_ANY \
   (FILE_ARCHIVED | FILE_SYSTEM | FILE_HIDDEN | FILE_READONLY)
#define FILEATTR_RECUR       (FILEATTR_ANY | FILE_DIRECTORY)
// file open error
#define FMO_ERROR            ((HFILE)-1)
// creating flags
#define FMO_FAILIFNEW        0
#define FMO_CREATE           OPEN_ACTION_CREATE_IF_NEW
// mutually exclusive
#define FMO_NOTIFEXIST       0
#define FMO_IFEXIST          OPEN_ACTION_OPEN_IF_EXISTS
#define FMO_REPLACE          OPEN_ACTION_REPLACE_IF_EXISTS
// sharing flags (mutually exclusive)
#define FMO_SHARENONE        (OPEN_SHARE_DENYREADWRITE << 16)
#define FMO_SHAREREAD        (OPEN_SHARE_DENYWRITE << 16)
#define FMO_SHAREWRITE       (OPEN_SHARE_DENYREAD << 16)
#define FMO_SHAREALL         (OPEN_SHARE_DENYNONE << 16)
// access flags (mutually exclusive)
#define FMO_READ             (OPEN_ACCESS_READONLY << 16)
#define FMO_WRITE            (OPEN_ACCESS_WRITEONLY << 16)
#define FMO_READWRITE        (OPEN_ACCESS_READWRITE << 16)
#define FMO_APPEND           (0x80000000 | FMO_WRITE)

