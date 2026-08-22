#pragma once

#include "Tuning.h"
#include <vector>

// Immutable factory tuning data (PROJECT.md section 4/6). Ids are stable
// identifiers -- never rename an existing id once released, since
// PluginState will persist a user's selected tuning by id.
namespace FactoryTunings
{
    std::vector<Tuning> createAll();
}
