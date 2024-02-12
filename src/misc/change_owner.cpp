#include <unistd.h>

#include "change_owner.h"

//    true if current user is not root
// or true if owner changing is not required (targ_uid == 0)
// or true if current user is root and uid changed
// false otherwise (owner changing was unsuccessful)
bool change_owner(unsigned int targ_uid, unsigned int targ_gid) noexcept
{
    return
        (0u != getuid()) || // made sure that src_uid  == 0 (source is root)
        (0u == targ_uid) || // made sure that targ_uid != 0 (target is not root)
        (0 == (setgid(targ_gid) & setuid(targ_uid))); // set the gid and then the uid and then checked the result
}