#pragma once
#include <vector>

class Vector {
public:

	Vector(size_t n);
	Vector(float* data, size_t n, size_t stride);
	~Vector();

	size_t size() { return m_Size; }
	void print() const;

	float& operator[](size_t i);
	float operator[](size_t i) const;

private:
	float* m_Data;
	size_t m_Size;
	size_t m_Stride;
	bool m_IsMemoryOwner;
};