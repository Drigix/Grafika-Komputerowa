#pragma once

#include "GK lab4.h"


typedef boost::gil::bgr8_pixel_t Pixel;
typedef boost::gil::bgr8_view_t View;
typedef boost::gil::bgr8c_view_t ViewReadOnly;
typedef boost::gil::bgr8_view_t::x_iterator Iterator_X;
typedef boost::gil::bgr8_view_t::y_iterator Iterator_Y;
typedef boost::gil::bgr8_view_t::locator Iterator_XY;


/// READ ME FIRST
void TheLookAtMeFakeFunction(View& view, const unsigned short& x, const unsigned short& y)
{
	// a View is the access to image's pixels ect.

	// pixel access, slow but easy to understand
	view(x, y);
	view(x, y) = Pixel(0, 0, 255); // blue, green, red channels

	// pixel access, optimalize for small x/y increments
	Iterator_XY position = view.xy_at(x, y);
	*position = Pixel(0, 0, 255);
	++position.x();		// point at pixel(x+1, y)
	--position.y();		// point at pixel(x, y-1)
	
	// pixel access, row and column interators
	unsigned int row_number = 0;
	unsigned int column_number = 0;
	Iterator_X itr_x = view.row_begin(row_number);
	Iterator_Y itr_y = view.col_begin(column_number);

	// the Windows applications coordinates system is used in this code
	// origin point (0, 0) in the left upper corner
	// do not forget: pixel(b,g,r), full_red(0, 0, 255), bright_red(130, 130, 255)
};

////////////////////////////////////////////////////////////////////////////////
#define NFACES 7			// number of faces
#define MAXDEPTH 1000		// maximal value of z

struct coord3d { int x, y, z;};

coord3d frame_faces [NFACES][3];
coord3d base_faces  [NFACES][3];
coord3d random_faces[NFACES][3];

Pixel colours[NFACES];

float depth[480][600];		// depth buffer

////////////////////////////////////////////////////////////////////////////////


// Rounding
inline int Round(float x)
{
	return (floor (x + 0.5f));
}

// Random
BYTE RandomFromZeroTo(BYTE value)
{
	return rand() % value;
}

// fills a line
void FillLine(View& view, float* depth,
			  const Pixel& colour, const unsigned short& row,
			  const unsigned short& x1, const unsigned short& x2,
			  const float& z1, const float& z2)
{
	if (row >= view.height() || x1 >= view.width() || x2 >= view.width() /*|| x2 < x1*/)
	{
		return;
	};
	//TODO 2:
	//Należy wykonać Test bufora Z dla każdego piksela z linii 'row' oraz uzupełnić bufor Z
	//oraz bufor koloru o nowe wartośći
	//Dostęp do bufora kolorów odbywa się poprzez iterator:
	//Iterator_X itr = view.x_at(x1, row);
	//dostęp do następnego piksela odbywa się poprzez zwiększenie iteratora: ++itr
	//kolor przyporządkowywuje się poprzez operator wyłuskania: *itr = colour;

	float z = z1;
	float z_delta = (z2 - z1)/(x2 - x1);

	int x =x1;

	for(x; x <= x2; x++){
		if(z < depth[x]){
			view(x, row) = colour;
			depth[x] = z;
		}
		z += z_delta;
	}

}

// initialize basic variables and creat random faces
void InitializeLab(const POINT& client_area_size)
{
	// setting vertices for each face
	for (unsigned short i = 0; i < NFACES; ++i)
	{
		colours[i] = Pixel(RandomFromZeroTo(255), RandomFromZeroTo(255), RandomFromZeroTo(255));
		
		for (unsigned short j = 0; j < 3; ++j)
		{
			random_faces[i][j].x = RandomFromZeroTo(client_area_size.x);
			random_faces[i][j].y = RandomFromZeroTo(client_area_size.y);
			random_faces[i][j].z = RandomFromZeroTo(MAXDEPTH);
		}
	}
}

// 
void RemoveHiddenSurfaces(View& view, coord3d faces_2d_projection[NFACES][3])
{
	// sorting
	unsigned short point_1, point_2, point_3;
	float x1, y1, z1, x2, y2, z2, x3, y3, z3;
	// x and z calculations
	float dz12, dz13, dz23;
	float dx12, dx13, dx23;
	float dy;
	// filling triangle
	unsigned short y;
	float xr1, xr2, zr1, zr2;
	float xp1, xp2, zp1, zp2;

	// initialize depth, the image is clear elsewhere
	for (unsigned short i = 0; i < view.height(); i++)
	{
		for (unsigned short j = 0; j < view.width(); j++)
		{
			depth[i][j] = 2147483646;  // max_int
		}
	}
	
	for (unsigned short i = 0; i < NFACES; ++i)
	{
		// simple sorting verteces by increasing y
		point_1 = 0;
		point_2 = 1;
		point_3 = 2;
		if (faces_2d_projection[i][1].y < faces_2d_projection[i][0].y)
		{
			point_1 = 1;
			point_2 = 0;
			point_3 = 2;
		}
		if (faces_2d_projection[i][2].y < faces_2d_projection[i][point_1].y)
		{
			point_1 = 2;
			point_2 = 0;
			point_3 = 1;
		}
		x1 = faces_2d_projection[i][point_1].x;
		y1 = faces_2d_projection[i][point_1].y;
		z1 = faces_2d_projection[i][point_1].z;
		point_1 = point_2;
		if (faces_2d_projection[i][point_3].y < faces_2d_projection[i][point_1].y)
		{
			point_1 = point_3;
			point_3 = point_2;
		}
		x2 = faces_2d_projection[i][point_1].x;
		y2 = faces_2d_projection[i][point_1].y;
		z2 = faces_2d_projection[i][point_1].z;
		x3 = faces_2d_projection[i][point_3].x;
		y3 = faces_2d_projection[i][point_3].y;
		z3 = faces_2d_projection[i][point_3].z;

		dz12=(z2-z1)/(y2-y1);
		dz13=(z3-z1)/(y3-y1);
		dz23=(z3-z2)/(y3-y2);
		dx12=(x2-x1)/(y2-y1);
		dx13=(x3-x1)/(y3-y1);
		dx23=(x3-x2)/(y3-y2);

		y=y1;
		xr1=x1;
		xr2=x1;
		zr1=z1;
		zr2=z1;
			

		while(y<y2)
		{
			if(xr1<xr2)
				FillLine(view, &depth[y][0], colours[i], y, xr1, xr2, zr1, zr2);
			else
				FillLine(view, &depth[y][0], colours[i], y, xr2, xr1, zr2, zr1);

			xr1+=dx12;
			xr2+=dx13;
			zr1+=dz12;
			zr2+=dz13;
			y++;
		}

		while(y<=y3){

			if(xr1<xr2)
				FillLine(view, &depth[y][0], colours[i], y, xr1, xr2, zr1, zr2);
			else
				FillLine(view, &depth[y][0], colours[i], y, xr2, xr1, zr2, zr1);

			xr1+=dx23;
			xr2+=dx13;
			zr1+=dz23;
			zr2+=dz13;
			y++;
		}
		// TODO 1: calculation of x, y values fill triangle
		// oblicz przyrosty wartosci wspolrzednych x oraz z
		// miedzy wierzcholkami trojkata

		// wypełnij pierwszą część trójkąta (od y1 do y2)
		// wypełnij drugą część trójkąta (od y2 do y3)

		//Przykład użycia

		// FillLine(view, &depth[/*wiersz trojkata (Y)*/][0], /*kolor wypelnienia*/ colours[i],/*wiersz trojkata (Y)*/,/*współrzędna x wiersza początkowa */,/*współrzędna x wiersza końcowa */,/*współrzędna z wiersza początkowa */,/*współrzędna z wiersza końcowa */);

		//FillLine(view, &depth[240][0], colours[1], 240, 100, 500, 1, 10);	// linia pozioma y 240 z zakresu x 100 d0 500 bufor głębokości zmieniający się od 1 do 10
		//FillLine(view, &depth[240][0], colours[2], 240, 100, 500, 5, 1);
	}	// end for
}

