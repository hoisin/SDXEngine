#include "stdafx.h"
#include "SDXErrors.h"

using namespace SDXEngine;

bool SDXEngine::IsError(SDXErrorId error)
{
	return error != SDXErrorId::SDX_ERROR_NONE ? true : false;
}
