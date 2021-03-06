/**
 * This file has no copyright assigned and is placed in the Public Domain.
 * This file is part of the w64 mingw-runtime package.
 * No warranty is given; refer to the file DISCLAIMER.PD within this package.
 */
#ifndef _INC_VSMGT
#define _INC_VSMGT

#ifdef __cplusplus
extern "C" {
#endif
typedef PWSTR VSS_PWSZ; /*assumed*/

typedef enum _VSS_PROTECTION_FAULT {
  VSS_PROTECTION_FAULT_NONE                           = 0,
  VSS_PROTECTION_FAULT_DIFF_AREA_MISSING              = ( VSS_PROTECTION_FAULT_NONE + 1 ),
  VSS_PROTECTION_FAULT_IO_FAILURE_DURING_ONLINE       = ( VSS_PROTECTION_FAULT_DIFF_AREA_MISSING + 1 ),
  VSS_PROTECTION_FAULT_META_DATA_CORRUPTION           = ( VSS_PROTECTION_FAULT_IO_FAILURE_DURING_ONLINE + 1 ),
  VSS_PROTECTION_FAULT_MEMORY_ALLOCATION_FAILURE      = ( VSS_PROTECTION_FAULT_META_DATA_CORRUPTION + 1 ),
  VSS_PROTECTION_FAULT_MAPPED_MEMORY_FAILURE          = ( VSS_PROTECTION_FAULT_MEMORY_ALLOCATION_FAILURE + 1 ),
  VSS_PROTECTION_FAULT_COW_READ_FAILURE               = ( VSS_PROTECTION_FAULT_MAPPED_MEMORY_FAILURE + 1 ),
  VSS_PROTECTION_FAULT_COW_WRITE_FAILURE              = ( VSS_PROTECTION_FAULT_COW_READ_FAILURE + 1 ),
  VSS_PROTECTION_FAULT_DIFF_AREA_FULL                 = ( VSS_PROTECTION_FAULT_COW_WRITE_FAILURE + 1 ),
  VSS_PROTECTION_FAULT_GROW_TOO_SLOW                  = ( VSS_PROTECTION_FAULT_DIFF_AREA_FULL + 1 ),
  VSS_PROTECTION_FAULT_GROW_FAILED                    = ( VSS_PROTECTION_FAULT_GROW_TOO_SLOW + 1 ),
  VSS_PROTECTION_FAULT_DESTROY_ALL_SNAPSHOTS          = ( VSS_PROTECTION_FAULT_GROW_FAILED + 1 ),
  VSS_PROTECTION_FAULT_FILE_SYSTEM_FAILURE            = ( VSS_PROTECTION_FAULT_DESTROY_ALL_SNAPSHOTS + 1 ),
  VSS_PROTECTION_FAULT_IO_FAILURE                     = ( VSS_PROTECTION_FAULT_FILE_SYSTEM_FAILURE + 1 ),
  VSS_PROTECTION_FAULT_DIFF_AREA_REMOVED              = ( VSS_PROTECTION_FAULT_IO_FAILURE + 1 ),
  VSS_PROTECTION_FAULT_EXTERNAL_WRITER_TO_DIFF_AREA   = ( VSS_PROTECTION_FAULT_DIFF_AREA_REMOVED + 1 ) 
} VSS_PROTECTION_FAULT;

typedef enum _VSS_PROTECTION_LEVEL {
  VSS_PROTECTION_LEVEL_ORIGINAL_VOLUME   = 0,
  VSS_PROTECTION_LEVEL_SNAPSHOT          = ( VSS_PROTECTION_LEVEL_ORIGINAL_VOLUME + 1 ) 
} VSS_PROTECTION_LEVEL;

typedef struct _VSS_DIFF_AREA_PROP {
  VSS_PWSZ m_pwszVolumeName;
  VSS_PWSZ m_pwszDiffAreaVolumeName;
  LONGLONG m_llMaximumDiffSpace;
  LONGLONG m_llAllocatedDiffSpace;
  LONGLONG m_llUsedDiffSpace;
} VSS_DIFF_AREA_PROP, *PVSS_DIFF_AREA_PROP;

typedef struct _VSS_DIFF_VOLUME_PROP {
  VSS_PWSZ m_pwszVolumeName;
  VSS_PWSZ m_pwszVolumeDisplayName;
  LONGLONG m_llVolumeFreeSpace;
  LONGLONG m_llVolumeTotalSpace;
} VSS_DIFF_VOLUME_PROP, *PVSS_DIFF_VOLUME_PROP;

typedef union _VSS_MGMT_OBJECT_UNION {
  VSS_VOLUME_PROP      Vol;
  VSS_DIFF_VOLUME_PROP DiffVol;
  VSS_DIFF_AREA_PROP   DiffArea;
} VSS_MGMT_OBJECT_UNION, *PVSS_MGMT_OBJECT_UNION;

typedef struct _VSS_MGMT_OBJECT_PROP {
  VSS_MGMT_OBJECT_TYPE  Type;
  VSS_MGMT_OBJECT_UNION Obj;
} VSS_MGMT_OBJECT_PROP, *PVSS_MGMT_OBJECT_PROP;

typedef enum _VSS_MGMT_OBJECT_TYPE {
  VSS_MGMT_OBJECT_UNKNOWN       = 0,
  VSS_MGMT_OBJECT_VOLUME        = 1,
  VSS_MGMT_OBJECT_DIFF_VOLUME   = 2,
  VSS_MGMT_OBJECT_DIFF_AREA     = 3 
} VSS_MGMT_OBJECT_TYPE, *PVSS_MGMT_OBJECT_TYPE;

typedef struct _VSS_VOLUME_PROP {
  VSS_PWSZ m_pwszVolumeName;
  VSS_PWSZ m_pwszVolumeDisplayName;
} VSS_VOLUME_PROP, *PVSS_VOLUME_PROP;

typedef struct _VSS_VOLUME_PROTECTION_INFO {
  VSS_PROTECTION_LEVEL m_protectionLevel;
  WINBOOL              m_volumeIsOfflineForProtection;
  VSS_PROTECTION_FAULT m_protectionFault;
  LONG                 m_failureStatus;
  WINBOOL              m_volumeHasUnusedDiffArea;
  DWORD                m_reserved;
} VSS_VOLUME_PROTECTION_INFO;

#ifdef __cplusplus
}
#endif

#endif /*_INC_VSMGT*/
