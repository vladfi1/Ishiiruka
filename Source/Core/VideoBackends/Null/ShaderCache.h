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
    GetVertexShaderUID(uid, 0, xfmem, bpmem);
    return uid;
  }
  ShaderCode GenerateCode(VertexShaderUid uid) override
  {
    ShaderCode code;
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
    GeometryShaderUid uid;
    GetGeometryShaderUid(uid, primitive_type, xfmem, 0);
    return uid;
  }
  ShaderCode GenerateCode(GeometryShaderUid uid) override
  {
    ShaderCode code;
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
    PixelShaderUid uid;
    GetPixelShaderUID(uid, 0, 0, xfmem, bpmem);
    return uid;
  }
  ShaderCode GenerateCode(PixelShaderUid uid) override
  {
    ShaderCode code;
    GeneratePixelShaderCodeGL(code, uid.GetUidData());
    return code;
  }
};

}  // namespace NULL
