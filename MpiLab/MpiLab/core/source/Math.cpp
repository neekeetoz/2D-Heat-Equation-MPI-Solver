#include "../headers/Math.h"


void thomasAlgorithm(Vector& a, Vector& b, Vector& c, Vector& d, Vector& out) {
	size_t n = a.size();
	float w;
	// Forward sweep
	for (size_t i = 1; i < n; i++) {
		w = a[i] / b[i - 1];
		b[i] = b[i] - w * c[i - 1];
		d[i] = d[i] - w * d[i - 1];
	}

	out[n - 1] = d[n - 1] / b[n - 1];

	// Backward sweep
	for (int i = n - 2; i > -1; i--)
		out[i] = (d[i] - c[i] * out[i + 1]) / b[i];
}

void transposeTiled(float* A, float* B, int N, int tile_size) {
	for (int i = 0; i < N; i += tile_size) {
		for (int j = 0; j < N; j += tile_size) {
			for (int p = 0; p < tile_size; p++) {
				for (int q = 0; q < tile_size; q++) {
					B[IND(i + p, j + q, N)] = A[IND(j + q, i + p, N)];
				}
			}
		}
	}
}