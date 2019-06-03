//--------------------------------------------------------------------------
//
// Errors 
//
//--------------------------------------------------------------------------

#pragma once

namespace SDXEngine
{
	enum SDXErrorId
	{
		SDX_ERROR_NONE = 0,

		// App errors
		SDX_ERROR_APPCLASSREG_FAIL,
		SDX_ERROR_CREATEWINDOW_FAIL,

		// DirectX errors
		SDX_ERROR_DEVICE_CREATE_FAILED,
		SDX_ERROR_DEVICE_ALREADY_EXIST,
		SDX_ERROR_DEVICE_DIRECTX11_NOTSUPPORTED,
		SDX_ERROR_DEVICE_NOT_CREATED,
		SDX_ERROR_CHECKMSAA_FAILED,

		SDX_ERROR_SWAPCHAIN_CREATE_FAILED,
		SDX_ERROR_SWAPCHAIN_ALREADY_EXIST,
		SDX_ERROR_SWAPCHAIN_NO_SET_OUTPUTWINDOW,
		SDX_ERROR_SWAPCHAIN_NOT_CREATED,
		SDX_ERROR_SWAPCHAIN_PRESENT_FAILED,
		SDX_ERROR_SWAPCHAIN_INVALID_MSAAQUALITY,

		SDX_ERROR_RENDERTARGETVIEW_CREATE_FAILED,
		SDX_ERROR_RENDERTARGETVIEW_ALREADY_EXIST,
		SDX_ERROR_RENDERTARGETVIEW_NOT_CREATED,

		SDX_ERROR_DEPTHSTENCIL_ALREADY_EXIST,
		SDX_ERROR_DEPTHSTENCIL_NOT_CREATED,

		SDX_ERROR_VERTEXINPUTLAYOUT_ALREADY_EXIST,
		SDX_ERROR_VERTEXINPUTLAYOUT_NOT_CREATED,
		SDX_ERROR_VERTEXINPUTLAYOUT_UNKNOWN_VERTEX_TYPE,

		SDX_ERROR_RASTERSTATECREATE_FAILED,
		SDX_ERROR_SAMPLERSTATECREATE_FAILED,
		SDX_ERROR_DEPTHSTENCILCREATE_FAILED,

		SDX_ERROR_INVALIDDIRECTX_SETUPINFO,

		SDX_ERROR_PARMETER_DIRECTX_NULLPTR,

		// Shader Loader/MGR errors
		SDX_ERROR_SHADER_COMPILE_FAILED,
		SDX_ERROR_VERTEXSHADER_CREATE_FAILED,
		SDX_ERROR_INPUTLAYOUT_CREATE_FAILED,
		SDX_ERROR_PIXELSHADER_CREATE_FAILED,

		SDX_ERROR_SHADERMGR_DIRECTX_NOTSET,

		SDX_ERROR_SHADERMGR_BIND_CONSTANT_FAILED,
		SDX_ERROR_SHADERMGR_BIND_CONSTANT_ID_ALREADY_EXIST,

		SDX_ERROR_SHADERMGR_INVALID_SHADER_HANDLE,
		SDX_ERROR_SHADERMGR_INVALID_CBUFFER_HANDLE,
		SDX_ERROR_SHADERMGR_NULL_ID,

		// Direct2D errors
		SDX_ERROR_DIRECT2D_NOT_INITIALISED,
		SDX_ERROR_DIRECT2D_NULLPTR_SET,
		SDX_ERROR_DIRECT2D_NOT_SET_DIRECTX,
		SDX_ERROR_DIRECT2D_DEVICE_NOT_CREATED,
		SDX_ERROR_DIRECT2D_DEVICE_ALREADY_CREATED,
		SDX_ERROR_DIRECT2D_DEVICE_CREATE_FAILED,
		SDX_ERROR_DIRECT2D_BITMAPRENDERTARGET_CREATE_FAILED,
		SDX_ERROR_DIRECT2D_BITMAPRENDERTARGET_ALREADY_CREATED,
		SDX_ERROR_DIRECT2D_INITTEXTFORMATS_FAILED,
		SDX_ERROR_DIRECT2D_CREATEBURSHES_FAILED,
		SDX_ERROR_DIRECT2D_TEXTFORMAT_NOT_SETUP,
		SDX_ERROR_DIRECT2D_SETUPTEXTFORMAT_FAILED,
		SDX_ERROR_DIRECT2D_DRAWTEXT_FAILED,

		// Renderer Errors
		SDX_ERROR_RENDERER_CLIENTAREA_NOT_SET,
		SDX_ERROR_RENDERER_OUTPUTWINDOW_NOT_SET,
		SDX_ERROR_RENDERER_INITIALISE_FAILED,

		SDX_ERROR_RENDERER_VERTEXSHADER_ALREADY_CREATED,
		SDX_ERROR_RENDERER_PIXELSHADER_ALREADY_CREATED,
		SDX_ERROR_RENDERER_VERTEXSHADER_CREATE_FAILED,
		SDX_ERROR_RENDERER_PIXELSHADER_CREATE_FAILED,
		SDX_ERROR_RENDERER_INPUTLAYOUT_CREATE_FAILED,
		SDX_ERROR_RENDERER_CONSTANTBUFFER_BIND_FAILED,
		SDX_ERROR_RENDERER_CONSTANTBUFFER_NOT_BOUND,

		// DirectX Compiler errors
		SDX_ERROR_DXCOMPILER_COMPILE_INVALID_ARGS,
		SDX_ERROR_DXCOMPILER_COMPILE_FAILED,

		// MeshData errors
		SDX_ERROR_MESHDATA_VERTEXARRAY_ALREADY_CREATED,
		SDX_ERROR_MESHDATA_CREATE_VERTEXARRAY_COUNT_ZERO,
		SDX_ERROR_MESHDATA_UNKNOWN_VERTEXTYPE,
		SDX_ERROR_MESHDATA_INDEXARRAY_ALREADY_CREATED,
		SDX_ERROR_MESHDATA_CREATE_INDEXARRAY_COUNT_ZERO,

		// Vertex buffer errors
		SDX_ERROR_VERTEXBUFFER_ALREADY_LOADED,
		SDX_ERROR_VERTEXBUFFER_MESHDATA_NULL,
		SDX_ERROR_VERTEXBUFFER_UNKNOWN_VERTEXTYPE,
		SDX_ERROR_VERTEXBUFFER_DIRECTX_NULL,
		SDX_ERROR_VERTEXBUFFER_CREATE_FAILED,

		// Index buffer errors
		SDX_ERROR_INDEXBUFFER_ALREADY_LOADED,
		SDX_ERROR_INDEXBUFFER_MESHDATA_NULL,
		SDX_ERROR_INDEXBUFFER_DIRECTX_NULL,
		SDX_ERROR_INDEXBUFFER_CREATE_FAILED,

		// Mesh errors
		SDX_ERROR_MESH_MESHDATA_NULL,
		SDX_ERROR_MESH_DIRECTX_NULL,

		// SDXLog errors
		SDX_ERROR_LOG_OPEN_FAILED,
		SDX_ERROR_LOG_NO_FILE_OPEN,
		SDX_ERROR_LOG_FILE_ALREADY_OPEN,

		SDX_ERROR_UNKNOWN
	};

	bool IsError(SDXErrorId error);
}