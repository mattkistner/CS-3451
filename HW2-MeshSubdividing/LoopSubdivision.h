// Matthew Kistner

#ifndef __LoopSubdivision_h__
#define __LoopSubdivision_h__
#include <unordered_map>
#include <vector>
#include "Mesh.h"

inline void LoopSubdivision(TriangleMesh<3>& mesh)
{
	std::vector<Vector3>& old_vtx=mesh.Vertices();
	std::vector<Vector3i>& old_tri=mesh.Elements();
	std::vector<Vector3> new_vtx;		////vertex array for the new mesh
	std::vector<Vector3i> new_tri;		////element array for the new mesh
	
	new_vtx=old_vtx;	////copy all the old vertices to the new_vtx array

	////step 1: add mid-point vertices and triangles
	////for each old triangle, 
	////add three new vertices (in the middle of each edge) to new_vtx 
	////add four new triangles to new_tri

	std::unordered_map<Vector2i,int> edge_odd_vtx_map;

	for (int i = 0; i < old_tri.size(); i++) {
		Vector3i triangle = old_tri[i];
		int v1 = triangle[0];
		int v2 = triangle[1];
		int v3 = triangle[2];

		Vector2i e1 = Vector2i(std::min(v1, v2), std::max(v1, v2));
		Vector2i e2 = Vector2i(std::min(v2, v3), std::max(v2, v3));
		Vector2i e3 = Vector2i(std::min(v1, v3), std::max(v1, v3));

		int m1_i;

		if (edge_odd_vtx_map.find(e1) == edge_odd_vtx_map.end()) {
			Vector3 m1 = (old_vtx[v1] + old_vtx[v2]) / 2.0;
			new_vtx.push_back(m1);
			m1_i = new_vtx.size() - 1;
			edge_odd_vtx_map[e1] = m1_i;
		} else {
			m1_i = edge_odd_vtx_map[e1];
		}
		

		int m2_i;
		if (edge_odd_vtx_map.find(e2) == edge_odd_vtx_map.end()){
			Vector3 m2 = (old_vtx[v2] + old_vtx[v3]) / 2.0;
			new_vtx.push_back(m2);
			m2_i = new_vtx.size() - 1;
			edge_odd_vtx_map[e2] = m2_i;

		} else {
			m2_i = edge_odd_vtx_map[e2];
		}
		

		int m3_i;
		if (edge_odd_vtx_map.find(e3) == edge_odd_vtx_map.end()) {
			Vector3 m3 = (old_vtx[v3] + old_vtx[v1]) / 2.0;
			new_vtx.push_back(m3);
			m3_i = new_vtx.size() - 1;
			edge_odd_vtx_map[e3] = m3_i;
		} else {
			m3_i = edge_odd_vtx_map[e3];
		}
		

		new_tri.push_back(Vector3i(m3_i, v1, m1_i));
		new_tri.push_back(Vector3i(m2_i, m3_i, m1_i));
		new_tri.push_back(Vector3i(v3, m3_i, m2_i));
		new_tri.push_back(Vector3i(m2_i, m1_i, v2));

	}

	////step 2: update the position for each new mid-point vertex: 
	////for each mid-point vertex, find its two end-point vertices A and B, 
	////and find the two opposite-side vertices on the two incident triangles C and D,
	////then update the new position as .375*(A+B)+.125*(C+D)

	std::unordered_map<Vector2i,std::vector<int> > edge_tri_map;

	for (int i = 0; i < old_tri.size(); i++) {
		Vector3i triangle = old_tri[i];
		int v1 = triangle[0];
		int v2 = triangle[1];
		int v3 = triangle[2];

		Vector2i e1 = Vector2i(std::min(v1, v2), std::max(v1, v2));
		Vector2i e2 = Vector2i(std::min(v2, v3), std::max(v2, v3));
		Vector2i e3 = Vector2i(std::min(v1, v3), std::max(v1, v3));

		edge_tri_map[e1].push_back(i);
		edge_tri_map[e2].push_back(i);
		edge_tri_map[e3].push_back(i);
	}

	for (auto & iterator: edge_odd_vtx_map) {
		Vector2i e = iterator.first;
		int oddV = iterator.second;
		Vector3 A = old_vtx[e[0]];
		Vector3 B = old_vtx[e[1]];

		int i_A = e[0];
		int i_B = e[1];

		Vector3 C;
		Vector3 D;

		int i_C;
		int i_D;

		auto neighbor_tri = edge_tri_map[e];
		Vector3i t0 = old_tri[neighbor_tri[0]];
		Vector3i t1 = old_tri[neighbor_tri[1]];

		for (int j = 0; j < t0.size(); j++) {
			if (t0[j] != i_A && t0[j] != i_B) {
				i_C = t0[j];
			}
		}

		for (int j = 0; j < t1.size(); j++) {
			if (t1[j] != i_A && t1[j] != i_B) {
				i_D = t1[j];
			}
		}

		C = old_vtx[i_C];
		D = old_vtx[i_D];

		new_vtx[oddV] = ((0.375) * (A + B)) + ((0.125) * (C + D));
	}


	////step 3: update vertices of each old vertex
	////for each old vertex, find its incident old vertices, and update its position according its incident vertices

	std::unordered_map<int,std::vector<int> > vtx_vtx_map;

	std::vector<Vector3>& vtx=mesh.Vertices();
	std::vector<Vector3i>& tri=mesh.Elements();

	for(int i=0;i<tri.size();i++){
		Vector3i& t=tri[i];
		for(int j=0;j<3;j++){
			if(vtx_vtx_map.find(t[j])==vtx_vtx_map.end()){
				vtx_vtx_map[t[j]]=std::vector<int>();
			}
			std::vector<int>& nbs=vtx_vtx_map[t[j]];
			for(int k=0;k<3;k++){
				if(t[k]==t[j])continue;
				if(std::find(nbs.begin(),nbs.end(),t[k])!=nbs.end())continue;
				nbs.push_back(t[k]);
			}
		}
	}

	for (int i = 0; i < old_vtx.size(); i++) {
		int i_v = i;
		auto neighbors = vtx_vtx_map[i_v];
		int n = neighbors.size();
		float beta;

		if (n > 3) {
			beta = (3.0) / (8.0 * n);
		} else {
			beta = (3.0 / 16.0);
		}

		Vector3 posSum = Vector3(0.0, 0.0, 0.0);
		for (int j = 0; j < n; j++) {
			posSum = posSum + (old_vtx[neighbors[j]] * beta);
		}

		new_vtx[i_v] = (1.0-(n * beta)) * (old_vtx[i_v]) + posSum;
	}

	////update subdivided vertices and triangles onto the input mesh
	old_vtx=new_vtx;
	old_tri=new_tri;
}

#endif