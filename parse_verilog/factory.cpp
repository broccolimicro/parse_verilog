#include "factory.h"
#include "module.h"

namespace parse_verilog {

const parse::factory factory(parse::schema::from<module_def>());

} 
