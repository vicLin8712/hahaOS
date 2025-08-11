#pragma once

struct sbiret {
    long error;  // sbi return error value a0
    long value;  // sbi return value a1
};

struct sbiret sbi_call(long arg0,
                       long arg1,
                       long arg2,
                       long arg3,
                       long arg4,
                       long arg5,
                       long fid,
                       long eid);

struct sbiret sbi_get_spec_version(void);
struct sbiret sbi_get_mvendorid(void);
