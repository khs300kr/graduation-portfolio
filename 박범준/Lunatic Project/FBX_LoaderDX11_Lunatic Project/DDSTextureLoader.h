

#ifdef _MSC_VER
#pragma once
#endif

#include <d3d11.h>

#pragma warning(push)
#pragma warning(disable : 4005)
#include <stdint.h>
#pragma warning(pop)

#if defined(_MSC_VER) && (_MSC_VER<1610) && !defined(_In_reads_)
#define _In_reads_(exp) _In_count_x_(exp)
#define _Out_writes_(exp) _Out_cap_x_(exp)
#define _In_reads_bytes_(exp) _In_bytecount_x_(exp)
#endif

HRESULT CreateDDSTextureFromMemory( _In_ ID3D11Device* d3dDevice,
                                    _In_reads_bytes_(ddsDataSize) const uint8_t* ddsData,
                                    _In_ size_t ddsDataSize,
                                    _Out_opt_ ID3D11Resource** texture,
                                    _Out_opt_ ID3D11ShaderResourceView** textureView,
                                    _In_ size_t maxsize = 0
                                  );

HRESULT CreateDDSTextureFromFile( _In_ ID3D11Device* d3dDevice,
                                  _In_z_ const wchar_t* szFileName,
                                  _Out_opt_ ID3D11Resource** texture,
                                  _Out_opt_ ID3D11ShaderResourceView** textureView,
                                  _In_ size_t maxsize = 0
                                );
