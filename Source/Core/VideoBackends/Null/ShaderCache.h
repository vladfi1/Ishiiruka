// Copyright 2015 Dolphin Emulator Project
// Licensed under GPLv2+
// Refer to the license.txt file included.

#pragma once

#include <map>
#include <memory>

#include "VideoCommon/GeometryShaderGen.h"
#include "VideoCommon/PixelShaderGen.h"
#include "VideoCommon/VertexShaderGen.h"
#include "VideoCommon/VideoCommon.h"
#include "VideoCommon/VertexLoaderManager.h"

namespace Null
{
template <typename Uid>
class ShaderCache
{
public:
  ShaderCache();
  virtual ~ShaderCache();

  void Clear();
  bool SetShader(PrimitiveType primitive_type);

protected:
  virtual Uid GetUid(PrimitiveType primitive_type) = 0;
  virtual ShaderCode GenerateCode(Uid uid) = 0;

private:
  std::map<Uid, std::string> m_shaders;
  const std::string* m_last_entry = nullptr;
  Uid m_last_uid;
};

class VertexShaderCache : public ShaderCache<VertexShaderUid>
{
public:
  static std::unique_ptr<VertexShaderCache> s_instance;

protected:
  VertexShaderUid GetUid(PrimitiveType primitive_type) override
  {
    VertexShaderUid uid;
    std::cout << "VertexShaderCache.GetUid\n";
    GetVertexShaderUID(uid, VertexLoaderManager::g_current_components, xfmem, bpmem);
    return uid;
  }
  ShaderCode GenerateCode(VertexShaderUid uid) override
  {
    ShaderCode code;
    std::cout << "VertexShaderCache.GenerateCode\n";
    GenerateVertexShaderCodeGL(code, uid.GetUidData());
    return code;
  }
};

class GeometryShaderCache : public ShaderCache<GeometryShaderUid>
{
public:
  static std::unique_ptr<GeometryShaderCache> s_instance;

protected:
  GeometryShaderUid GetUid(PrimitiveType primitive_type) override
  {
    std::cout << "GeometryShaderCache.GetUid\n";
    GeometryShaderUid uid;
    GetGeometryShaderUid(uid, primitive_type, xfmem, VertexLoaderManager::g_current_components);
    return uid;
  }
  ShaderCode GenerateCode(GeometryShaderUid uid) override
  {
    ShaderCode code;
    std::cout << "GeometryShaderCache.GenerateCode\n";
    GenerateGeometryShaderCode(code, uid.GetUidData(), API_OPENGL);
    return code;
  }
};

class PixelShaderCache : public ShaderCache<PixelShaderUid>
{
public:
  static std::unique_ptr<PixelShaderCache> s_instance;

protected:
  PixelShaderUid GetUid(PrimitiveType primitive_type) override
  {
    std::cout << "PixelShaderCache.GetUid\n";
    PixelShaderUid uid;
    GetPixelShaderUID(uid, PSRM_DEFAULT, VertexLoaderManager::g_current_components, xfmem, bpmem);
    return uid;
  }
  ShaderCode GenerateCode(PixelShaderUid uid) override
  {
    ShaderCode code;
    std::cout << "PixelShaderCache.GenerateCode\n";
    GeneratePixelShaderCodeGL(code, uid.GetUidData());
    return code;
  }
};

}  // namespace NULL
