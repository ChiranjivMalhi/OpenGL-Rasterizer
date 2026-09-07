#pragma once
#pragma warning(push)
#pragma warning(disable: 4505)  
#pragma warning(disable: 4702)  
#pragma warning(disable: 4800) 

namespace gli {
	namespace gl {
		enum format {};
	}
}

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#include <gli/gli.hpp>

#pragma GCC diagnostic pop
#pragma warning(pop)
