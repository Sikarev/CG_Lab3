#pragma once

#include "Camera3D.h"
#include "Model3D.h"

class Scene3D : public Camera3D {
public:
	Model3D Model;

	Scene3D(double X0, double Y0, double px, double py, string vData, string fData) : Camera3D(X0, Y0, px, py) { SetModel(vData, fData); };

	void SetModel(string vData, string fData) {
		Model = Model3D(vData, fData);
		Model.Project(WorldToProject);
	}
	void Apply(Matrix<> AT) {
		Model.Apply(AT);
		Model.Project(WorldToProject);
	}
	void Render(HDC dc) {
		int rows = Model.GetEdges().RowsCount();
		int cols = Model.GetEdges().ColsCount();

		for (int i = 1; i <= rows; i++) {
			for (int j = i + 1; j <= cols; j++) {
				if (Model.isBinded(i, j)) {
					MoveTo(Model.getProjVertexX(i), Model.getProjVertexY(i));
					LineTo(dc, Model.getProjVertexX(j), Model.getProjVertexY(j));
				}
			}
		}
	}
};