#pragma once

#include <cstdint>

namespace gfx {

enum class VertexSemantic : uint8_t {
  Position,
  Normal,
  Tangent,
  Color,
  UVCoord,
  BoneIndices,
  BoneWeights,
};

enum class VertexFormat : uint8_t {
  Float,
  Float2,
  Float3,
  Float4,

  UInt,
  UInt2,
  UInt3,
  UInt4,
};

struct VertexStream {
  VertexSemantic semantic;
  VertexFormat format;

  const void* data;

  uint32_t stride;
};

struct MeshSource {
  const VertexStream* streams;
  uint32_t stream_count;

  const uint32_t* indices;
  uint32_t index_count;

  uint32_t vertex_count;
};

}  // namespace gfx