#pragma once

//    true if current user is not root
// or true if owner changing is not required (targ_uid == 0)
// or true if current user is root and uid changed
// false otherwise (owner changing was unsuccessful)
bool change_owner(unsigned int targ_uid) noexcept;