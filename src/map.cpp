#include<math.h>

#include"mapgen.h"
#include"view3d.h"
#include"map.h"


V3D_f tri[3],*ptri[3];
V3D_f *pvout[24],vout[24];
V3D_f *pvtmp[24],vtmp[24];


MAP::MAP(MAPGEN &mapgen,float hflat,float lflat){
	for (int j=0; j<3; j++)
		ptri[j] = &tri[j];
	for (int j=0; j<24; j++) {
		pvtmp[j] = &vtmp[j];
		pvout[j] = &vout[j];
	}

	size=mapgen.size();
	map=new V3D_f*[size];
	for(int i=0;i<size;i++)
		map[i]=new V3D_f[size];

	float min = 256, max = -256;
	for (int i = 0; i < size; i++){
		for (int j = 0; j < size; j++){
			if (min > mapgen.heightmap()[i][j]) min = mapgen.heightmap()[i][j];
			if (max < mapgen.heightmap()[i][j]) max = mapgen.heightmap()[i][j];
		}
	}
	float center=((max - min)/2+min);
	hflat=(hflat*(max - min)+min);
	lflat=(lflat*(max - min)+min);


	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++){
			map[i][j].x=i*tilesize;
//			map[i][j].y=mapgen.heightmap()[i][j]*tilesize-center;	//I center the height around zero.
			map[i][j].y=mapgen.heightmap()[i][j];
			if(hflat>=min && hflat<=max)
				if(map[i][j].y>hflat)	map[i][j].y=hflat;
			if(lflat>=min && lflat<=max)
				if(map[i][j].y<lflat)	map[i][j].y=lflat;
			map[i][j].y=(map[i][j].y-center)*tilesize;
			
//			if(map[i][j].y<0)	map[i][j].y=0;	//Flat sea, or whatever you have below center in the palette.
//			if(map[i][j].y<center)	map[i][j].y=center;
			map[i][j].z=j*tilesize;
			map[i][j].c=getpixel(mapgen.colormap(),i,j);
		}
}



//Currently inoperative...
void MAP::remake(MAPGEN &mapgen){
	for(int i=0;i<size;i++)
		delete [] map[i];
	delete [] map;

		size=mapgen.size();
	map=new V3D_f*[size];
	for(int i=0;i<size;i++)
		map[i]=new V3D_f[size];

	for(int i=0;i<size;i++)
		for(int j=0;j<size;j++){
			map[i][j].x=i*10;
			map[i][j].y=mapgen.heightmap()[i][j];
			map[i][j].z=j*10;
			map[i][j].c=getpixel(mapgen.colormap(),i,j);
		}
}



int out[50];
int nv;
int flags[3];
void render_triangle(BITMAP *bmp){
	// set flags if this vertex is off the edge of the screen 
	for(int c=0;c<3;c++){
		flags[c]=0;
		if (tri[c].x < -tri[c].z)
			flags[c] |= 1;
		else if (tri[c].x > tri[c].z)
			flags[c] |= 2;
		
		if (tri[c].y < -tri[c].z)
			flags[c] |= 4;
		else if (tri[c].y > tri[c].z)
			flags[c] |= 8;
		
		if (tri[c].z < 0.1)
			flags[c] |= 16;
		if (tri[c].z > farlimit)
			flags[c] |= 32;
	}
	// Skip if all vertices are off the same edge of the screen 
	if (flags[0] & flags[1] & flags[2])
		return;//continue;

	// nv: number of vertices after clipping is done 
	// clip if any vertices are off the edge of the screen 
	if (flags[0] | flags[1] | flags[2])
		nv = clip3d_f(POLYTYPE_GRGB, .1, farlimit, 3, (AL_CONST V3D_f**)ptri, pvout, pvtmp, out);
	else{
		nv=3;
		for(int c=0;c<3;c++)
			vout[c]=tri[c];
	}
	if (nv) {
		for (int k=0; k<nv; k++)
			persp_project_f(vout[k].x, vout[k].y, vout[k].z, &vout[k].x, &vout[k].y);

		//Discriminate all that is not normal ;)
		if (polygon_z_normal_f(&vout[0], &vout[1], &vout[2]) > 0){
			polys_drawn++;
			polygon3d_f(bmp, POLYTYPE_GRGB|POLYTYPE_ZBUF, NULL, nv, pvout);
		}
//			scene_polygon3d_f(POLYTYPE_GRGB, NULL, nv, pvout);
	}
}



void MAP::render(BITMAP *bmp, MATRIX_f *camera, int ox, int oy){
	V3D_f vtx[size][size];
	for(int i=0;i<size-1;i++)
		for(int j=0;j<size-1;j++){
			vtx[i][j]=map[i][j];
			apply_matrix_f(camera,map[i][j].x,map[i][j].y,map[i][j].z,&vtx[i][j].x,&vtx[i][j].y,&vtx[i][j].z);
		}

	for(int i=0;i<size-2;i++){
		for(int j=0;j<size-2;j++){
			//Realized that polygon3d doesn't handle vonvex quads, so I split them.
			tri[0]=vtx[i][j];
			tri[1]=vtx[i+1][j];
			tri[2]=vtx[i+1][j+1];
			render_triangle(bmp);
			tri[0]=vtx[i][j];
			tri[1]=vtx[i+1][j+1];
			tri[2]=vtx[i][j+1];
			render_triangle(bmp);
		}
	}
}
