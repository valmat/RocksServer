// #include <unistd.h>
#include <sys/resource.h> // C headers for `getrlimit()`


#include "change_files_limit.h"

ChangingFilesLimitResult change_files_limit(unsigned short nofile_limit) noexcept
{
    if(0 == nofile_limit) { return {};}

    rlimit rlim;
    getrlimit(RLIMIT_NOFILE, &rlim);
    const rlimit i_rlimit {nofile_limit, rlim.rlim_max};
    
    return (-1 == setrlimit(RLIMIT_NOFILE, &i_rlimit)) ?
        ChangingFilesLimitResult{false, rlim.rlim_cur, rlim.rlim_max} :
        ChangingFilesLimitResult{true,  rlim.rlim_cur, rlim.rlim_max};
}