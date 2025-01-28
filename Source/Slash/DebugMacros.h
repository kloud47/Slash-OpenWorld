#pragma once

#include "DrawDebugHelpers.h"
#define world GetWorld()
#define DRAW_SPHERE(Location) if (world) DrawDebugSphere(world, Location, 25.f, 12, FColor::Blue, true)
#define DRAW_SPHERE_COLOR(Location, Color) if (world) DrawDebugSphere(world, Location, 8.f, 12, Color, false, 5.f)
#define DRAW_LINE(Location, forward) if (world) DrawDebugLine(world, Location, Location + forward * 100.f, FColor::Green, true, -1.f, 0, 1)
#define DRAW_POINT(Location) if (world) DrawDebugPoint(world, Location, 20.f, FColor::Cyan, true, -1.f, 0)
#define DRAW_VECTOR(start, end) if (world) \
	{\
		DrawDebugLine(world, start, end, FColor::Blue, true, -1.f, 0, 1);\
		DrawDebugPoint(world, start, 15.f, FColor::Green, true);\
		DrawDebugPoint(world, end, 15.f, FColor::Red, true);\
	}	
#define DRAW_SPHERE_SINGLEFRAME(Location) if (world) DrawDebugSphere(world, Location, 25.f, 12, FColor::Blue, false, -1)
#define DRAW_LINE_SINGLEFRAME(Location, forward) if (world) DrawDebugLine(world, Location, Location + forward * 100.f, FColor::Green, false, -1.f, 0, 1);
#define DRAW_POINT_SINGLEFRAME(Location) if (world) DrawDebugPoint(world, Location, 20.f, FColor::Cyan, false, -1.f, 0)
#define DRAW_VECTOR_SINGLEFRAME(start, end) if (world) \
	{\
		DrawDebugLine(world, start, end, FColor::Blue, false, -1.f, 0, 1);\
		DrawDebugPoint(world, start, 15.f, FColor::Green, false, -1.f);\
		DrawDebugPoint(world, end, 15.f, FColor::Red, false, -1.f);\
	}