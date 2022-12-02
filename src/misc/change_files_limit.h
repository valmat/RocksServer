#pragma once

struct ChangingFilesLimitResult
{
    bool successful = true;
    unsigned long soft_limit = 0;
    unsigned long hard_limit = 0;
    operator bool() const noexcept {return successful;}
};

// Changes soft limit on the number of simultaneously opened files (if required)
// Keep `nofile_limit = 0` if changing is not required
// Returns true if changing was required and was unsuccessful
ChangingFilesLimitResult change_files_limit(unsigned short nofile_limit = 0) noexcept;