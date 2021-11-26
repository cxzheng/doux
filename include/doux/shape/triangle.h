#pragma once

NAMESPACE_BEGIN(doux::shape)

// A triangle in n-dimensional space.
template <typename T_, size_t D_>
requires(D_ > 1 && std::is_arithmetic_v<T_>) 
class Triangle {
 public:

 private:
  Point<T_, D_>	v_[3];	// three vertices
};

NAMESPACE_END(doux::shape)