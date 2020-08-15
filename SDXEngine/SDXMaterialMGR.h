#pragma once

#include <map>
#include <string>
#include "SDXGfx.h"
#include "SDXErrors.h"

namespace SDXEngine
{
	class SDXMaterialMGR
	{
	public:
		SDXMaterialMGR();
		~SDXMaterialMGR();

		SDXErrorId AddMaterial(const std::string& id, SMaterial material);

		SMaterial GetMaterial(const std::string& id);

	private:
		std::map<std::string, SMaterial> m_materialMap;
	};
}